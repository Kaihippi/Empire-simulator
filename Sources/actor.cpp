/* Imports */
#include "../Headers/actor.hpp"


/* Constructor */
/* x -> vertical coordinate */
/* y -> horizontal coordinate */
// New actors always start at 100 HP; World::setWorldActorsRandomly()
// overrides this immediately afterwards for prey/predators (50 and 90
// respectively) so they don't all start completely full.
Actor::Actor(int faction)
{
  faction_ = faction;
  hp_ = 100;
}


/* Getters */
int Actor::getHP()
{
  return(hp_);
}


/* Faction */
/* For the base class: 0 */
int Actor::getFaction()
{
  return( faction_ );
}


/* Setter */
void Actor::setHP(int hp)
{
  hp_ = hp;
}


/* Action */
// Called once per turn for every tile (see World::playTurn()).
// Prey (faction 1) passively regenerate HP over time (representing
// grazing/foraging). Predators (faction 2) steadily lose HP, and can
// only replenish it by catching prey (see World::Move()) - so a
// predator that can't find prey will eventually starve and die (see
// World::Die()). Empty tiles (faction 0) are unaffected.
void Actor::Hunger()
{
  if(faction_ == 1) {
    hp_ += 3;
  } else if(faction_ == 2) {
    hp_ -= 50;
  }
}
