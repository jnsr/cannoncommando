#pragma once
#include "Entity.h"
#include "Cannon.h"

class Button; // Forward delcaration

/*
* Player is pretty self explanatory.
* This class provides a mechanism by which
* to control a player object.
* E.g. Fire cannon, aim it, etc..
* Every Player will have ownership of a Cannon object
*/
class Player : public Entity
{
public:
  enum PlayerSide{ LEFT = 1, RIGHT = -1 };

  Player(PlayerSide ps, Cannon* cnn, const char* fname, int x, int y, int w, int h, SDL_RendererFlip o = SDL_FLIP_NONE);
  ~Player();

  void updateObj() override;
  void updateSprite();
  
  int getAmmo() const;
  bool getIsNetworked() const;

  bool isIdling();

  int& getHealthRef();
  int& getAmmoRef();

  void shootCannon(int p);
  void shootCannon(Button* b = nullptr);
  void aimCannonUp(Button* b = nullptr);
  void aimCannonDown(Button* b = nullptr);
  void loadCannon(Button* b = nullptr);
  int getCannonLoad() const;
  void setNetworked(bool b);

  void setCannonAngle(int theta);

  void onCollision(const Entity* with) override;

private:
  int m_Ammo;
  bool m_isNetworked;
  
  // States
  bool m_isShooting;
  bool m_isHit;

  PlayerSide m_Side;
  Cannon* m_Cnn;
};

