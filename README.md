# api-game

An API Game

non-std dependencies:

* [crow](https://crowcpp.org/master/getting_started/setup/linux/), aur: `crow`

## **Files:**

* src/master.cpp, out/master: The master client. This is the one that will dominate, hopefully.
* src/server.cpp, out/server: The server. This hosts the game.
* src/worker.cpp, out/worker: The worker client. This will flock to, and die to, the master.

## **Build:**

* master: clang++ -lSDL2 -lpthread src/master.cpp src/utils.cpp -o out/master
* worker: clang++ -lSDL2 -lpthread src/worker.cpp src/utils.cpp -o out/worker
* server: clang++ -lSDL2 -lpthread src/worker.cpp src/utils.cpp -o out/worker
