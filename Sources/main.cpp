/* Includes */
#include "../Headers/window.hpp"

/* The main application */
//
// Sets up a World from the terrain image, populates it with 4000 prey
// and 1000 predators scattered on random passable tiles, then hands it
// to a Window and runs the render/simulate loop until the window is
// closed.
//
// To load a hand-authored starting layout instead of a random one, swap
// setWorldActorsRandomly(...) for setWorldActorsFromConfig(...) with a
// config file formatted as one "<x> <y> <faction>" line per actor (see
// World::setWorldActorsFromConfig() in Sources/world.cpp).
int main()
{

  World world;
  world.setWorldFromImage("Others/Maailma.png");
  world.setWorldActorsRandomly(4000, 1000);
  //world.setWorldActorsFromConfig("Others/Config.txt");

  Window window;
  window.setWorld(world);

  window.playLoop();

  return(0);
}
