/* Include */
#include "../Headers/world.hpp"

// Loads the terrain from an image: green pixels become passable tiles
// (type 1), every other colour becomes impassable (type 0, e.g. water).
//
// Note on axes: SFML's texture size gives (x = image width in pixels,
// y = image height in pixels), but here height_ is assigned from
// xAndY.x and width_ from xAndY.y - i.e. the names are swapped relative
// to the image's actual width/height. This is consistent throughout the
// rest of the code (Window sizes its render window the same way), so it
// doesn't cause bugs, but it's worth knowing if you're debugging
// coordinates against the source image.
void World::setWorldFromImage(std::string filename)
{

  filename_ = filename;
  texture_.loadFromFile(filename);
  sf::Vector2u xAndY = texture_.getSize();

  height_ = xAndY.x;
  width_ = xAndY.y;

  sf::Image image = texture_.copyToImage();

  for(int i = 0; i < height_; i++) {
    std::vector<Tile> tiles;
    for(int j = 0; j < width_; j++) {
      if(image.getPixel(i, j) == sf::Color::Green) {
        tiles.push_back(Tile(1));
      } else {
        tiles.push_back(Tile(0));
      }
    }
    tiles_.push_back(tiles);
  }

}


// Populates the actor grid from a text file. Every tile starts as an
// empty (faction 0) actor; then each line of the config file is parsed
// as "<x> <y> <faction>" (space-separated) and placed onto the grid,
// provided the target tile is passable. setWorldFromImage() must have
// already run, since this relies on height_/width_/tiles_.
void World::setWorldActorsFromConfig(std::string filename)
{

  for(int i = 0; i < height_; i++) {
    std::vector<Actor> actors;
    for(int j = 0; j < width_; j++) {
      actors.push_back(Actor(0));
    }
    actors_.push_back(actors);
  }

  std::fstream file;
  file.open(filename);
  std::string line;
  while(std::getline(file, line)) {
    std::stringstream ss(line);
    std::string x_a, y_a, tribe_a;
    std::getline(ss, x_a, ' ');
    std::getline(ss, y_a, ' ');
    std::getline(ss, tribe_a, ' ');
    int x, y, tribe;
    x = stoi(x_a);
    y = stoi(y_a);
    if( tiles_[x][y].getType() == 0 ) {
      // Target tile is impassable (e.g. water) - skip this line
      // entirely rather than placing an actor there.
      continue;
    } else {
      tribe = stoi(tribe_a);
    }
    actors_[x][y] = Actor(tribe);

  }
  file.close();
}


// Populates the actor grid by scattering `prey` faction-1 actors and
// `predator` faction-2 actors onto randomly chosen passable tiles.
// Prey start at 50 HP, predators at 90 HP (both below the 100 needed to
// reproduce, so the population needs a few turns of Hunger() before
// anyone can breed). Tiles are picked with simple rejection sampling:
// a random (x, y) is retried until it lands on a passable tile, with no
// check for whether an actor is already there - so on a nearly-full
// grid this could occasionally overwrite a previously placed actor.
void World::setWorldActorsRandomly(int prey, int predator) {

  for(int i = 0; i < height_; i++) {
    std::vector<Actor> actors;
    for(int j = 0; j < width_; j++) {
      actors.push_back(Actor(0));
    }
    actors_.push_back(actors);
  }

  int prey_counter = 0;
  int predator_counter = 0;
  while(prey_counter < prey) {
    int x = rand()%height_;
    int y = rand()%width_;
    if( tiles_[x][y].getType() != 0 ) {
      actors_[x][y] = Actor(1);
      actors_[x][y].setHP(50);
      prey_counter += 1;
    }
  }
  while(predator_counter < predator) {
    int x = rand()%height_;
    int y = rand()%width_;
    if( tiles_[x][y].getType() != 0 ) {
      actors_[x][y] = Actor(2);
      actors_[x][y].setHP(90);
      predator_counter += 1;
    }
  }
  return;

}


// Advances the simulation by one turn. Visits every tile in row-major
// order and runs the four per-tile actions in sequence. Because actors
// are updated in place (no double-buffering), an actor that moves or is
// born during this pass may be visited and acted on again later in the
// same pass if its new position hasn't been reached by the loop yet.
void World::playTurn()
{

  for(int i = 0; i < height_; i++) {
    for(int j = 0; j < width_; j++) {
      Reproduce(i, j);
      Move(i, j);
      Hunger(i, j);
      Die(i, j);
    }
  }
  return;

}


