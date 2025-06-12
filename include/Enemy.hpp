#pragma once
#include "Character.hpp"
#include <vector>
#include <string>

class Enemy : public Character {
public:
    Enemy();

    void Update() override;

    void SetPatrolRange(float left, float right);
    void SetGroundY(float y);     // 設定怪物落地的 Y 值
    void Jump(float strength);    // 讓怪物跳起來

    void Kill();
    bool IsDead() const;
    bool IsGone() const;

private:
    void LoadAnimationFrames();
    void UpdateAnimation();

    float m_Speed = 1.5f;
    float m_LeftBound = 0.0f;
    float m_RightBound = 0.0f;
    bool m_MovingRight = true;
    bool m_IsDeadAndGone = false;

    enum class State { Walk, Die } m_State = State::Walk;

    std::vector<std::string> m_WalkFrames;
    std::vector<std::string> m_DieFrames;

    int m_CurrentFrame = 0;
    int m_FrameTimer = 0;
    const int m_FrameInterval = 6;

    // 跳躍相關
    float m_GroundY = -390.0f;
    float m_VelocityY = 0.0f;
    bool m_IsJumping = false;
    float m_Gravity = -0.45f;
};
