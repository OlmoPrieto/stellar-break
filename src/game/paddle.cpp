#include "game/paddle.h"

#include <limits>
#include <cmath>
#include <cassert>

#include "game/game.h"
#include "game/ball.h"

// ------------------------

CPaddle::CPaddle()
{
}

// ------------------------

CPaddle::~CPaddle()
{
  UnloadTexture(*m_spTexture.get());
}

// ------------------------

void CPaddle::Init()
{
  m_spTexture = std::make_shared<Texture2D>(LoadTexture("assets/paddle_basic.png"));

  m_v2Size.x = m_spTexture->width;
  m_v2Size.y = m_spTexture->height;
  m_v2HalfSize = m_v2Size * 0.5f;

  m_v2Pos.x = CGame::sm_uWindowWidth * 0.5f - m_v2HalfSize.x;
  m_v2Pos.y = CGame::sm_uWindowHeight * 0.9f;
}

// ------------------------

void CPaddle::Update(float _fDt)
{
  m_v2Pos.x = GetMouseX() / CGame::sm_uRenderFactor;
}

// ------------------------

void CPaddle::Draw()
{
  // if (m_spTexture.get())
  DrawTextureEx(*m_spTexture.get(), 
   Vector2{m_v2Pos.x - m_v2HalfSize.x, 
     m_v2Pos.y - m_v2HalfSize.y},
     0.0f, 1.0f, WHITE);

  /*
  DrawTextureEx(*m_spTexture.get(),
      Vector2{ m_v2Pos.x - m_v2HalfSize.x,
        m_v2Pos.y - m_v2HalfSize.y },
      0.0f, 1.0f, Color({ (byte)(m_aColor[0] * 255.0f), 
        (byte)(m_aColor[1] * 255.0f), (byte)(m_aColor[2] * 255.0f), 255 }));
  */
}

// ------------------------

void CPaddle::CheckCollisions(CBall* _pBall)
{
  const CVector2D v2BallPos = _pBall->m_v2Pos;
  const CVector2D v2BallHalfSize = _pBall->m_v2HalfSize;
  const float fBallRadius = _pBall->m_fRadius;
  if (v2BallPos.x + fBallRadius > m_v2Pos.x - m_v2HalfSize.x &&
    v2BallPos.x - fBallRadius < m_v2Pos.x + m_v2HalfSize.x &&
    v2BallPos.y + fBallRadius > m_v2Pos.y - m_v2HalfSize.y &&
    v2BallPos.y - fBallRadius < m_v2Pos.y + m_v2HalfSize.y
    )
  {
    _pBall->m_v2Velocity.y *= -1.0f;
    
    float fPercentage = Clamp((v2BallPos.x - m_v2Pos.x) / (m_v2Size.x), -0.5f, 0.5f);
    printf("Percentage: %.3f\n", fPercentage);
    //fPercentage = Remap(fPercentage, -0.5f, 0.5f, -1.0f, 1.0f);
    //printf("Impacted at %.3f%%\n", fPercentage);

    const CVector2D v2BallVelocity = _pBall->m_v2Velocity;
    float fSpeed = std::sqrtf(v2BallVelocity.x * v2BallVelocity.x + v2BallVelocity.y * v2BallVelocity.y);
    constexpr float fPi = 3.14159265f;
    constexpr float fBaseAngle = fPi * 0.5f;
    constexpr float fMaxAngle = 60.0f * (fPi / 180.0f);
    float fNewAngle = fBaseAngle - (fPercentage * 2.0f * fMaxAngle);
    _pBall->m_v2Velocity.x = fSpeed * std::cosf(fNewAngle);
  }
}

// ------------------------

const CBall* CPaddle::GetClosestBall()
{
  const std::vector<CBall>& vctBalls = CGame::GetInstance().GetBalls();

  float fClosestX = std::numeric_limits<float>::max();
  const CBall* pClosestBall = nullptr;

  for (uint32 i = 0; i < vctBalls.size(); ++i)
  {
    const float fX = vctBalls[i].m_v2Pos.x;
    if (fX < fClosestX)
    {
      fClosestX = fX;
      pClosestBall = &vctBalls[i];
    }
  }

  return pClosestBall;
}

// ------------------------

// ------------------------
// ------------------------
// ------------------------