#include "Player.h"
#include "Camera.h"
#include "Physics.h"
#include "SpikeTile.h"
#include "Collision.h"
#include <iostream>

constexpr float GRAVITY = 600.0f; // pixels/sec^2
constexpr float JUMP_VELOCITY = -500.0f; // Upward jump velocity
constexpr float DAMAGE_COOLDOWN_TIME = 1.0f; // seconds

Player::Player(Image* texture)
{
    playerTexture = texture;
    velX = 0;
    velY = 0;
    health = 100;
    attackCooldown = 0;
    onGround = false;
    jumpRequested = false;
    damageCooldown = 0.0f;

    // Assuming other necessary variables are initialized here
    dashTimer = 0.0f;
    dashCooldown = 0.0f;
    dashDir = 0;
    dashSpeed = 600.0f;
    dashDuration = 0.2f;
    canDash = true;
    canDoubleJump = false;
    leftHeld = false;
    rightHeld = false;
    lastLeftTap = -10000.0f;
    lastRightTap = -10000.0f;
    doubleTapThreshold = 0.25f;
    timeSinceStart = 0.0f;
    bounds = { 0, 0, 32, 32 };
    x = static_cast<float>(bounds.x);
    y = static_cast<float>(bounds.y);
}

void Player::Update(float deltaTime, const std::vector<Tile*>& worldTiles)
{
    timeSinceStart += deltaTime;

    // --- Cooldowns and Input Handling ---
    if (damageCooldown > 0.0f) {
        damageCooldown -= deltaTime;
        if (damageCooldown < 0.0f)
            damageCooldown = 0.0f;
    }

    // Handle jump input
    if (jumpRequested) {
        if (onGround) {
            velY = JUMP_VELOCITY;
            onGround = false;
            canDoubleJump = true;
        }
        else if (canDoubleJump) {
            velY = JUMP_VELOCITY;
            canDoubleJump = false;
        }
        jumpRequested = false;
    }

    // Regular movement (set intended velX for this frame)
    if (dashTimer <= 0.0f) {
        if (leftHeld && !rightHeld)
            velX = -200.0f;
        else if (rightHeld && !leftHeld)
            velX = 200.0f;
        else
            velX = 0.0f;
    }

    // Active Dash (override regular movement)
    if (dashTimer > 0.0f) {
        dashTimer -= deltaTime;
        velX = dashDir * dashSpeed;

        if (dashTimer <= 0.0f) {
            dashDir = 0;
            dashTimer = 0.0f;
        }
    }

    // Apply gravity
    Physics::ApplyGravity(velY, GRAVITY, deltaTime);

    // HORIZONTAL position ONLY
    x += velX * deltaTime;
    bounds.x = static_cast<int>(x);

    // Check horizontal collision with solid tiles
    for (const Tile* tile : worldTiles) {
        if (tile->type == Tile::TILE_PLATFORM) {
            const SDL_Rect& tileRect = tile->collisionRect;

            if (Collision::CheckAABBCollision(this->GetBounds(), tileRect)) {

                // Resolve horizontal collision
                if (velX > 0) { // Moving Right (Hit wall on left side)
                    x = static_cast<float>(tileRect.x - bounds.w);
                    velX = 0; // Stop horizontal movement
                }
                else if (velX < 0) { // Moving Left (Hit wall on right side)
                    x = static_cast<float>(tileRect.x + tileRect.w);
                    velX = 0; // Stop horizontal movement
                }

                // Re-update bounds after horizontal push-out
                bounds.x = static_cast<int>(x);
            }
        }
    }

    // VERTICAL position ONLY
    y += velY * deltaTime;
    bounds.y = static_cast<int>(y);

    // Assume not on ground unless a collision resolves it
    onGround = false;

    // Check vertical collision and hazards
    for (const Tile* tile : worldTiles) {
        const SDL_Rect& tileRect = tile->collisionRect;

        if (Collision::CheckAABBCollision(this->GetBounds(), tileRect)) {

            // Resolve Platform Collision
            if (tile->type == Tile::TILE_PLATFORM) {
                // Manually resolve vertical collision
                if (velY > 0) { // Moving Down (Hit floor)
                    y = static_cast<float>(tileRect.y - bounds.h);
                    velY = 0;
                    onGround = true;
                    canDoubleJump = true;
                }
                else if (velY < 0) { // Moving Up (Hit ceiling)
                    y = static_cast<float>(tileRect.y + tileRect.h);
                    velY = 0;
                }

                // Re-update bounds after vertical push-out
                bounds.y = static_cast<int>(y);
            }

            // Resolve Spike Damage (Hazard check)
            if (tile->type == Tile::TILE_SPIKE) {
                if (damageCooldown <= 0.0f) {
                    auto spike = dynamic_cast<const SpikeTile*>(tile);
                    if (spike) {
                        health -= spike->getDamage();
                        damageCooldown = DAMAGE_COOLDOWN_TIME;
                        std::cout << "Player hit spike! Health: " << health << "\n";
                    }
                }
            }
        }
    }

    // --- Remaining Cooldowns ---
    if (attackCooldown > 0.0f) {
        attackCooldown -= deltaTime;
        if (attackCooldown < 0.0f)
            attackCooldown = 0.0f;
    }

    if (dashCooldown > 0.0f) {
        dashCooldown -= deltaTime;
        if (dashCooldown <= 0.0f) {
            canDash = true;
            dashCooldown = 0.0f;
            lastLeftTap = -10000.0f;
            lastRightTap = -10000.0f;
        }
    }
}

