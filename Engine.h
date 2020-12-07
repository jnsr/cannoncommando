#pragma once
#include <vector>
#include "SDL.h"
#include "SDL_image.h"
#include "Button.h"
#include "Player.h"
#include "Particles.h"
#include <mutex>
#include <queue>

/*
* Use this to pass around messages about projectile objects
*/
struct ProjectileInfo {
  int x;
  int y;
  int load;
  float vx;
  float vy;
  ProjectileInfo(int px, int py, int l, float velx, float vely)
  : x(px), y(py), load(l), vx(velx), vy(vely) {};
};

/*
* Engine is the singleton responsible for
* the bulk of actual gameplay orchestration.
* It includes global containers for game objects,
* entities, and UI buttons.
*/
class Engine {

public:
  Engine();
  ~Engine();
  
  void initEngine(bool isMp);
  void handleInput();
  void updateEngine();
  void renderEngine();
  void drainQueue();
  void cleanUp();
  void terminate();

  static std::mutex ngnMtx;
  static SDL_Renderer* s_Renderer;
  static bool s_IsGameRunning;
  static bool s_IsNetworked;
  static bool s_mouseUp;

  // All renderable objects
  static std::vector<GameObject*> s_GameObjects;
  // All objects that have collisions/health enabled
  static std::vector<Entity*> s_EntityList;
  // All UI Buttons
  static std::vector<Button*> s_ButtonList;
  // Projectiles added outside the main thread
  static std::queue<ProjectileInfo> s_Pending;

  // The two players in the game, one might be networked
  static Player* s_P1;
  static Player* s_P2;
  
  static void prepareMatch();

  // Add either a locally controlled or networked player to the game
  // if networked, the pref value will be a reference to the created object
  static void addPlayer(Player::PlayerSide ps, bool isNetworked, Player** pref = nullptr);
  static void detectCollision(Entity* ent);
  static bool isCollision(const SDL_Rect& A, const SDL_Rect& B);
  static void notifyCollision(Entity* A, Entity* B);
  static SDL_Texture* loadTexture(const char* fname);
  static void renderScreen(const char* fname);

private:
  static void addBGUpper();
  static void addBGLower();
  static void generateBlocks();

  SDL_Window* m_Window;
};

