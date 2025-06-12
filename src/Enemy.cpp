#include "Enemy.hpp"
#include "config.hpp"

#define ENEMY_DIR GA_RESOURCE_DIR "/Image/Character"

Enemy::Enemy()
    : Character(ENEMY_DIR "/walk_1.png") {
    LoadAnimationFrames();
    m_LeftBound = GetPosition().x - 100;
    m_RightBound = GetPosition().x + 100;
}

void Enemy::LoadAnimationFrames() {
    for (int i = 1; i <= 2; ++i) {
        m_WalkFrames.push_back(ENEMY_DIR "/walk_" + std::to_string(i) + ".png");
        m_DieFrames.push_back(ENEMY_DIR "/die_" + std::to_string(i) + ".png");
    }
}

void Enemy::SetPatrolRange(float left, float right) {
    m_LeftBound = left;
    m_RightBound = right;
}

void Enemy::SetGroundY(float y) {
    m_GroundY = y;
}

void Enemy::Jump(float strength) {
    if (!m_IsJumping) {
        m_IsJumping = true;
        m_VelocityY = strength;
    }
}

void Enemy::Update() {
    if (m_State == State::Die) {
        if (m_FrameTimer++ >= m_FrameInterval) {
            m_FrameTimer = 0;
            if (m_CurrentFrame < m_DieFrames.size()) {
                SetImage(m_DieFrames[m_CurrentFrame++]);
            } else {
                m_IsDeadAndGone = true;
            }
        }
        return;
    }


    // 跳躍邏輯
    if (m_IsJumping) {
        m_VelocityY += m_Gravity;
        auto pos = GetPosition();
        pos.y += m_VelocityY;

        if (pos.y <= m_GroundY) {
            pos.y = m_GroundY;
            m_IsJumping = false;
            m_VelocityY = 0.0f;
        }
        SetPosition(pos);
    }

    // 移動邏輯
    auto pos = GetPosition();
    pos.x += m_MovingRight ? m_Speed : -m_Speed;

    if (pos.x < m_LeftBound) {
        pos.x = m_LeftBound;
        m_MovingRight = true;
        SetScale({1.5f, 1.5f});
    } else if (pos.x > m_RightBound) {
        pos.x = m_RightBound;
        m_MovingRight = false;
        SetScale({-1.5f, 1.5f});
    }

    SetPosition(pos);
    UpdateAnimation();
}

void Enemy::UpdateAnimation() {
    m_FrameTimer++;
    if (m_FrameTimer >= m_FrameInterval) {
        m_FrameTimer = 0;
        m_CurrentFrame = (m_CurrentFrame + 1) % 2;

        switch (m_State) {
            case State::Walk:
                SetImage(m_WalkFrames[m_CurrentFrame]);
                break;
            case State::Die:
                SetImage(m_DieFrames[m_CurrentFrame]);
                break;
        }
    }
}

void Enemy::Kill() {
    if (m_State == State::Die) return;
    m_State = State::Die;
    m_CurrentFrame = 0;
    m_FrameTimer = 0;
}

bool Enemy::IsDead() const {
    return m_State == State::Die;
}

bool Enemy::IsGone() const {
    return m_IsDeadAndGone;
}
