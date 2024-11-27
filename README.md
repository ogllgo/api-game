# api-game

An API Game

non-std dependencies:

* [crow](https://crowcpp.org/master/getting_started/setup/linux/), no pacman, aur: `crow`
* [SDL2](https://wiki.libsdl.org/SDL2/Installation), pacman: `sdl2`

## **Files:**

* src/master.cpp, out/master: The master client. This is the one that will dominate. hopefully.
* src/server.cpp, out/server: The server. This hosts the game.
* src/worker.cpp, out/worker: The worker client. This will flock to, and die to, the master.

## **Build:**

* master: clang++ -lSDL2 -lpthread src/utils.cpp src/circle.cpp src/master.cpp -o out/master
* worker: clang++ -lSDL2 -lpthread src/utils.cpp src/worker.cpp -o out/worker
* server: clang++ -lSDL2 -lpthread src/utils.cpp src/server.cpp -o out/server
