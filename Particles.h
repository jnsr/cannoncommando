#pragma once
#include "GameObject.h"
#include "Entity.h"

class Particle :
    public Entity
{
public:
  Particle();
  ~Particle() {};

  void updateObj() override;
  void renderObj() override;

};

class ParticleEmitter :
    public GameObject
{

public:
  ParticleEmitter(int x, int y);
  ~ParticleEmitter();

  void updateObj() override;
  void renderObj() override;

  void blockExplosion();

private:
  Particle m_Particles[25];
};


