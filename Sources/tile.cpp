/* Includes */
#include "../Headers/tile.hpp"

Tile::Tile(int type)
{
  type_ = type;
}

int Tile::getType()
{
  return(type_);
}
