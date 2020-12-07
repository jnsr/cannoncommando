#include "GameManager.h"
#include "Engine.h"
#include "NetworkManager.h"
#include "Constants.h"
#include <iostream>

GameManager::GameManager() : m_PlayAgain(false), m_Timer(0.0) {}

void GameManager::trackPlayers(const Player* a, const Player* b) {
  m_P1 = a;
  m_P2 = b;
}

void GameManager::endGameHealth(Player::PlayerSide loser) {
  Engine::s_IsGameRunning = false;
  NetworkManager::disconnectSocket();

  // TODO move this to another function
  if (Engine::s_IsNetworked) {
    Player::PlayerSide nwSide = m_P1->getIsNetworked() ? 
      Player::PlayerSide::LEFT : 
      Player::PlayerSide::RIGHT;

    if (loser == nwSide) {
      Engine::renderScreen("assets/mp-win.png");
    }
    else {
      Engine::renderScreen("assets/mp-lose.png");
    }
  }
  else {
    if (loser == Player::PlayerSide::LEFT)
      Engine::renderScreen("assets/sp-p2.png");
    else if (loser == Player::PlayerSide::RIGHT)
      Engine::renderScreen("assets/sp-p1.png");
  }


}
void GameManager::endGameAmmo(Player::PlayerSide loser) {
  Engine::s_IsGameRunning = false;
  NetworkManager::disconnectSocket();

  // TODO move this to another function
  if (Engine::s_IsNetworked) {
    Player::PlayerSide nwSide = m_P1->getIsNetworked() ? 
      Player::PlayerSide::LEFT : 
      Player::PlayerSide::RIGHT;

    if (loser == nwSide) {
      Engine::renderScreen("assets/mp-win.png");
    }
    else {
      Engine::renderScreen("assets/mp-lose.png");
    }
  }
  else {
    if (loser == Player::PlayerSide::LEFT)
      Engine::renderScreen("assets/sp-p2.png");
    else if (loser == Player::PlayerSide::RIGHT)
      Engine::renderScreen("assets/sp-p1.png");
  }
}

void GameManager::checkGameState() {
  // Check Player Health
  if (m_P1->getHealth() <= 0 || m_P2->getHealth() <= 0) {
    if(m_P1->getHealth() <= 0)
      endGameHealth(Player::PlayerSide::LEFT);
    else if(m_P2->getHealth() <= 0)
      endGameHealth(Player::PlayerSide::RIGHT);
  }

  // Check Player Ammo
  if (m_P1->getAmmo() <= 0 && m_P1->getCannonLoad() == 0) {
    m_Timer += CONSTANTS::DT;
    if (m_Timer >= 3000.0)
      endGameAmmo(Player::PlayerSide::LEFT);
  }
  else if (m_P2->getAmmo() <= 0 && m_P2->getCannonLoad() == 0) {
    m_Timer += CONSTANTS::DT;
    if (m_Timer >= 3000.0)
      endGameAmmo(Player::PlayerSide::RIGHT);
  }

}
void GameManager::getInput() {
  //TODO If user Xs out of the window, need to kill program...

  // Get User Input
  bool input = false;
  while (!input) {
    SDL_Event event;
    SDL_WaitEvent(&event);
    if (event.type == SDL_MOUSEBUTTONDOWN &&
      event.button.button == SDL_BUTTON_LEFT) {

      SDL_Point p;
      p.x = event.button.x;
      p.y = event.button.y;

      SDL_Rect replay;
      replay.x = 345;
      replay.y = 625;
      replay.w = 190;
      replay.h = 60;

      SDL_Rect quit;
      quit.x = 670;
      quit.y = 620;
      quit.w = 190;
      quit.h = 60;

      if (SDL_PointInRect(&p, &replay)) {
        input = true;
        m_PlayAgain = true;
        Engine::s_IsGameRunning = true;
      }
      else if (SDL_PointInRect(&p, &quit)) {
        input = true;
        m_PlayAgain = false;
      }
    }
  }


}
