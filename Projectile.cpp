#include "Constants.h"
#include "Projectile.h"
#include "Engine.h"
#include <iostream>

Projectile::Projectile(int x, int y, int size, float velx, float vely, SDL_RendererFlip o) : Entity("assets/pro.png", x, y, size*10, size*10, o)
{
  m_Vel = Vec2D(velx, -vely);
  m_Grav = Vec2D(0.0, CONSTANTS::DEF_GRAVITY);
  m_Mass = size;
  m_isExploded = false;
  m_Type = EntityType::PROJ;

  // Projectiles vary in size, but the texture is static 50x50
  m_TexClipRect.w = m_TexClipRect.h = 50;
}
Projectile::~Projectile() {} 

void Projectile::simulateObj() {
  // If already exploded, nothing left to simulate
  if (m_isExploded) return;

  // Set bounds for the projectile
  if (m_Pos.x > 1250 || m_Pos.x < -50 || m_Pos.y > 605) {
    explode();
    return;
  }

  Entity::updateObj();
  //std::cout << "Position: " << m_Pos.x << " " << m_Pos.y << std::endl;
  //std::cout << "Vel: " << m_Vel.x << " " << m_Vel.y << std::endl;

  // AABB Collision Detection
  Engine::detectCollision(this);
}
void Projectile::updateSprite() {
  // Default Idle
  int frm = (int)(SDL_GetTicks() / CONSTANTS::PROJ_IDLE_F_DUR)
                              % CONSTANTS::PROJ_IDLE_F_CNT;
  m_TexClipRect.x = 50 * frm;

  // Trigger the animation only once (when collider isnt yet disabled)
  if (m_isExploded && m_Collider.w != -1) {
    m_StateTime += CONSTANTS::DT;

    int frm = (int)(m_StateTime / CONSTANTS::PROJ_EXPLD_F_DUR)
                                % CONSTANTS::PROJ_EXPLD_F_CNT;
    m_TexClipRect.y = 50;
    m_TexClipRect.x = 50 * frm;
    if (m_StateTime >= CONSTANTS::PROJ_EXPLD_DUR) {
      // Remove the projectile from the screen and memory
      m_Collider.w = -1;
      m_ScreenTargetRect.w = 0;
      m_ScreenTargetRect.h = 0;
      m_isDeleted = true;
    }
  }
}

void Projectile::updateObj() {
  this->simulateObj();
  this->updateSprite();
}

void Projectile::explode() {
  std::cout << "Projectile Exploded!" << std::endl;
  m_isExploded = true;
}

void Projectile::onCollision(const Entity* with) {
  /*
  * By default when a projectile needs to update itself due to collision,
  * it will explode. However a unique case is where the projectile
  * comes into contact with another projectile. In this case the momentum
  * is the deciding factor of who breaks who.
  */
  switch (with->getEntType()) {
    case EntityType::PROJ:
      if (this->getMomentum() < with->getMomentum())
        explode();
      break;
    default:
      explode();
      break;
  }
}

