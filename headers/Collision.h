#pragma once

#include <SDL3/SDL.h>
#include <array>

struct OBB {
    float cx, cy;      // Center position
    float hw, hh;      // Half-width, half-height
    float angle;       // Rotation in radians

    std::array<SDL_FPoint, 4> getCorners() const;
};

class Collision
{
public:
    static bool CheckAABBCollision(const SDL_Rect& rectA, const SDL_Rect& rectB);
    static void ResolveAABBCollision(SDL_Rect& movingRect, const SDL_Rect& staticRect, float& velX, float& velY);
    static bool CheckOBBCollision(const OBB& a, const OBB& b);

private:
    static void projectOntoAxis(const std::array<SDL_FPoint, 4>& corners, const SDL_FPoint& axis, float& min, float& max);
};

