#include "Button.h"
#include <iostream>


Button::Button(const char* fname, int x, int y, int w, int h, Player* p, void(Player::*fn)(Button* b)) : GameObject(fname, x, y, w, h)
{
  m_Player = p;
  onClick = fn;
}
Button::~Button() {}

void Button::buttonClicked() {
  if (m_Player->isIdling()) {
    (m_Player->*onClick)(this); 
  }
}

ShootButton::ShootButton(int x, int y, Player* p, void(Player::* fn)(Button* b)) 
  : Button("assets/fire.png", x, y, 68, 68, p, fn) {
  m_R = m_G = m_B = 255;
}
ShootButton::~ShootButton() {}

// Basic Linear Interpolation
Uint8 lerp(Uint8 a, Uint8 b, float t) {
  return ((double)a * (1.0 - t)) + ((double)b * t);
}
void ShootButton::updateObj() {
  // If the button is being held down,
  // interpolate the color from Green -> Red
  if (m_StateTime) {
    float t = m_StateTime / 2000;
    m_R = lerp(0, 255, t);
    m_G = lerp(255, 0, t);
    m_B = lerp(0, 0, t);
  }
  else {
    m_R = m_G = m_B = 255;
  }
}
void ShootButton::renderObj() {
  SDL_SetTextureColorMod(m_Texture, m_R, m_G, m_B);
  SDL_RenderCopyEx(m_ObjRenderer, m_Texture, NULL, &m_ScreenTargetRect, 0.0, NULL, m_Orientation);
}

AmmoButton::AmmoButton(int x, int y, Player* p, void(Player::* fn)(Button* b)) 
  : Button("assets/load.png", x, y, 68, 68, p, fn) {}
AmmoButton::~AmmoButton() {}

void AmmoButton::updateObj() {
  m_TexClipRect.x = m_Player->getCannonLoad() * 68;
}
