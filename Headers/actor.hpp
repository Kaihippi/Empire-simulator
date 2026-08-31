#pragma once

/* Includes */
#include <vector>

/**
 * Actor
 *
 * A single creature occupying one tile of the world grid. An Actor's
 * behaviour is entirely driven by its `faction`:
 *
 *   0 - empty / no actor present on this tile (the "nobody here" value
 *       used to represent unoccupied tiles - see World::Die()).
 *   1 - prey. Gains HP over time (see Hunger()) and is eaten by
 *       predators.
 *   2 - predator. Loses HP over time and must eat prey (see
 *       World::Move()) to recover.
 *
 * World owns a 2D grid of Actors (one per tile) and drives their
 * behaviour each turn via World::playTurn().
 */
struct Actor
{

  /** Creates an actor of the given faction with full (100) starting HP.
   *  Callers that want a different starting HP (e.g.
   *  World::setWorldActorsRandomly()) call setHP() right afterwards. */
  Actor(int faction);

  /** Current hit points. Faction 0 (empty) actors are never checked for
   *  HP in practice, but still carry whatever value they were given. */
  int getHP();

  /** Faction of this actor: 0 = empty, 1 = prey, 2 = predator. */
  int getFaction();

  /** Overwrites this actor's current HP. */
  void setHP(int hp);

  /** Applies one turn's worth of passive HP change based on faction:
   *  prey (1) slowly gain HP, predators (2) steadily lose HP. Has no
   *  effect on empty tiles (faction 0). See actor.cpp for the exact
   *  amounts. */
  void Hunger();

  private:
    // x_/y_ are currently unused (grid position is tracked externally
    // by World via the actors_ 2D vector's own indices), but are kept
    // here as fields for potential future use.
    int x_;
    int y_;
    int hp_;
    int faction_;

};
