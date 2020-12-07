#pragma once
#include "Entity.h"

/*
* Each Player will have a Cannon that they control
* A cannon can be aimed higher, lower, loaded, and fired.
*/
class Cannon : public Entity
{
public:
  Cannon(const char* fname, int x, int y, int w, int h, SDL_RendererFlip o = SDL_FLIP_NONE);
  ~Cannon();

  void fire(int power, int dir);
  int getLoad() const;
  int getAngle() const;
  void incrementAngle();
  void decrementAngle();
  void setAngle(double angle);
  void increaseLoad();

  void updateSprite();
  bool isIdling();

  void renderObj() override;
  void updateObj() override;
  void onCollision(const Entity* with) override;

private:
  int m_Angle;
  int m_Load;

  SDL_Point m_Center;

  bool m_isLoading;
  bool m_isFiring;

};

