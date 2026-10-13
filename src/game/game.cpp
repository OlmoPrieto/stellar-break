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
  InitWindow(sm_uWindowWidth, sm_uWindowHeight, "stellar-break");
  SetTargetFPS(60);

  m_oPaddle.Init();
  m_vctBalls.emplace_back();
  m_vctBalls[0].Init();
}

// ------------------------

void CGame::Update(float _fDt)
{
  m_bWantClose = IsKeyPressed(KEY_ESCAPE);

  m_oPaddle.Update(_fDt);
  for (uint32 i = 0; i < m_vctBalls.size(); ++i)
  {
    m_vctBalls[i].Update(_fDt);
    m_oPaddle.CheckCollisions(&m_vctBalls[i]);
  }

}

// ------------------------

void CGame::Draw()
{
  BeginDrawing();
  ClearBackground(BLACK);

  m_oPaddle.Draw();
  for (uint32 i = 0; i < m_vctBalls.size(); ++i)
  {
    m_vctBalls[i].Draw();
  }

  EndDrawing();
}

// ------------------------
// ------------------------