// Attempts to move the actor at (x, y) one step in a random direction.
// Empty tiles (faction 0) never move. Movement/interaction rules,
// checked in order:
//   - off the edge of the grid, or onto impassable terrain -> no-op.
//   - onto an empty tile -> actor moves there, old tile becomes empty.
//   - onto a same-faction actor -> no-op (tile already occupied by
//     "friendly" actor).
//   - prey (1) walking into a predator (2) -> the prey is removed and
//     the predator's HP is restored to 100 (it has eaten).
//   - predator (2) walking into prey (1) -> the destination tile
//     becomes a copy of the predator at full HP. Note: unlike the
//     prey-into-predator case above, the predator's original tile
//     (x, y) is not cleared here, so the predator effectively also
//     remains behind rather than having moved - a quirk of the
//     original logic worth knowing about if you're relying on actor
//     counts.
void World::Move(int x, int y)
{

  if( actors_[x][y].getFaction() == 0 ) {
    return;
  }

  int randomNumber = rand()%(100);
  int dir = 0;
  int futureX, futureY;
  if( randomNumber < 25 ) {
    dir = 1;
  } else if( randomNumber < 50 ) {
    dir = 2;
  } else if( randomNumber < 75 ) {
    dir = 3;
  } else {
    dir = 4;
  }

  if( dir == 1 ) {
    futureX = x - 1;
    futureY = y;
  } else if( dir == 2 ) {
    futureX = x;
    futureY = y + 1;
  } else if( dir == 3 ) {
    futureX = x + 1;
    futureY = y;
  } else {
    futureX = x;
    futureY = y - 1;
  }

  if( futureX < 0 || futureX >= height_ || futureY < 0 || futureY >= width_ ) {
    return;
  }

  if( tiles_[futureX][futureY].getType() == 0 ) {
    return;
  }

  if( actors_[futureX][futureY].getFaction() == 0 ) {
    actors_[futureX][futureY] = actors_[x][y];
    actors_[x][y] = Actor(0);
    return;
  }

  if( actors_[x][y].getFaction() == actors_[futureX][futureY].getFaction() ) {
    return;
  }

  if( actors_[x][y].getFaction() == 1 && actors_[futureX][futureY].getFaction() == 2 ) {
    actors_[x][y] = Actor(0);
    actors_[futureX][futureY].setHP(100);
  }

  if( actors_[x][y].getFaction() == 2 && actors_[futureX][futureY].getFaction() == 1 ) {
    actors_[futureX][futureY] = actors_[x][y];
    actors_[futureX][futureY].setHP(100);
  }

}


// If the actor at (x, y) is at full HP, tries to reproduce onto a
// random adjacent empty, passable tile (same direction-picking logic as
// Move()). Reproduction costs the parent HP either way it's attempted,
// but a child is only actually created if the target tile is empty and
// in-bounds/passable. Prey children start at 40 HP (matching the
// parent's post-reproduction HP); predator children start higher, at
// 70 HP, while the parent still drops to 40.
void World::Reproduce(int x, int y)
{

  if( actors_[x][y].getHP() < 100 || actors_[x][y].getFaction() == 0 ) {
    return;
  }



  int randomNumber = rand()%(100);
  int dir = 0;
  int futureX, futureY;
  if( randomNumber < 25 ) {
    dir = 1;
  } else if( randomNumber < 50 ) {
    dir = 2;
  } else if( randomNumber < 75 ) {
    dir = 3;
  } else {
    dir = 4;
  }

  if( dir == 1 ) {
    futureX = x - 1;
    futureY = y;
  } else if( dir == 2 ) {
    futureX = x;
    futureY = y + 1;
  } else if( dir == 3 ) {
    futureX = x + 1;
    futureY = y;
  } else {
    futureX = x;
    futureY = y - 1;
  }

  if( futureX < 0 || futureX >= height_ || futureY < 0 || futureY >= width_ ) {
    return;
  }

  if( tiles_[futureX][futureY].getType() == 0 ) {
    return;
  }

  if( actors_[futureX][futureY].getFaction() == 0 ) {
    if( actors_[x][y].getFaction() == 1 ) {
      actors_[futureX][futureY] = Actor(1);
      actors_[x][y].setHP(40);
      actors_[futureX][futureY].setHP(40);
    } else {
      actors_[futureX][futureY] = Actor(2);
      actors_[x][y].setHP(40);
      actors_[futureX][futureY].setHP(70);
    }
  }
  return;

}


// Applies this tile's actor's passive per-turn HP change (growth for
// prey, decay for predators - see Actor::Hunger()).
void World::Hunger(int x, int y)
{
  actors_[x][y].Hunger();
}


// Removes (empties) the actor at (x, y) if it has run out of HP.
void World::Die(int x, int y)
{
  if( actors_[x][y].getHP() <= 0 ) {
    actors_[x][y] = Actor(0);
  }
}


int World::getHeight()
{
  return( height_ );
}


int World::getWidth()
{
  return( width_ );
}


std::vector<std::vector<Tile>> World::getTiles()
{
  return( tiles_ );
}


std::vector<std::vector<Actor>>& World::getActors()
{
  return( actors_ );
}


sf::Texture& World::getTexture()
{
  return( texture_ );
}
