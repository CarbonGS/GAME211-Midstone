#include "Player.h"
#include "Camera.h"
#include "Physics.h"
#include "SpikeTile.h"
#include "Collision.h"

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
}

#include <iostream>
void Player::Update(float deltaTime, const std::vector<Tile*>& worldTiles)
{
    timeSinceStart += deltaTime;

    // Update damage cooldown
    if (damageCooldown > 0.0f) {
        damageCooldown -= deltaTime;
        if (damageCooldown < 0.0f)
            damageCooldown = 0.0f;
    }

    // Apply gravity
    Physics::ApplyGravity(velY, GRAVITY, deltaTime);

    // Integrate position
    Physics::Integrate(x, y, velX, velY, deltaTime);

    // Update bounds
    bounds.x = static_cast<int>(x);
    bounds.y = static_cast<int>(y);

    // Check collision with solid tiles
    for (const Tile* tile : worldTiles) {

        if (tile->type == Tile::TILE_PLATFORM) {
            const SDL_Rect& tileRect = tile->collisionRect;

            if (Collision::CheckAABBCollision(this->GetBounds(), tileRect)) {
                SDL_Rect playerRect = this->GetBounds();
                Collision::ResolveAABBCollision(playerRect, tileRect, velX, velY);

                x = static_cast<float>(playerRect.x);
                y = static_cast<float>(playerRect.y);

                if (velY <= 0 && bounds.y + bounds.h <= tileRect.y + tileRect.h) {
                    onGround = true;
                    canDoubleJump = true;
                }

                bounds.x = playerRect.x;
                bounds.y = playerRect.y;
            }
        }

        if (tile->type == Tile::TILE_SPIKE) {
            const SDL_Rect& tileRect = tile->collisionRect;

            if (Collision::CheckAABBCollision(this->GetBounds(), tileRect)) {
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

    // Regular movement (non-dash movement)
    if (dashTimer <= 0.0f) {
        if (leftHeld && !rightHeld)
            velX = -200;
        else if (rightHeld && !leftHeld)
            velX = 200;
        else
            velX = 0;
    }

    // Attack cooldown
    if (attackCooldown > 0) {
        attackCooldown -= deltaTime;
        if (attackCooldown < 0)
            attackCooldown = 0;
    }

    // DASH COOLDOWN
    if (dashCooldown > 0.0f) {
        dashCooldown -= deltaTime;

        if (dashCooldown <= 0.0f) {
            canDash = true;
            dashCooldown = 0.0f;

            // Clear tap history when cooldown ends
            lastLeftTap = -10000.0f;
            lastRightTap = -10000.0f;
        }
    }

    // ACTIVE DASH
    if (dashTimer > 0.0f) {
        dashTimer -= deltaTime;
        velX = dashDir * dashSpeed;

        if (dashTimer <= 0.0f) {
            dashDir = 0;
            dashTimer = 0;
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
            if (rightHeld)
                velX = 200;
            else
                velX = 0;
            break;

        case SDLK_D:
            rightHeld = false;
            if (leftHeld)
                velX = -200;
            else
                velX = 0;
            break;
        }
    }
}
