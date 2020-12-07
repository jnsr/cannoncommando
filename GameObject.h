#pragma once

#include "SDL.h"
#include "SDL_image.h"
#include "Vec2D.h"

/*
* GameObject is the most primitive type able to exist within the game.
* Provides the most basic way to update/render within the game loop.
* More complex classes use GameObject as the parent class
*/
class GameObject {

public:
  GameObject();
  GameObject(const char* fname, int x, int y, int w, int h, SDL_RendererFlip o = SDL_FLIP_NONE);
  virtual ~GameObject();

  bool getIsDeleted() const;
  double getStateTime() const;
  void setStateTime(double st);

  virtual void renderObj();
  virtual void updateObj();

  SDL_Rect& getScreenRect();

protected:
  Vec2D m_Pos;

  // Rect responsible for placing this object on the screen
  SDL_Rect m_ScreenTargetRect;
  // Rect responsible for any sprite sheet manipulation
  SDL_Rect m_TexClipRect;

  SDL_Renderer* m_ObjRenderer;
  SDL_Texture* m_Texture;

  SDL_RendererFlip m_Orientation;

  // Duration in current state
  double m_StateTime;

  bool m_isDeleted;
};

