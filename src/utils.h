#ifndef SHARED_H
#define SHARED_H

struct Int2 {
    int x, y;
    Int2(int inX, int inY) :
        x(inX),
        y(inY)
    {}
    float distanceTo(Int2& other);
    float angleTo(Int2& other);
};

struct Float2 {
    float x, y;
    Float2(float inX, float inY) :
        x(inX),
        y(inY)
    {}
    float distanceTo(Float2& other);
    float angleTo(Float2& other);
};

#endif