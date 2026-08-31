# Builds the "main" executable from Sources/*.cpp, linking against
# SFML's graphics/window/system modules, then deletes the intermediate
# object files.
#
# Usage:
#   make    - build ./main
#
# How this works: each "<file>.cpp:" rule below has no listed
# prerequisites, and no file named e.g. "window.cpp" exists in this
# top-level directory (the real source lives in Sources/window.cpp), so
# make always considers that target "missing" and reruns its compile
# recipe every time. In effect, every source file is recompiled on every
# `make` invocation - there's no incremental/partial rebuild here, and
# the "X.hpp: Y.hpp Y.cpp" lines exist only to chain these always-rerun
# rules together in the right order, not to express real header
# dependencies. This works but is non-standard; a conventional pattern
# rule (e.g. "%.o: Sources/%.cpp") with real dependency tracking would
# be more typical and would allow incremental rebuilds.
main: main.o
	g++ -o main main.o actor.o tile.o world.o window.o  -lsfml-graphics -lsfml-window -lsfml-system
	rm main.o actor.o tile.o world.o window.o

main.o: window.hpp
	g++ -c Sources/main.cpp

window.hpp: world.hpp window.cpp

window.cpp:
	g++ -c Sources/window.cpp

world.hpp: actor.hpp tile.hpp world.cpp

world.cpp:
	g++ -c Sources/world.cpp

actor.hpp: actor.cpp

actor.cpp:
	g++ -c Sources/actor.cpp

tile.hpp: tile.cpp

tile.cpp:
	g++ -c Sources/tile.cpp
