#include "Collision.h"
#include <cmath>
#include <algorithm>

// Returns the four corners of the OBB in world coordinates, accounting for rotation.
std::array<SDL_FPoint, 4> OBB::getCorners() const {
    float cosA = std::cos(angle);
    float sinA = std::sin(angle);
    std::array<SDL_FPoint, 4> corners;
    float dx[] = { -hw,  hw,  hw, -hw };
    float dy[] = { -hh, -hh,  hh,  hh };
    for (int i = 0; i < 4; ++i) {
        corners[i].x = cx + dx[i] * cosA - dy[i] * sinA;
        corners[i].y = cy + dx[i] * sinA + dy[i] * cosA;
    }
    return corners;
}

// Checks for collision between two axis-aligned bounding boxes (AABB).
bool Collision::CheckAABBCollision(const SDL_Rect& rectA, const SDL_Rect& rectB) {
    return (rectA.x < rectB.x + rectB.w &&
            rectA.x + rectA.w > rectB.x &&
            rectA.y < rectB.y + rectB.h &&
            rectA.y + rectA.h > rectB.y);
}

// Resolves collision between a moving AABB and a static AABB by pushing the moving rect out along the smallest axis of overlap.
// Also sets the velocity along the collision axis to zero.
void Collision::ResolveAABBCollision(SDL_Rect& movingRect, const SDL_Rect& staticRect, float& velX, float& velY) {
    int leftA = movingRect.x;
    int rightA = movingRect.x + movingRect.w;
    int topA = movingRect.y;
    int bottomA = movingRect.y + movingRect.h;
    int leftB = staticRect.x;
    int rightB = staticRect.x + staticRect.w;
    int topB = staticRect.y;
    int bottomB = staticRect.y + staticRect.h;
    int overlapLeft = rightA - leftB;
    int overlapRight = rightB - leftA;
    int overlapTop = bottomA - topB;
    int overlapBottom = bottomB - topA;
    int minOverlapX = (overlapLeft < overlapRight) ? overlapLeft : -overlapRight;
    int minOverlapY = (overlapTop < overlapBottom) ? overlapTop : -overlapBottom;
    if (abs(minOverlapX) < abs(minOverlapY)) {
        movingRect.x -= minOverlapX;
        velX = 0;
    } else {
        movingRect.y -= minOverlapY;
        velY = 0;
    }
}

// Projects the four corners of a box onto a given axis and returns the minimum and maximum values.
void Collision::projectOntoAxis(const std::array<SDL_FPoint, 4>& corners, const SDL_FPoint& axis, float& min, float& max) {
    min = max = corners[0].x * axis.x + corners[0].y * axis.y;
    for (int i = 1; i < 4; ++i) {
        float proj = corners[i].x * axis.x + corners[i].y * axis.y;
        if (proj < min) min = proj;
        if (proj > max) max = proj;
    }
}

// Checks for collision between two oriented bounding boxes (OBB) using the Separating Axis Theorem (SAT).
bool Collision::CheckOBBCollision(const OBB& a, const OBB& b) {
    auto ca = a.getCorners();
    auto cb = b.getCorners();
    SDL_FPoint axes[4] = {
        { ca[1].x - ca[0].x, ca[1].y - ca[0].y },
        { ca[3].x - ca[0].x, ca[3].y - ca[0].y },
        { cb[1].x - cb[0].x, cb[1].y - cb[0].y },
        { cb[3].x - cb[0].x, cb[3].y - cb[0].y }
    };
    for (int i = 0; i < 4; ++i) {
        float len = std::sqrt(axes[i].x * axes[i].x + axes[i].y * axes[i].y);
        axes[i].x /= len;
        axes[i].y /= len;
    }
    for (int i = 0; i < 4; ++i) {
        float minA, maxA, minB, maxB;
        projectOntoAxis(ca, axes[i], minA, maxA);
        projectOntoAxis(cb, axes[i], minB, maxB);
        if (maxA < minB || maxB < minA)
            return false;
    }
    return true;
}
