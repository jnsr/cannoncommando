#include "Entity.h"
#include "Engine.h"
#include "Constants.h"
#include <iostream>

Entity::Entity(const char* fname, int x, int y, int w, int h, SDL_RendererFlip o) : GameObject(fname, x, y, w, h, o)
{
  m_Vel = Vec2D(0.0, 0.0);
  m_Grav = Vec2D(0.0, 0.0);

  m_Health = 100;

  m_Collider.w = m_ScreenTargetRect.w;
  m_Collider.h = m_ScreenTargetRect.h;
  m_Collider.x = x;
  m_Collider.y = y;

  m_Mass = 0;
  m_isBreakable = false;

}
Entity::~Entity() {} 

void Entity::updateObj() {
  Vec2D dp = m_Vel * CONSTANTS::DT;
  dp *= CONSTANTS::PX_PER_M;
  m_Pos = m_Pos + dp;
  Vec2D dv = m_Grav * CONSTANTS::DT;
  m_Vel = m_Vel + dv;

  m_Collider.x = m_Pos.x;
  m_Collider.y = m_Pos.y;

  GameObject::updateObj();
}

void Entity::onCollision(const Entity* with) {
  std::cout << "Default Entity Collision" << std::endl;
}

void Entity::renderObj() {
  //SDL_RenderDrawRect(Engine::s_Renderer, &m_Collider);
  GameObject::renderObj(); 
}

int Entity::getHealth() const {
  return m_Health;
}
int Entity::getMass() const {
  return m_Mass;
}
float Entity::getVelMagn() const {
  return m_Vel.length();
}
float Entity::getMomentum() const {
  return getMass() * getVelMagn() * CONSTANTS::PROJ_MOMENTUM_MODIFIER ;
}
Entity::EntityType Entity::getEntType() const {
  return m_Type;
}

SDL_Rect& Entity::getCollider() {
  return m_Collider;
}