void Player::Render(SDL_Renderer* renderer, const Camera& camera)
{
    float zoom = camera.zoom;

    SDL_FRect dst = {
        static_cast<float>(GetBounds().x - camera.x) * zoom,
        static_cast<float>(GetBounds().y - camera.y) * zoom,
        static_cast<float>(GetBounds().w) * zoom,
        static_cast<float>(GetBounds().h) * zoom
    };

    if (playerTexture && playerTexture->GetTexture()) {
        playerTexture->Render(renderer, nullptr, &dst);
    }
}

void Player::OnHit(Entity* other)
{
    if (dynamic_cast<Player*>(other)) {
        return;
    }
}

void Player::HandleInput(const SDL_Event& sdlEvent)
{
    // Ignore repeated keydown events
    if (sdlEvent.key.repeat != 0)
        return;

    if (sdlEvent.type == SDL_EVENT_KEY_DOWN)
    {
        switch (sdlEvent.key.key)
        {
        case SDLK_A:
            leftHeld = true;

            if (canDash && dashDir == 0) {
                if ((timeSinceStart - lastLeftTap) < doubleTapThreshold) {
                    // Start dash
                    dashDir = -1;
                    dashTimer = dashDuration;
                    canDash = false;
                    dashCooldown = 3.0f;
                    lastLeftTap = -10000.0f;
                }
                else {
                    lastLeftTap = timeSinceStart;
                }
            }
            else {
                lastLeftTap = -10000.0f;
            }
            break;

        case SDLK_D:
            rightHeld = true;

            if (canDash && dashDir == 0) {
                if ((timeSinceStart - lastRightTap) < doubleTapThreshold) {
                    // Start dash
                    dashDir = 1;
                    dashTimer = dashDuration;
                    canDash = false;
                    dashCooldown = 3.0f;
                    lastRightTap = -10000.0f;
                }
                else {
                    lastRightTap = timeSinceStart;
                }
            }
            else {
                lastRightTap = -10000.0f;
            }
            break;

        case SDLK_W:
        case SDLK_SPACE:
            jumpRequested = true;
            break;
        }
    }
    else if (sdlEvent.type == SDL_EVENT_KEY_UP)
    {
        switch (sdlEvent.key.key)
        {
        case SDLK_A:
            leftHeld = false;
            break;

        case SDLK_D:
            rightHeld = false;
            break;
        }
    }
}