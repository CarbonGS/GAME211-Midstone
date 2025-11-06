#pragma once

class Physics
{
public:
    // Applies gravity to the vertical velocity
    static void ApplyGravity(float& velY, float gravity, float deltaTime);

    // Integrates position using velocity
    static void Integrate(float& x, float& y, float velX, float velY, float deltaTime);
};

