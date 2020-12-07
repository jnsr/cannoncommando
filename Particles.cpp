#include "Particles.h"
#include "Constants.h"
#include <time.h>

ParticleEmitter::ParticleEmitter(int x, int y){
  m_Pos = Vec2D(x, y);
}
ParticleEmitter::~ParticleEmitter() { m_isDeleted = true; }

void ParticleEmitter::updateObj() {
  for (int i = 0; i < 25; i++) {
    m_Particles[i].updateObj();
  }
}
void ParticleEmitter::renderObj() {
  for (int i = 0; i < 25; i++) {
    m_Particles[i].renderObj();
  }
}

void ParticleEmitter::blockExplosion() {
  srand(time(NULL));
  for (Particle &p : m_Particles) {
    p.m_Grav = Vec2D(0.0, CONSTANTS::DEF_GRAVITY);
    p.m_Pos = Vec2D(m_Pos.x + (5 - rand() % 10), m_Pos.y + (5 - rand() % 10));
    p.m_Vel = Vec2D(20 - rand() % 40, 10 - rand() % 20);
    p.m_ScreenTargetRect.w = (rand() % 50);
    p.m_ScreenTargetRect.h = (rand() % 50);

    p.m_ScreenTargetRect.x = m_Pos.x;
    p.m_ScreenTargetRect.y = m_Pos.y;
  }
}

Particle::Particle() : Entity("assets/particle.png", 0, 0, 0, 0) {
  m_TexClipRect.w = 50;
  m_TexClipRect.h = 50;
}

void Particle::updateObj() {
  m_ScreenTargetRect.w -= 0.05;
  m_ScreenTargetRect.h -= 0.05;
  Entity::updateObj();

}
void Particle::renderObj() {
  SDL_RenderCopyEx(m_ObjRenderer, m_Texture, &m_TexClipRect, &m_ScreenTargetRect, rand() % 180, NULL, m_Orientation);
}


