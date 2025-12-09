#include "Scene.h"
#include "Audio.h"
#include "Enemy.h"
#include <iostream>

Scene::Scene(SDL_Renderer* renderer, int width, int height, FMOD::System* fmodSystem)
	: camera(0, 0, width, height, 2.75f), fmodSystem(fmodSystem) // camera zoom is the last float parameter in camera constructor
{
	levelImage = new Image();
	levelImage->LoadSurface("assets/lvl.png");

	backgroundImage = new Image();
	backgroundImage->LoadTexture(renderer, "assets/background.png");
	SDL_Texture* bgTexture = backgroundImage->GetTexture();
	SDL_GetTextureSize(bgTexture,&backH, &backW);

	levelDesigner.LevelDesignerLoad(levelImage);
	levelDesigner.GenerateLevel(renderer);

	// Load all four player images
	Image* idleR = new Image();
	idleR->LoadTexture(renderer, "assets/Player_IdleR.png");
	Image* idleL = new Image();
	idleL->LoadTexture(renderer, "assets/Player_IdleL.png");
	Image* runR = new Image();
	runR->LoadTexture(renderer, "assets/Player_RunR.png");
	Image* runL = new Image();
	runL->LoadTexture(renderer, "assets/Player_RunL.png");
	player = new Player(idleR, idleL, runR, runL, fmodSystem);
	camera.zoom = 2.0f;

	for (Tile* tile : levelDesigner.GetWorldTiles()) { // Set Player Spawn Position
		if (tile->type == Tile::TILE_SPAWN) {
			player->setPosition(static_cast<float>(tile->position.x), static_cast<float>(tile->position.y));
			break;
		}
	}

	// Enemy setup
	enemyTexture = new Image();
	enemyTexture->LoadTexture(renderer, "assets/SlimeR.png");
	enemy = new Enemy(enemyTexture, enemyTexture);
	// Spawn enemy behind player
	if (player) {
		SDL_Rect pBounds = player->GetBounds();
		enemy->SetPositionSync(static_cast<float>(pBounds.x - 64), static_cast<float>(pBounds.y));
	}

	// Audio
	Theme = new Audio(fmodSystem, "assets/audio/Theme.wav");
	Theme->play();

	// Load UI images
	gameUI = new UI(renderer);
}

Scene::~Scene()
{
	delete levelImage;
	delete backgroundImage;
	delete player;
	delete enemyTexture;
	delete enemy;
	delete gameUI;
}

void Scene::Update(float deltaTime)
{
	if (gameFinished) return;
	// Update world tiles (physics for spikes, etc.)
	levelDesigner.UpdateWorldTiles(deltaTime);

	// Update entities
	if (player) {
		player->Update(deltaTime, levelDesigner.GetWorldTiles());
		// Center camera on player
		camera.CenterOn(player->GetBounds().x + player->GetBounds().w / 2,
			player->GetBounds().y + player->GetBounds().h / 2);
		// End game if player HP is zero or less
		if (player->GetHealth() <= 0) {
			gameFinished = true;
			std::cout << "Player died! Game Over." << std::endl;
			return;
		}
	}
	if (enemy && player) {
		enemy->UpdateAIWithCollision(deltaTime, player->GetBounds().x, player->GetBounds().y, levelDesigner.GetWorldTiles());
	}

	// Player Attack/Enemy Knockback
	if (player && enemy && player->IsAttacking() && !enemy->IsInvisible()) {
		// Get centers
		SDL_Rect pRect = player->GetBounds();
		SDL_Rect eRect = enemy->GetBounds();
		float playerCenterX = pRect.x + pRect.w / 2.0f;
		float playerCenterY = pRect.y + pRect.h / 2.0f;
		float enemyCenterX = eRect.x + eRect.w / 2.0f;
		float enemyCenterY = eRect.y + eRect.h / 2.0f;
		float dx = enemyCenterX - playerCenterX;
		float dy = std::abs(enemyCenterY - playerCenterY);
		float range = 48.0f;
		float verticalRange = pRect.h;
		bool inRange = false;
		if (player->GetAttackDirection() == -1) { // left
			inRange = (dx < 0 && std::abs(dx) < range && dy < verticalRange);
		} else if (player->GetAttackDirection() == 1) { // right
			inRange = (dx > 0 && std::abs(dx) < range && dy < verticalRange);
		}
		if (inRange) {
			float knockbackVel = (player->GetAttackDirection() == -1) ? -300.0f : 300.0f;
			enemy->ApplyKnockback(knockbackVel);
			enemy->OnHit(player);
		}
	}

	// Check Collisions
	HandleCollisions();

	// Check for finish line
	SDL_Rect playerRect = player->GetBounds();
	for (const Tile* tile : levelDesigner.GetWorldTiles()) {
		if (tile->type == Tile::TILE_FINISH) {
			if (SDL_HasRectIntersection(&playerRect, &tile->collisionRect)) {
				gameFinished = true;
				std::cout << "You reached the finish line! Game Over." << std::endl;
				break;
			}
		}
	}
	// Other scene updates
}

