#pragma once
#include "Entity.h"

/*
* Projectiles simulate projectile motion
*/
class Projectile : public Entity
{
public:
  Projectile(int x, int y, int size, float velx, float vely, SDL_RendererFlip o = SDL_FLIP_NONE);
  ~Projectile();

  void updateObj() override;
  void onCollision(const Entity* with) override;
  void simulateObj();
  void updateSprite();

  void explode();
private:
  bool m_isExploded;
};

