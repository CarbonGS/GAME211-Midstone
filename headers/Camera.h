#pragma once
#include <SDL3/SDL.h>

class Camera {
public:
    float x, y;
    int width, height;
    float zoom;

    Camera(float x = 0, float y = 0, int width = 800, int height = 600, float zoom = 1.0f)
        : x(x), y(y), width(width), height(height), zoom(zoom) {}

    SDL_FRect GetViewRect() const {
        return { x, y, static_cast<float>(width), static_cast<float>(height) };
    }

    void Move(float dx, float dy) {
        x += dx;
        y += dy;
    }

    void CenterOn(float targetX, float targetY) {
        x = targetX - (width / (2 * zoom));
        y = targetY - (height / (2 * zoom));
    }
};
