#pragma once
#include "GameObject.h"
#include "Player.h"

/*
* A Button provides the framework for an image rendered
* to the screen to have the capability of being clicked on
*/
class Button : public GameObject
{
public:
  Button(const char* fname, int x, int y, int w, int h, Player* p, void(Player::*fn)(Button* b));
  virtual ~Button();

  virtual void updateObj() {};
  virtual void renderObj() { GameObject::renderObj(); };
  virtual void buttonClicked();

protected:
  void(Player::*onClick)(Button* b);
  Player* m_Player;
};

class ShootButton : public Button
{
public:
  ShootButton(int x, int y, Player* p, void(Player::*fn)(Button* b));
  ~ShootButton();

  virtual void updateObj() override;
  virtual void renderObj() override;
  virtual void buttonClicked() { Button::buttonClicked(); };

private:
  Uint8 m_R, m_G, m_B;
};

class AmmoButton : public Button
{
public:
  AmmoButton(int x, int y, Player* p, void(Player::*fn)(Button* b));
  ~AmmoButton();

  virtual void updateObj() override;
  virtual void buttonClicked() { Button::buttonClicked(); };

};
