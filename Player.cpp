#include "Player.h"
#include "Engine.h"
#include "NetworkManager.h"
#include "Constants.h"
#include <thread>
#include <iostream>
#include "Constants.h"
#include <chrono>

Player::Player(PlayerSide ps, Cannon* cnn, const char* fname, int x, int y,
  int w, int h, SDL_RendererFlip o) : Entity(fname, x, y, w, h, o)
{
  m_Cnn = cnn;
  m_Health = CONSTANTS::DEF_PLAYER_HEALTH;
  m_Ammo = CONSTANTS::DEF_PLAYER_AMMO;
  m_Side = ps;
  m_Type = EntityType::PLAYER;

  m_isShooting = m_isHit = false;

  m_isNetworked = false;

  m_StateTime = 0.0;

  // Customize hitbox which is a different size than the screen rect
  m_Collider.w = 100;
  m_Collider.h = 120;
  m_Collider.x = m_Pos.x + 50;
  m_Collider.y = m_Pos.y + 20;
}
Player::~Player() {} 

void Player::updateSprite() {

  // Default Idle
  int frm = (int)(SDL_GetTicks() / CONSTANTS::PLAYER_IDLE_F_DUR)
                              % CONSTANTS::PLAYER_IDLE_F_CNT;
  m_TexClipRect.x = m_ScreenTargetRect.w * frm;

  if (m_isHit) {
    m_StateTime += CONSTANTS::DT;
    int frm = (int)(m_StateTime / CONSTANTS::PLAYER_HIT_F_DUR)
                                % CONSTANTS::PLAYER_HIT_F_CNT;
    m_TexClipRect.y = m_ScreenTargetRect.h * 1;
    m_TexClipRect.x = m_ScreenTargetRect.w * frm;
    if (m_StateTime >= CONSTANTS::PLAYER_HIT_DUR) {
      m_isHit = false;
      m_StateTime = 0.0;
      m_TexClipRect.x = 0;
      m_TexClipRect.y = 0;
    }
  }
  if (m_isShooting) {
    m_StateTime += CONSTANTS::DT;
    int frm = (int)(m_StateTime / CONSTANTS::PLAYER_FIRE_F_DUR)
                                % CONSTANTS::PLAYER_FIRE_F_CNT;
    m_TexClipRect.y = m_ScreenTargetRect.h * 2;
    m_TexClipRect.x = m_ScreenTargetRect.w * frm;
    if (m_StateTime >= CONSTANTS::PLAYER_FIRE_DUR) {
      m_isShooting = false;
      m_StateTime = 0.0;
      m_TexClipRect.x = 0;
      m_TexClipRect.y = 0;
    }
  }
}

void Player::updateObj() {
  updateSprite();
  GameObject::updateObj();
}

int Player::getAmmo() const { return m_Ammo; }

void Player::setNetworked(bool b) {
  m_isNetworked = b;
}


/*
* Function that runs on a separate thread,
* responsible for calculating how long button was held
*/
void holdButton(Player* p, Button* b) {
  auto startTime = std::chrono::steady_clock::now();
  int dur = 0;
  while (!Engine::s_mouseUp) {
    auto curTime = std::chrono::steady_clock::now();
    dur = std::chrono::duration_cast<std::chrono::milliseconds>(curTime - startTime).count();
    b->setStateTime(dur);
    if (dur >= 2000) {
      Engine::s_mouseUp = true;
    }
  }
  b->setStateTime(0.0);
  p->shootCannon(dur/50);
}
void Player::shootCannon(Button* b) {
  if (m_Cnn->getLoad()) {
    std::thread buttonHandler (holdButton, this, b);
    buttonHandler.detach();
  }
}
void Player::shootCannon(int p) {
  m_isShooting = true;
  m_Cnn->fire(p, m_Side);

  // Network: Check if the game is online multiplayer and player is local
  if (Engine::s_IsNetworked && !m_isNetworked) {
    std::string pckt = "3," + std::to_string(p);
    std::thread sendP (NetworkManager::sendPacket, pckt);
    sendP.detach();
  }
}

void Player::aimCannonUp(Button* b) {
  int preAng = m_Cnn->getAngle();
  m_Cnn->incrementAngle();
  int postAng = m_Cnn->getAngle();

  // Network
  if (Engine::s_IsNetworked && !m_isNetworked && preAng != postAng) {
    std::string pckt = "1," + std::to_string(postAng);
    std::thread sendP (NetworkManager::sendPacket, pckt);
    sendP.detach();
  }
}
void Player::aimCannonDown(Button* b) {
  int preAng = m_Cnn->getAngle();
  m_Cnn->decrementAngle();
  int postAng = m_Cnn->getAngle();

  // Network
  if (Engine::s_IsNetworked && !m_isNetworked && preAng != postAng) {
    std::string pckt = "1," + std::to_string(postAng);
    std::thread sendP (NetworkManager::sendPacket, pckt);
    sendP.detach();
  }
}
void Player::setCannonAngle(int theta) {
  m_Cnn->setAngle(theta);
}

void Player::loadCannon(Button* b) {
  if (m_Ammo > 0 && m_Cnn->getLoad() < 5) {
    m_Cnn->increaseLoad();
    --m_Ammo;

    // Network
    if (Engine::s_IsNetworked && !m_isNetworked) {
      std::string pckt = "2";
      std::thread sendP (NetworkManager::sendPacket, pckt);
      sendP.detach();
    }
  }
}
int Player::getCannonLoad() const {
  return m_Cnn->getLoad();
}
bool Player::getIsNetworked() const {
  return m_isNetworked;
}

void Player::onCollision(const Entity* with) {
  m_Health -= with->getMomentum() * CONSTANTS::PLAYER_BLOCK_COL_MODIFIER;
  m_isHit = true;
}

int& Player::getHealthRef() { return m_Health; }
int& Player::getAmmoRef() { return m_Ammo; }

bool Player::isIdling() {
  return !m_isShooting && 
         !m_isHit && m_Cnn->isIdling();
}
