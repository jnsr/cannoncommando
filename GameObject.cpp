#include "GameObject.h"
#include "Engine.h"

GameObject::GameObject(){
  m_ObjRenderer = Engine::s_Renderer;
  m_StateTime = 0.0;
  m_isDeleted = false;
}
GameObject::GameObject(const char* fname, int x, int y, int w, int h, SDL_RendererFlip o)
{
  m_ObjRenderer = Engine::s_Renderer;
  m_Texture = Engine::loadTexture(fname);
  m_Pos = Vec2D(x, y);

  m_Orientation = o;

  m_TexClipRect.x = 0;
  m_TexClipRect.y = 0;

  m_ScreenTargetRect.x = m_Pos.x;
  m_ScreenTargetRect.y = m_Pos.y;

  m_ScreenTargetRect.w = m_TexClipRect.w = w;
  m_ScreenTargetRect.h = m_TexClipRect.h = h;

  m_StateTime = 0.0;

  m_isDeleted = false;
}
GameObject::~GameObject() {}

void GameObject::renderObj() {
  SDL_RenderCopyEx(m_ObjRenderer, m_Texture, &m_TexClipRect, &m_ScreenTargetRect, 0.0, NULL, m_Orientation);
}

void GameObject::updateObj() {
  m_ScreenTargetRect.x = m_Pos.x;
  m_ScreenTargetRect.y = m_Pos.y;
}

SDL_Rect& GameObject::getScreenRect() {
  return m_ScreenTargetRect;
}

double GameObject::getStateTime() const {
  return m_StateTime;
}
void GameObject::setStateTime(double st) {
  m_StateTime = st;
}

bool GameObject::getIsDeleted() const {
  return m_isDeleted;
}
