#pragma once
#include "GameObject.h"
#include "Player.h"

class BarIndicator : public GameObject
{
public:
  BarIndicator(const char* fname, Player::PlayerSide ps, int& val, int x, int y);
  virtual ~BarIndicator() {};

  virtual void updateObj() = 0;
  virtual void renderObj() { GameObject::renderObj(); }

protected:
  int& m_Val;
};

class HealthBar : public BarIndicator
{

public:
  HealthBar(Player::PlayerSide ps, int& val);
  ~HealthBar() {};

  void updateObj() override;

};
class AmmoBar : public BarIndicator
{

public:
  AmmoBar(Player::PlayerSide ps, int& val);
  ~AmmoBar() {};

  void updateObj() override;
};


