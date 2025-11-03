#pragma once
#include "Entity.h"
#include "Camera.h"
class Enemy : public Entity
{
public:
	Enemy();
	~Enemy();

	void Update(float deltaTime) override;
	void Render(SDL_Renderer* renderer, const Camera& camera) override;

	void OnHit(Entity* other) override;
	void ChasePlayer(float playerX, float playerY, float deltaTime); // To be implemented (Sprint 2)

private:
	float velX, velY;
};

