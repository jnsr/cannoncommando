#include "Constants.h"
#include "Engine.h"
#include "Block.h"
#include "BarIndicator.h"
#include <thread>
#include <iostream>
#include <time.h>
#include "Projectile.h"

SDL_Renderer* Engine::s_Renderer = nullptr;
bool Engine::s_IsGameRunning = true;
bool Engine::s_mouseUp = true;
bool Engine::s_IsNetworked = false;

std::mutex Engine::ngnMtx;

std::vector<GameObject*> Engine::s_GameObjects;
std::vector<Entity*> Engine::s_EntityList;
std::vector<Button*> Engine::s_ButtonList;

std::queue<ProjectileInfo> Engine::s_Pending;

Player* Engine::s_P1 = nullptr;
Player* Engine::s_P2 = nullptr;

Engine::Engine() : m_Window(nullptr) {}
Engine::~Engine() {}

void Engine::addBGUpper() {
	GameObject* bg = new GameObject("assets/bgup.png", 0, 0, 1200, 800);
	s_GameObjects.push_back(bg);
}
void Engine::addBGLower() {
	GameObject* bg = new GameObject("assets/bglow.png", 0, 610, 1200, 190);
	s_GameObjects.push_back(bg);
}
void Engine::addPlayer(Player::PlayerSide ps, bool isNetworked, Player** pref) {
	if (ps == Player::PlayerSide::LEFT) {
    // P1
    Cannon* p1cn = new Cannon("assets/c1.png", 150, 540, 99, 85);
    Engine::s_GameObjects.push_back(p1cn);
    Engine::s_EntityList.push_back(p1cn);

    Player* p = new Player(Player::PlayerSide::LEFT, p1cn, "assets/p1.png", 0, 495, 198, 135);
    s_GameObjects.push_back(p);
    s_EntityList.push_back(p);
		Engine::s_P1 = p;

		if (!isNetworked) {
      Button* p1AngleUpBtn = new Button("assets/up.png", 40, 680, 56, 25, p, &Player::aimCannonUp);
      Button* p1AngleDownBtn = new Button("assets/down.png", 40, 715, 56, 25, p, &Player::aimCannonDown);
      Button* p1LoadBtn = new AmmoButton(120, 715, p, &Player::loadCannon);
			Button* p1FireBtn = new ShootButton(210, 715, p, &Player::shootCannon);

      Engine::s_GameObjects.push_back(p1FireBtn);
      Engine::s_GameObjects.push_back(p1AngleUpBtn);
      Engine::s_GameObjects.push_back(p1AngleDownBtn);
      Engine::s_GameObjects.push_back(p1LoadBtn);

      Engine::s_ButtonList.push_back(p1FireBtn);
      Engine::s_ButtonList.push_back(p1AngleUpBtn);
      Engine::s_ButtonList.push_back(p1AngleDownBtn);
      Engine::s_ButtonList.push_back(p1LoadBtn);
		}
		else {
			p->setNetworked(true);
			*pref = p;
		}

    BarIndicator* health = new HealthBar(Player::PlayerSide::LEFT, p->getHealthRef());
    BarIndicator* ammo = new AmmoBar(Player::PlayerSide::LEFT, p->getAmmoRef());
    s_GameObjects.push_back(health);
    s_GameObjects.push_back(ammo);
	}
	else if (ps == Player::PlayerSide::RIGHT) {
		// P2
		Cannon* p2cn = new Cannon("assets/c2.png", 950, 540, 99, 85, SDL_FLIP_HORIZONTAL);
		Engine::s_GameObjects.push_back(p2cn);
		Engine::s_EntityList.push_back(p2cn);


		Player* p2 = new Player(Player::PlayerSide::RIGHT, p2cn, "assets/p2.png", 1000, 495, 198, 135, SDL_FLIP_HORIZONTAL);
		s_GameObjects.push_back(p2);
		s_EntityList.push_back(p2);
		Engine::s_P2 = p2;

		if (!isNetworked) {
			Button* p2AngleUpBtn = new Button("assets/up.png", 730, 680, 56, 25, p2, &Player::aimCannonUp);
			Button* p2AngleDownBtn = new Button("assets/down.png", 730, 715, 56, 25, p2, &Player::aimCannonDown);
      Button* p2LoadBtn = new AmmoButton(810, 715, p2, &Player::loadCannon);
			Button* p2FireBtn = new ShootButton(900, 715, p2, &Player::shootCannon);

			Engine::s_GameObjects.push_back(p2FireBtn);
			Engine::s_GameObjects.push_back(p2AngleUpBtn);
			Engine::s_GameObjects.push_back(p2AngleDownBtn);
			Engine::s_GameObjects.push_back(p2LoadBtn);

			Engine::s_ButtonList.push_back(p2FireBtn);
			Engine::s_ButtonList.push_back(p2AngleUpBtn);
			Engine::s_ButtonList.push_back(p2AngleDownBtn);
			Engine::s_ButtonList.push_back(p2LoadBtn);
		}
		else {
			p2->setNetworked(true);
			*pref = p2;
		}
		
    BarIndicator* health = new HealthBar(Player::PlayerSide::RIGHT, p2->getHealthRef());
    BarIndicator* ammo = new AmmoBar(Player::PlayerSide::RIGHT, p2->getAmmoRef());
    s_GameObjects.push_back(health);
    s_GameObjects.push_back(ammo);
	}
}

