#include "game/game.h"

#include <random>
#include <chrono>

#include "raylib/raylib.h"
#include "base/network_serialization.h"
#include "game/network_protocol.h"

// ------------------------------------------------
// ------------------------------------------------


// ------------------------

CGame::CGame()
{

}

// ------------------------

CGame::~CGame()
{
  for (uint32 i = 0; i < 1; ++i)
  {
    m_vctBalls[i].UnloadResources();
  }

  if (m_bWantClose)
  {
    CloseWindow();
  }
}

// ------------------------

void CGame::Init()
{
  InitWindow(sm_uVirtualWindowWidth, sm_uVirtualWindowHeight, "stellar-break");
  SetTargetFPS(60);

  m_oRenderTarget = LoadRenderTexture(sm_uWindowWidth, sm_uWindowHeight);
  SetTextureFilter(m_oRenderTarget.texture, TEXTURE_FILTER_POINT);

  m_oPaddle.Init();

  m_vctBalls.emplace_back();
  m_vctBalls[0].Init();

  for (uint32 i = 0; i < 10; ++i)
  {
    m_vctBlocks.emplace_back(1);
    m_vctBlocks[i].Init();
    m_vctBlocks[i].m_v2Pos.x += i * m_vctBlocks[i].m_v2Size.x;
  }
}

// ------------------------

void CGame::Update(float _fDt)
{
  m_bWantClose = IsKeyPressed(KEY_ESCAPE);

  m_oPaddle.Update(_fDt);

  for (uint32 i = 0; i < m_vctBlocks.size(); ++i)
  {
    m_vctBlocks[i].Update(_fDt);
  }

  for (uint32 i = 0; i < m_vctBalls.size(); ++i)
  {
    m_vctBalls[i].Update(_fDt);
    const int32 iIndex = m_vctBalls[i].CheckCollisions(m_vctBlocks);
    if (iIndex != -1)
    {
      m_vctBlocks.erase(m_vctBlocks.begin() + (uint32)iIndex);
    }
    m_oPaddle.CheckCollisions(&m_vctBalls[i]);
  }

}

// ------------------------

void CGame::Draw()
{
  //BeginDrawing();
  BeginTextureMode(m_oRenderTarget);
  ClearBackground(BLACK);

  m_oPaddle.Draw();
  
  for (uint32 i = 0; i < m_vctBlocks.size(); ++i)
  {
    m_vctBlocks[i].Draw();
  }
  
  for (uint32 i = 0; i < m_vctBalls.size(); ++i)
  {
    m_vctBalls[i].Draw();
  }

  //EndDrawing();
  EndTextureMode();

  // -----
  BeginDrawing();
  ClearBackground(BLACK);

  // Volteamos la textura en el eje Y (Raylib dibuja las texturas de render invertidas por defecto)
  Rectangle sourceRec = { 0.0f, 0.0f, (float)m_oRenderTarget.texture.width, -(float)m_oRenderTarget.texture.height };

  // El rectángulo de destino será del tamaño total de la ventana física actual
  Rectangle destRec = { 0.0f, 0.0f, (float)GetScreenWidth(), (float)GetScreenHeight() };

  // Dibujamos el lienzo virtual estirándolo de forma perfecta en la ventana grande
  DrawTexturePro(m_oRenderTarget.texture, sourceRec, destRec, Vector2{ 0, 0 }, 0.0f, WHITE);

  EndDrawing();
}

// ------------------------
// ------------------------