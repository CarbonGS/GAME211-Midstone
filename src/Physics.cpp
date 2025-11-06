#include "Physics.h"

// Applies gravity to the vertical velocity
void Physics::ApplyGravity(float& velY, float gravity, float deltaTime) {
    velY += gravity * deltaTime;
}

// Integrates position using velocity
void Physics::Integrate(float& x, float& y, float velX, float velY, float deltaTime) {
    x += velX * deltaTime;
    y += velY * deltaTime;
}
