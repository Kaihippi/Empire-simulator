/* Includes */

/**
 * Tile
 *
 * A single square of terrain in the world grid. Tiles are static - they
 * never change after the world is loaded - and simply record whether
 * actors are allowed to stand/move on that square.
 *
 * `type`:
 *   0 - impassable (e.g. water/ocean in the source image). Actors can
 *       never move onto, be placed on, or reproduce onto this tile.
 *   1 - passable/habitable land. Actors can occupy this tile.
 *
 * See World::setWorldFromImage() for how tile type is derived from the
 * source image (green pixels become type 1, everything else type 0).
 */
struct Tile
{
  Tile(int type);

  /** Returns this tile's type (0 = impassable, 1 = passable). */
  int getType();

  private:
    int type_;
};
