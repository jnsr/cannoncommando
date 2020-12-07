#include "Block.h"
#include "Constants.h"
#include "Engine.h"
#include <iostream>

Block::Block(const char* fname, int x, int y, int w, int h, bool breakable) : Entity(fname, x, y, w, h)
{
  m_isBreakable = breakable;
  m_Type = EntityType::BLOCK;


  if (m_isBreakable) {
    // Blocks vary in size, but the texture is static
    m_TexClipRect.w = 190;
    m_TexClipRect.h = 130;

    m_Health = 150;

    m_ParticleEmtr = new ParticleEmitter(x+(w/2), y+(h/2));
    Engine::s_GameObjects.push_back(m_ParticleEmtr);
  }
}
Block::~Block() { }

void Block::updateSprite() {

  // Set the sprite based on block's health
  if (m_Health >= 90) {
    m_TexClipRect.x = 0;
  }
  else if (m_Health < 90 && m_Health >= 60) {
    m_TexClipRect.x = 190;
  }
  else if (m_Health < 60 && m_Health >= 30) {
    m_TexClipRect.x = 190 * 2;
  }
  else if (m_Health < 30 && m_Health >= 1) {
    m_TexClipRect.x = 190 * 3;
  }
  else if (m_Health <= 0) {
    if (m_StateTime == 0.0) {
      m_ParticleEmtr->blockExplosion();

      m_ScreenTargetRect.w = 0;
      m_ScreenTargetRect.h = 0;
      m_Collider.w = -1;
    }
    m_StateTime += CONSTANTS::DT;

    if (m_StateTime >= CONSTANTS::BLOCK_EXPD_DUR) {
      m_isDeleted = true;
    }
  }
}

void Block::updateObj() {
  updateSprite();
  Entity::updateObj();
}

void Block::onCollision(const Entity* with) {
  float m = with->getMomentum();
  std::cout << "Block was hit with momentum: " << m << std::endl;

  if (m_isBreakable) {
    std::cout << "Block was hit" << std::endl;
    m_Health -= m;
  }
  else {
    std::cout << "Vertical was hit" << std::endl;
    m_Pos.y += m;
  }
}
