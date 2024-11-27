#include <SDL2/SDL.h>
#include <crow.h>
#include <string>
#include <iostream>
#include "utils.cpp"

int main() {
    Int2 one = Int2(20, 10);
    Int2 two = Int2(300, 100);
    float dist = one.distanceTo(two);
    std::string distString = std::to_string(dist);
    std::cout << "Distance between (x: " << std::to_string(one.x) << ", y: " << std::to_string(one.y) << ") and (x: " << std::to_string(two.x) << ", y: " << std::to_string(two.y) << ") is " << distString << std::endl;
}