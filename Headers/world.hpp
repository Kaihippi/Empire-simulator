#pragma once

/* Includes */
#include "actor.hpp"
#include "tile.hpp"

#include <SFML/Graphics.hpp>

#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

/**
 * World
 *
 * Owns the whole simulation state: a grid of terrain Tiles (loaded once
 * from an image and never changed) and a same-sized grid of Actors
 * (prey/predators/empty, updated every turn). World also owns the SFML
 * texture used to draw the terrain, since it's derived from the same
 * source image used to build the tile grid.
 *
 * Typical setup/usage (see Sources/main.cpp):
 *   World world;
 *   world.setWorldFromImage("Others/Maailma.png");     // terrain
 *   world.setWorldActorsRandomly(4000, 1000);          // or ...FromConfig()
 *   // then repeatedly call world.playTurn() once per simulated tick.
 */
struct World {

  /* Getters */

  /** Grid height, in tiles. Derived from the source image's dimensions
   *  in setWorldFromImage() - see the note there about the height/width
   *  axis naming. */
  int getHeight();

  /** Grid width, in tiles. */
  int getWidth();

  /** Returns a copy of the terrain grid. */
  std::vector<std::vector<Tile>> getTiles();

  /** Returns a reference to the actor grid (mutable, so callers such as
   *  Window can inspect current actor positions/factions each frame). */
  std::vector<std::vector<Actor>>& getActors();

  /** Returns the SFML texture generated from the source image, used by
   *  Window to draw the terrain background. */
  sf::Texture& getTexture();

  /* Setters */

  /** Loads the terrain from an image file: builds the tile grid (green
   *  pixels -> passable, everything else -> impassable) and keeps the
   *  loaded texture around for rendering. Must be called before any of
   *  the ...Actors... setters below, since they size the actor grid to
   *  match height_/width_. */
  void setWorldFromImage(std::string filename);

  /** Populates the actor grid from a plain-text config file, one actor
   *  per line formatted as "x y faction" (space-separated). Lines that
   *  target an impassable tile are skipped. All other tiles start out
   *  as empty (faction 0) actors. */
  void setWorldActorsFromConfig(std::string filename);

  /** Populates the actor grid by scattering `prey` faction-1 actors
   *  (starting HP 50) and `predator` faction-2 actors (starting HP 90)
   *  onto random passable tiles. Keeps retrying random tiles until both
   *  quotas are met, so this can occasionally overwrite a
   *  just-placed actor if the same tile is picked twice. */
  void setWorldActorsRandomly(int prey, int predator);

  /* Action */

  /** Advances the whole world by one turn: for every tile, in
   *  row-major order, runs Reproduce -> Move -> Hunger -> Die (see
   *  each method below). Because this is a single in-place pass rather
   *  than a double-buffered update, an actor that has already moved
   *  this turn can be acted on again later in the same pass if it
   *  moved to a not-yet-visited tile. */
  void playTurn();

  /** Attempts to move the actor at (x, y) one tile in a random
   *  direction (25% chance each for up/down/left/right). Handles
   *  bumping into empty tiles (moves), impassable terrain or grid edges
   *  (no-op), same-faction actors (no-op), and cross-faction encounters
   *  (predator/prey combat - see world.cpp for the exact rules). */
  void Move(int x, int y);

  /** If the actor at (x, y) is at full HP (100) and not empty, attempts
   *  to spawn a new same-faction actor on a random adjacent empty,
   *  passable tile. Both parent and child end up at reduced HP
   *  afterwards (see world.cpp for the exact values, which differ
   *  between prey and predators). */
  void Reproduce(int x, int y);

  /** Applies the actor at (x, y)'s passive per-turn HP change (see
   *  Actor::Hunger()). */
  void Hunger(int x, int y);

  /** Removes the actor at (x, y) (replacing it with an empty faction-0
   *  actor) if its HP has dropped to zero or below. */
  void Die(int x, int y);

  private:
    std::string filename_;

    int height_;
    int width_;

    sf::Texture texture_;

    std::vector<std::vector<Tile>> tiles_;
    std::vector<std::vector<Actor>> actors_;

};
