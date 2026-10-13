#pragma once

#include <chrono>
#include <vector>

#include "base/types.h"
#include "raylib/raylib.h"
#include "game/ball.h"
#include "game/paddle.h"
#include "game/block.h"

struct SNetStream;

class CGame
{
public:
  static constexpr uint32 sm_uWindowWidth   = 384;
  static constexpr uint32 sm_uWindowHeight  = 682u;

  static constexpr uint32 sm_uRenderFactor = 2u;

  static constexpr uint32 sm_uVirtualWindowWidth  = sm_uWindowWidth   * sm_uRenderFactor;
  static constexpr uint32 sm_uVirtualWindowHeight = sm_uWindowHeight  * sm_uRenderFactor;
  
  static inline CVector2D sm_v2PaddleSize = { 16.0f, 64.0f };
  static inline CVector2D sm_v2BallSize = { 32.0f, 32.0f };

  static CGame& GetInstance()
  {
    static CGame oInstance;
    return oInstance;
  }

  CGame(const CGame&) = delete;
  void operator=(const CGame&) = delete;

  void Init();
  void Update(float _fDt);
  void Draw();
  const std::vector<CBall>& GetBalls() const { return m_vctBalls; }
  bool WantClose() const { return m_bWantClose; }

private:
  CGame();
  ~CGame();

  RenderTexture2D m_oRenderTarget;
  std::vector<CBlock> m_vctBlocks;
  std::vector<CBall> m_vctBalls;
  CPaddle m_oPaddle;
  bool m_bWantClose = false;
};