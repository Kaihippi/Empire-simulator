#pragma once

/* Includes */
#include "world.hpp"

#include <SFML/Graphics.hpp>

#include <iostream>

/**
 * Window
 *
 * Owns the SFML render window and drives the render/simulate loop.
 * Holds its own copy of the World (see setWorld()) and is responsible
 * for drawing it every frame and advancing the simulation between
 * frames.
 */
struct Window
{

  /** Stores a copy of `world` to render/simulate. Called once, before
   *  startWindow()/playLoop(), from main.cpp. Note this copies the
   *  entire World (including its tile/actor grids), so the caller's
   *  original World and the Window's internal copy are independent
   *  after this point. */
  void setWorld(World &world);

  /** Creates the SFML render window, sized to match the world's
   *  height/width (one pixel per tile). Called automatically by
   *  playLoop(). */
  void startWindow();

  /** Clears the window (call before drawing a new frame). */
  void clear();

  /** Presents the current frame to the screen (call after drawing). */
  void display();

  /** Draws the terrain as a single sprite using the world's texture. */
  void drawWorld();

  /** Draws one 1x1 pixel rectangle per occupied tile on top of the
   *  terrain: black for prey (faction 1), red for predators (faction
   *  2). Empty tiles are left showing the terrain underneath. */
  void drawActors();

  /** Returns a copy of the Window's internal World. */
  World getWorld();

  /** Returns a reference to the underlying SFML render window. */
  sf::RenderWindow& getRenderWindow();

  /** Runs the main loop: opens the window, then repeatedly polls
   *  events (closing the window on a close request), draws a frame
   *  (terrain + actors), and advances the simulation by one turn -
   *  until the window is closed. */
  void playLoop();

  private:
    World world_;

    sf::RenderWindow window_;

};
