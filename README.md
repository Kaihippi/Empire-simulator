# Empire Simulator

A small C++/SFML predator/prey simulation. Terrain is loaded from an
image (green = passable land, everything else = impassable), then
populated with two kinds of actors that move, eat, reproduce, and die
each turn:

- **Prey** (drawn black) — passively regain HP each turn.
- **Predators** (drawn red) — steadily lose HP each turn, and must eat
  prey (by moving onto their tile) to recover.

## Requirements

- A C++ compiler with `g++`
- [SFML](https://www.sfml.org/) (graphics, window, and system modules)

On Debian/Ubuntu:

```bash
sudo apt install libsfml-dev
```

## Build & run

```bash
make
./main
```

`make` compiles everything in `Sources/` and produces an executable
called `main` in the project root (see the comments in `makefile` for
how the build is wired up — it's a slightly unconventional setup that
recompiles every file on every run, but it works).

## What you'll see

A window opens, sized to match the terrain image (`Others/Maailma.png`
by default), with 4000 prey and 1000 predators scattered randomly on
passable tiles. Each frame, every actor:

1. **Reproduces** — if at full HP, spawns a same-faction offspring on a
   random adjacent empty tile (both parent and child lose HP as a
   result).
2. **Moves** — one step in a random direction. Moving onto an empty
   tile relocates the actor; moving onto another actor either does
   nothing (same faction) or triggers a predator/prey encounter.
3. **Hungers** — prey gain HP, predators lose HP.
4. **Dies** — removed if HP drops to zero or below.

Close the window to stop the simulation.

## Loading a custom starting layout

By default, `main.cpp` scatters actors randomly:

```cpp
world.setWorldActorsRandomly(4000, 1000);
```

To place actors by hand instead, comment that line out and use:

```cpp
world.setWorldActorsFromConfig("Others/config.txt");
```

`config.txt` (see `Others/config.txt` for a small example) is plain
text, one actor per line:

```
<x> <y> <faction>
```

where `faction` is `1` for prey or `2` for predators, and `x`/`y` must
land on a passable tile of the currently loaded terrain image (lines
that don't are silently skipped).

## Using a different terrain image

Any image works as long as the passable areas are pure green
(`sf::Color::Green`) — swap the filename in `main.cpp`'s call to
`world.setWorldFromImage(...)`. `Others/World.png` and
`Others/World.jpeg` are unused reference/scratch images kept from
development; they aren't wired up to anything.

## Known quirks

This was a learning project, and a couple of behaviours in the
simulation rules are worth knowing about if you plan to extend it
(documented in more detail as comments at the relevant code):

- In `World::Move()`, when a predator walks into a prey's tile, the
  predator's original tile isn't cleared — so it can end up appearing
  in two places at once rather than having moved.
- `World::playTurn()` updates actors in place (no double buffering), so
  an actor that already acted this turn can occasionally be acted on
  again later in the same pass if it moved to a not-yet-visited tile.

## Project layout

```
Sources/           .cpp implementation files
Headers/           .hpp header/class declarations
Others/            Terrain images and the actor-config example
makefile           Build rules
```
