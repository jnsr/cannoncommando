#pragma once
#include "GameObject.h"

/*
* Entity introduces the ability for an object to collide (physics)
* and also introduces the concept of health.
*/
class Entity : public GameObject
{
public:
  enum EntityType{ PLAYER, CANNON, PROJ, BLOCK, POWERUP };

  Entity(const char* fname, int x, int y, int w, int h, SDL_RendererFlip o = SDL_FLIP_NONE);
  virtual ~Entity();

  int getHealth() const;
  int getMass() const;
  float getVelMagn() const;
  float getMomentum() const;
  EntityType getEntType() const;

  virtual void renderObj();
  virtual void updateObj();
  virtual void onCollision(const Entity* with);

  SDL_Rect& getCollider();

protected:
  Entity::EntityType m_Type;

  bool m_isBreakable;
  Vec2D m_Vel;
  Vec2D m_Grav;

  SDL_Rect m_Collider;

  int m_Health;
  int m_Mass;
 
private:
  friend class ParticleEmitter;
};

