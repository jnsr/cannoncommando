#pragma once
#include "Entity.h"
#include "Particles.h"

class Block : public Entity
{
public:
  Block(const char* fname, int x, int y, int w, int h, bool breakable = true);
  ~Block();

  void onCollision(const Entity* with) override;
  void updateObj() override;

  void updateSprite();

private:
  bool m_isBreakable;
  ParticleEmitter* m_ParticleEmtr;
};

