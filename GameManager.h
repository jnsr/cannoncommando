#pragma once
#include "Player.h"

class GameManager
{
public:
  GameManager();
  ~GameManager(){}

  void trackPlayers(const Player* a, const Player* b);

  /*
  * Responsible for checking all the different scenarios
  * where a player has won/lost the game. If there is a 
  * winner/loser then this function halts the game and
  * waits for the user to choose to play again or not.
  */
  void checkGameState();
  /*
  * Blocking function that checks whether the player
  * wants to play again or not. Stores input as member var.
  * Based on the user's input they will either choose to play
  * again or not. If not, the manager will simply allow the 
  * program to terminate. Otherwise, it will have to do some
  * re-init on the engine side to prepare for a new game.
  */
  void getInput();

  bool isPlayAgain() { return m_PlayAgain; }

private:
  void endGameHealth(Player::PlayerSide loser);
  void endGameAmmo(Player::PlayerSide loser);

  const Player* m_P1;
  const Player* m_P2;
  bool m_PlayAgain;
  double m_Timer;
};

