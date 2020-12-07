#include "BarIndicator.h"
#define HEALTH_BAR_WIDTH 200

BarIndicator::BarIndicator(const char* fname, Player::PlayerSide ps, int& val, int x, int y) 
  : GameObject(fname, x, y, 200, 10), m_Val(val)
{}

HealthBar::HealthBar(Player::PlayerSide ps, int& val)
  : BarIndicator("assets/healthbar.png", ps, val, 0, 0)
{
  if (ps == Player::PlayerSide::LEFT) {
    m_ScreenTargetRect.x = 120;
    m_ScreenTargetRect.y = 660;
  }
  else {
    m_ScreenTargetRect.x = 810;
    m_ScreenTargetRect.y = 660;
  }
}

void HealthBar::updateObj() {
  if(m_Val >= 0 && m_Val <= 100)
    m_ScreenTargetRect.w = ((float)m_Val / 100) * HEALTH_BAR_WIDTH;
}

AmmoBar::AmmoBar(Player::PlayerSide ps, int& val)
  : BarIndicator("assets/ammobar.png", ps, val, 0, 0)
{
  if (ps == Player::PlayerSide::LEFT) {
    m_ScreenTargetRect.x = 120;
    m_ScreenTargetRect.y = 680;
  }
  else {
    m_ScreenTargetRect.x = 810;
    m_ScreenTargetRect.y = 680;
  }
}

void AmmoBar::updateObj() {
  if(m_Val >= 0 && m_Val <= 50)
    m_ScreenTargetRect.w = ((float)m_Val / 50) * HEALTH_BAR_WIDTH;
}

