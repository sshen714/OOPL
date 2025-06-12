#include "Spring.hpp"
#include "Util/Time.hpp"   // 依你的框架調整路徑

Spring::Spring(float boostStrength, float frameIntervalMs)
    : Character(SPRING_DIR "/spring3.png"),
      m_FrameIntervalMs(frameIntervalMs),
      m_BoostStrength(boostStrength)
{
    LoadAnimationFrames();
    SetZIndex(10);   // 讓彈簧圖層優先
}

void Spring::LoadAnimationFrames()
{
    m_Frames.reserve(3);
    m_Frames.emplace_back(SPRING_DIR "/spring3.png"); // 壓縮
    m_Frames.emplace_back(SPRING_DIR "/spring2.png"); // 中間
    m_Frames.emplace_back(SPRING_DIR "/spring1.png"); // 彈起
}

void Spring::AdvanceAnimationFrame()
{
    ++m_FrameIndex;
    if (m_FrameIndex < m_Frames.size())
    {
        SetImage(m_Frames[m_FrameIndex]);
    }
    else   // 動畫結束，回 Idle
    {
        m_IsSquishing = false;
        m_FrameIndex  = 0;
        SetImage(m_Frames[0]);
    }
}

void Spring::Update(bool isPlayerOnSpring)
{
    // 當玩家首次踩到彈簧 → 進入 Squishing 狀態
    if (isPlayerOnSpring && !m_IsSquishing)
    {
        m_IsSquishing = true;
        m_FrameIndex  = 0;
        m_Timer       = 0.0f;
        SetImage(m_Frames[m_FrameIndex]);
    }

    if (!m_IsSquishing) return;

    m_Timer += Util::Time::GetDeltaTimeMs();
    if (m_Timer >= m_FrameIntervalMs)
    {
        m_Timer -= m_FrameIntervalMs;  // 保留多餘的時間，避免 dt 不穩
        AdvanceAnimationFrame();
    }
}

float Spring::GetBoostStrength() const
{
    return m_BoostStrength;
}