void Scene::Render(SDL_Renderer* renderer)
{
	// Set Background (static, no parallax, no scaling)
	SDL_FRect bgDestRect = { 0, 0, static_cast<float>(camera.width), static_cast<float>(camera.height) };
	backgroundImage->Render(renderer, nullptr, &bgDestRect);

	// Render World Tiles
	const std::vector<Tile*>& worldTiles = levelDesigner.GetWorldTiles();
	for (Tile* tile : worldTiles) {
		if (tile->type != Tile::TILE_EMPTY && tile->type != Tile::TILE_SPAWN) {
			tile->Render(renderer, camera);
		}
	}

	// Render Player
	if (player) {
		player->Render(renderer, camera);
	}
	if (enemy) {
		enemy->Render(renderer, camera);
	}

	// Render UI
	gameUI->Render(renderer, camera, *player);

	// Debugging: Render collision boxes
	//SDL_SetRenderDrawColor(renderer, 255, 0, 0, 128);
	//for (const Tile* tile : worldTiles) {
	//	if (tile->type != Tile::TILE_EMPTY) {
	//		SDL_FRect rect = {
	//			(float)tile->collisionRect.x - camera.x,
	//			(float)tile->collisionRect.y - camera.y,
	//			(float)tile->collisionRect.w,
	//			(float)tile->collisionRect.h
	//		};
	//		// Apply camera zoom if needed
	//		rect.x *= camera.zoom;
	//		rect.y *= camera.zoom;
	//		rect.w *= camera.zoom;
	//		rect.h *= camera.zoom;
	//		SDL_RenderRect(renderer, &rect);
	//	}
	//}
	//// Set color for player collision box
	//SDL_SetRenderDrawColor(renderer, 0, 255, 0, 128);
	//SDL_Rect playerRect = player->GetBounds();
	//SDL_FRect playerFRect = {
	//	(float)playerRect.x - camera.x,
	//	(float)playerRect.y - camera.y,
	//	(float)playerRect.w,
	//	(float)playerRect.h
	//};
	//playerFRect.x *= camera.zoom;
	//playerFRect.y *= camera.zoom;
	//playerFRect.w *= camera.zoom;
	//playerFRect.h *= camera.zoom;
	//SDL_RenderRect(renderer, &playerFRect);

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
}

void Scene::EventHandler(const SDL_Event& sdlEvent)
{
	if (sdlEvent.type == SDL_EVENT_WINDOW_RESIZED) {
		camera.width = sdlEvent.window.data1; // new width
		camera.height = sdlEvent.window.data2; // new height
		// Re-center camera on player
		if (player) {
			camera.CenterOn(player->GetBounds().x + player->GetBounds().w / 2,
				player->GetBounds().y + player->GetBounds().h / 2);
		}
	}

	// Player Attack Input
	if (sdlEvent.type == SDL_EVENT_MOUSE_BUTTON_DOWN && sdlEvent.button.button == SDL_BUTTON_LEFT) {
		int mx = sdlEvent.button.x;
		int winW = camera.width;
		int direction = (mx < winW / 2) ? -1 : 1;
		if (player) player->Attack(direction);
	}

	if (player) {
		player->HandleInput(sdlEvent);
	}
}

void Scene::HandleCollisions()
{
	if (player && enemy) {
		SDL_Rect pRect = player->GetBounds();
		SDL_Rect eRect = enemy->GetBounds();
		if (SDL_HasRectIntersection(&pRect, &eRect) && !enemy->IsInvisible()) {
			// Enemy attacks at intervals, not every frame
			if (enemy->attackCooldown <= 0.0f) {
				player->TakeDamage(10);
				enemy->attackCooldown = enemy->attackInterval;
			}
		}
	}
}
