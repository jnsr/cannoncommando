#include "Cannon.h"
#include "NetworkManager.h"
#include "Engine.h"
#include "Projectile.h"
#include "Constants.h"
#include <iostream>

Cannon::Cannon(const char* fname, int x, int y, int w, int h, SDL_RendererFlip o) : Entity(fname, x, y, w, h, o) 
{
  m_Angle = CONSTANTS::DEF_CANNON_ANGLE;
  m_Load = 0;
  m_Type = EntityType::CANNON;

  if (m_Orientation == SDL_FLIP_NONE) {
    m_Center.x = 45;
    m_Center.y = 60;
  }
  else {
    m_Center.x = 60;
    m_Center.y = 60;
  }

  // Customize hitbox which is a different size than the screen rect
  m_Collider.x = m_Orientation == SDL_FLIP_HORIZONTAL ? x + 35 : x + 15;
  m_Collider.y = y + 35;
  m_Collider.w = 50;
  m_Collider.h = 50;
}
Cannon::~Cannon() {};

void Cannon::fire(int power, int dir) {

  m_isFiring = true;

  // Larger projectiles should feel heavier
  power -= (double)m_Load * (double)m_Load * 0.5;
  if (power < 0) power = 0;

  std::cout << "Firing Projectile Power: " << power << std::endl;
 
  // Calculate velocity based on angle cannon is aimed
  double theta = (double)m_Angle / 180.0 * M_PI;
  double vx = power * cos(theta) * dir;
  double vy = power * sin(theta);

  // Calculate the 'rotated' initial position for the projectile: rX, rY
  theta = m_Orientation == SDL_FLIP_NONE ? ((double)-m_Angle + 45) / 180.0 * M_PI : ((double)m_Angle - 45) / 180.0 * M_PI;
  double cX = (double)m_ScreenTargetRect.x + (m_ScreenTargetRect.w / 2);
  double cY = (double)m_ScreenTargetRect.y + (m_ScreenTargetRect.h / 2);
  double pX = m_Orientation == SDL_FLIP_NONE ? m_ScreenTargetRect.x + m_ScreenTargetRect.w : m_ScreenTargetRect.x;
  double rX = cos(theta) * (pX - cX) - sin(theta) * (m_ScreenTargetRect.y - cY) + cX;
  double rY = sin(theta) * (pX - cX) + cos(theta) * (m_ScreenTargetRect.y - cY) + cY;

  // Prevent collisions between projectile and firing cannon with additional offset
  rX += m_Orientation == SDL_FLIP_NONE ? 5 : -15;

  std::cout << rX << " " << rY << std::endl;
  Engine::ngnMtx.lock();
  Engine::s_Pending.push(ProjectileInfo(rX, rY, m_Load, vx, vy));
  Engine::ngnMtx.unlock();

  m_Load = 0;
}

int Cannon::getLoad() const { return m_Load; }
int Cannon::getAngle() const { return m_Angle; }
void Cannon::increaseLoad() { 
  m_isLoading = true;
  ++m_Load; 
}

void Cannon::incrementAngle() {
  if (m_Angle < 90){ 
    ++m_Angle;
  }
}
void Cannon::decrementAngle() {
  if (m_Angle > 0) { 
    --m_Angle;
  }
}
void Cannon::setAngle(double angle) {
  if (angle >= 0 && angle <= 90)
    m_Angle = angle;
}

void Cannon::renderObj() {
  //SDL_RenderDrawRect(Engine::s_Renderer, &m_Collider);

  double renderAngle = m_Orientation == SDL_FLIP_HORIZONTAL ?  360 - (-m_Angle + 45) : -m_Angle + 45;
  SDL_RenderCopyEx(m_ObjRenderer, m_Texture, &m_TexClipRect, &m_ScreenTargetRect, renderAngle, &m_Center, m_Orientation);
}

void Cannon::updateSprite() {

  // Default Idle
  m_TexClipRect.x = 0;

  if (m_isFiring) {
    m_StateTime += CONSTANTS::DT;
    int frm = (int)(m_StateTime / CONSTANTS::CNN_FIRE_F_DUR)
                                % CONSTANTS::CNN_FIRE_F_CNT;
    m_TexClipRect.y = m_ScreenTargetRect.h * 1;
    m_TexClipRect.x = m_ScreenTargetRect.w * frm;
    if (m_StateTime >= CONSTANTS::CNN_FIRE_DUR) {
      m_isFiring = false;
      m_StateTime = 0.0;
      m_TexClipRect.x = 0;
      m_TexClipRect.y = 0;
    }
  }
  if (m_isLoading) {
    m_StateTime += CONSTANTS::DT;
    int frm = (int)(m_StateTime / CONSTANTS::CNN_LOAD_F_DUR)
                                % CONSTANTS::CNN_LOAD_F_CNT;
    m_TexClipRect.y = m_ScreenTargetRect.h * 2;
    m_TexClipRect.x = m_ScreenTargetRect.w * frm;
    if (m_StateTime >= CONSTANTS::CNN_LOAD_F_DUR) {
      m_isLoading = false;
      m_StateTime = 0.0;
      m_TexClipRect.x = 0;
      m_TexClipRect.y = 0;
    }
  }
}

void Cannon::updateObj() {
  updateSprite();
}

void Cannon::onCollision(const Entity* with) {
  std::cout << "Cannon was hit!" << std::endl;
  m_Angle = 0.0;
}

bool Cannon::isIdling() {
  return !m_isLoading && !m_isFiring;
}


