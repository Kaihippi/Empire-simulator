/* Includes */
#include "../Headers/window.hpp"


void Window::setWorld(World &world)
{
  world_ = world;
  return;
}


// One pixel per tile: window dimensions exactly match the world grid.
void Window::startWindow()
{
  window_.create(sf::VideoMode(world_.getHeight(), world_.getWidth()), "WORLD");
}


void Window::clear()
{
  window_.clear();
}


void Window::display()
{
  window_.display();
}


// Draws the terrain texture (loaded from the source image in
// World::setWorldFromImage()) as a single full-window sprite.
void Window::drawWorld()
{
  sf::Sprite sprite;
  sprite.setTexture(world_.getTexture());
  window_.draw(sprite);
}


// Overlays a single-pixel rectangle on every occupied tile: black for
// prey, red for predators. Empty tiles (faction 0) are left untouched,
// so the terrain shows through.
void Window::drawActors()
{

  sf::RectangleShape rectangle(sf::Vector2f(1.f, 1.f));
  for(int i = 0; i < world_.getHeight(); i++) {
    for(int j = 0; j < world_.getWidth(); j++) {
      if( world_.getActors()[i][j].getFaction() == 1 ) {
        rectangle.setPosition(i, j);
        rectangle.setFillColor(sf::Color::Black);
        window_.draw(rectangle);
      } else if( world_.getActors()[i][j].getFaction() == 2 ) {
        rectangle.setPosition(i, j);
        rectangle.setFillColor(sf::Color::Red);
        window_.draw(rectangle);
      }
    }
  }


}


World Window::getWorld()
{
  return( world_ );
}


sf::RenderWindow& Window::getRenderWindow()
{
  return(window_);
}


// Main loop: open the window, then each frame poll for a close request,
// redraw (terrain + actors), and advance the simulation by one turn.
// Note the simulation speed is tied directly to the frame rate - there
// is no separate timer/throttle, so playTurn() runs as fast as SFML can
// draw frames.
void Window::playLoop()
{
  startWindow();
  while (window_.isOpen())
  {
      // check all the window's events that were triggered since the last iteration of the loop
      sf::Event event;
      while (window_.pollEvent(event))
      {
          // "close requested" event: we close the window
          if (event.type == sf::Event::Closed)
              window_.close();
      }


      // clear the window with black color
      clear();
      drawWorld();
      drawActors();
      display();
      world_.playTurn();
  }

}