void Engine::generateBlocks() {
	srand(time(NULL));

	// Random block generation
	for (int i = 0; i < 3; i++) {
		int xoff = 200 + rand() % 300;
		int y = 300 - rand() % 200;
		int w = 50 + rand() % 140;
		int h = 30 + rand() % 100;

    Block* l = new Block("assets/block.png", xoff-(w/2), y, w, h);
    Engine::s_GameObjects.push_back(l);
    Engine::s_EntityList.push_back(l);
    Block* r = new Block("assets/block.png", 1200-xoff-(w/2), y, w, h);
    Engine::s_GameObjects.push_back(r);
    Engine::s_EntityList.push_back(r);
	}

}

void Engine::initEngine(bool isMp) {

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cout << "Error initializing SDL: " << SDL_GetError() << std::endl;
	}

	// GPU Enhanced Textures
	IMG_Init(IMG_INIT_PNG);

	m_Window = SDL_CreateWindow(CONSTANTS::GAME_NAME,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		CONSTANTS::SCREEN_WIDTH,
		CONSTANTS::SCREEN_HEIGHT, 0);
	s_Renderer = SDL_CreateRenderer(m_Window, -1, 0);

	// Anti Aliasing
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "best");

	s_IsNetworked = isMp;

}
void Engine::prepareMatch() {
	// Clear potential stale data from last game
	for (unsigned int i = 0; i < s_GameObjects.size(); i++) {
    delete s_GameObjects[i];
    s_GameObjects.erase(s_GameObjects.begin() + i);
	}
	for (unsigned int i = 0; i < s_EntityList.size(); i++) {
    s_EntityList.erase(s_EntityList.begin() + i);
	}
	for (unsigned int i = 0; i < s_ButtonList.size(); i++) {
    s_ButtonList.erase(s_ButtonList.begin() + i);
	}

	addBGUpper();

	Block* vertical = new Block("assets/vertical.png", 583, 280, 34, 371, false);
	Engine::s_GameObjects.push_back(vertical);
	Engine::s_EntityList.push_back(vertical);

	addBGLower();

	if (!s_IsNetworked) {
    std::cout << "ENGINE: Creating Local SP Game" << std::endl;
    addPlayer(Player::PlayerSide::LEFT, false);
    addPlayer(Player::PlayerSide::RIGHT, false);
    generateBlocks();
	}

}

void Engine::handleInput() {
	SDL_Event event;
  SDL_PollEvent(&event);

	switch (event.type) {
    case SDL_QUIT:
      this->terminate();
      s_IsGameRunning = false;
      break;
    case SDL_MOUSEBUTTONDOWN:
		{
      Engine::s_mouseUp = false;
			SDL_Point p;
			p.x = event.button.x;
			p.y = event.button.y;

			std::cout << p.x << " " << p.y << std::endl;

			if (event.button.button == SDL_BUTTON_LEFT) {
				for (auto b : s_ButtonList) {
					if (SDL_PointInRect(&p, &b->getScreenRect())) {
						b->buttonClicked();
					}
				}
			}
		}
      break;
    case SDL_MOUSEBUTTONUP:
      Engine::s_mouseUp = true;
      break;
    default:
      break;
	}
}

void Engine::updateEngine() {
	drainQueue();
	cleanUp();
	for (auto go : s_GameObjects) {
		go->updateObj();
	}
}

void Engine::renderEngine() {
  SDL_RenderClear(s_Renderer);
	for (auto go : s_GameObjects) {
		go->renderObj();
	}
  SDL_RenderPresent(s_Renderer);
}

void Engine::drainQueue() {
	ngnMtx.lock();
	while (!s_Pending.empty()) {
		ProjectileInfo pinf = s_Pending.front();
		Projectile* p = new Projectile(pinf.x, pinf.y, pinf.load, pinf.vx, pinf.vy);
		s_GameObjects.push_back(p);
		s_EntityList.push_back(p);
		s_Pending.pop();
	}
	ngnMtx.unlock();
}

void Engine::cleanUp() {
	for (unsigned int i = 0; i < s_GameObjects.size(); i++) {
		if (s_GameObjects[i]->getIsDeleted()) {
			delete s_GameObjects[i];
			s_GameObjects.erase(s_GameObjects.begin() + i);
		}
	}
	for (unsigned int i = 0; i < s_EntityList.size(); i++) {
		if (s_EntityList[i]->getIsDeleted()) {
			s_EntityList.erase(s_EntityList.begin() + i);
		}
	}
}

void Engine::terminate() {
	SDL_DestroyRenderer(s_Renderer);
	SDL_DestroyWindow(m_Window);
	IMG_Quit();
	SDL_Quit();
}

SDL_Texture* Engine::loadTexture(const char* fname) {
	SDL_Surface* tmp = IMG_Load(fname);
	SDL_Texture* tex = SDL_CreateTextureFromSurface(s_Renderer, tmp);
	SDL_FreeSurface(tmp);

	return tex;
}

void Engine::renderScreen(const char* fname) {
	GameObject* screen = new GameObject(fname, 0, 0, 1200, 800);
  Engine::s_GameObjects.insert(Engine::s_GameObjects.begin(), screen);
  screen->renderObj();
  SDL_RenderPresent(Engine::s_Renderer);
}

bool Engine::isCollision(const SDL_Rect& A, const SDL_Rect& B) {
	if (A.w == -1 || B.w == -1) { return false; }
	if 
		(
      A.x + A.w >= B.x &&
      B.x + B.w >= A.x &&
      A.y + A.h >= B.y &&
      B.y + B.h >= A.y
		) {
		return true; 
	}

	return false;
}

void Engine::notifyCollision(Entity* A, Entity* B) {
	A->onCollision(B);
	B->onCollision(A);
}

void Engine::detectCollision(Entity* ent) {
	for (auto e : Engine::s_EntityList) {
		if (e != ent) {
			if (Engine::isCollision(ent->getCollider(), e->getCollider()))
				Engine::notifyCollision(ent, e);
		}
	}
}
