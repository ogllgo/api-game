#include "utils.h"
#include <cmath>

float Int2::angleTo(Int2& other) {
    int dx = this->x - other.x;
    int dy = this->y - other.y;
    return std::atan2(dy, dx);
}

float Int2::distanceTo(Int2& other) {
    int a = this->x - other.x;
    int b = this->y - other.y;
    return std::sqrt(a*a+b*b);
}

float Float2::angleTo(Float2& other) {
    float dx = this->x - other.x;
    float dy = this->y - other.y;
    return std::atan2(dy, dx);
}

float Float2::distanceTo(Float2& other) {
    float a = this->x - other.x;
    float b = this->y - other.y;
    return std::sqrt(a*a+b*b);
}