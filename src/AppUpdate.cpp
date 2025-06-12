#include "App.hpp"
#include "AppUtil.hpp"
#include "Phase.hpp"
#include "Platform.hpp"
#include"ColorBlock.hpp"
#include "DamageHandler.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include "Spring.hpp"

void App::EnemyUpdate() {
    for (auto it = m_enemies.begin(); it != m_enemies.end(); ) {
        auto& enemy = *it;
        enemy->Update();

        if (enemy->IsGone()) {
            m_Root.RemoveChild(enemy);
            it = m_enemies.erase(it);
            continue;
        }

        if (!enemy->IsDead() && CheckCollision(m_player, enemy)) {
            if (IsStandingOnPlatform(m_player, enemy, m_VelocityY)) {
                enemy->Kill();
                m_VelocityY = 10.0f;  // 彈跳
                ++it;
                continue;
            }

            if (!m_DamageHandler->IsHurting()) {
                m_DamageHandler->TriggerHurt();
                LoadStage(m_CurrentStage);
            }
        }
        ++it;
    }
}

void App::UpdatePlatforms() {

    for (auto& platform : m_platforms) {
        if (auto moving = std::dynamic_pointer_cast<MovingPlatform>(platform)) {
            moving->Update(m_player, m_VelocityY); //  傳入角色與速度
        }
    }
}

void App::Update() {
    EnemyUpdate();
    UpdatePlatforms();
    HandleColorSwitching(); // 處理顏色切換

    if (m_player->GetTransform().translation.y < -390) {
        m_DamageHandler->TriggerHurt();
        LoadStage(m_CurrentStage);
    }

    m_player->UpdateCooldown();

    // 尖刺傷害
    for (const auto& spike : m_GameObjects) {
        if (CheckCollision(m_player, spike)) {
            m_DamageHandler->TriggerHurt();
            LoadStage(m_CurrentStage);
            break;
        }
    }

    // 處理受傷動畫階段（受傷中就直接 return）
    if (m_DamageHandler->UpdateHurtAnimation(m_Root)) { return; }

    // 處理跳躍邏輯
    // 減少冷卻時間
    m_JumpCooldown -= Util::Time::GetDeltaTimeMs() / 1000.0f;
    if (m_JumpCooldown < 0.0f) m_JumpCooldown = 0.0f;

    // 處理跳躍邏輯（加上冷卻）
    if (!m_IsJumping && Util::Input::IsKeyPressed(Util::Keycode::UP) && m_JumpCooldown <= 0.0f) {
        m_IsJumping = true;
        m_VelocityY = m_JumpStrength;
        m_JumpCooldown = m_JumpCooldownTime;  // 重設冷卻

        for (auto& enemy : m_enemies) {
            enemy->Jump(m_JumpStrength);
        }
    }


    if (m_IsJumping) {
        auto pos = m_player->GetPosition();
        m_VelocityY += m_Gravity;
        float newY = pos.y + m_VelocityY + 1;

        if (newY <= m_GroundY) {
            newY = m_GroundY;
            m_IsJumping = false;
            m_VelocityY = 0.0f;
        }

        m_player->SetPosition({pos.x, newY});
    }


    for (const auto& spring : m_springs)
    {
        bool onSpring = IsStandingOnPlatform(m_player, spring, m_VelocityY);
        if (onSpring && !spring->IsSquishing())
        {
            m_player->SetVelocityY(-spring->GetBoostStrength());
            m_VelocityY = 13.0f;
        }
        spring->Update(onSpring);
    }

    // 平台踩踏與碰撞
    bool isOnPlatformNow = false;

    for (auto it = m_platforms.begin(); it != m_platforms.end(); ) {
        auto& platform = *it;

        if (auto colored = std::dynamic_pointer_cast<ColoredPlatform>(platform)) {
            bool isSameColor = colored->GetColor() == m_player->GetColor();

            bool touchTop = IsStandingOnPlatform(m_player, platform, m_VelocityY);
            bool touchLeft = IsHittingPlatformRight(m_player, platform);
            bool touchRight = IsHittingPlatformLeft(m_player, platform);
            bool touchBottom = IsHittingPlatformCeiling(m_player, platform, m_VelocityY);

            if (isSameColor && (touchTop || touchLeft || touchRight || touchBottom)) {
                m_CollectedBlockCount++;
                if (m_TaskText) {
                    m_TaskText->UpdateText(m_CollectedBlockCount, m_TargetBlockCount);
                }
                LOG_INFO("已吃方塊：{}/{}", m_CollectedBlockCount, m_TargetBlockCount);
                m_Root.RemoveChild(platform);
                it = m_platforms.erase(it);
                continue;
            }
        }

        //  如果踩在平台上（不管是不是彩色）
        if (IsStandingOnPlatform(m_player, platform, m_VelocityY)) {
            float platformTop = platform->GetPosition().y + platform->GetSize().y / 2.0f;
            float playerHalfHeight = m_player->GetSize().y / 2.0f;
            m_player->SetPosition({m_player->GetPosition().x, platformTop + playerHalfHeight});
            m_IsJumping = false;
            m_VelocityY = 0.0f;
            isOnPlatformNow = true;
        }

        ++it;
    }

    // 頭撞平台
    for (auto& platform : m_platforms) {
        if (auto colored = std::dynamic_pointer_cast<ColoredPlatform>(platform)) {
            if (colored->GetColor() == m_player->GetColor()) continue; // 同色消失，無需判斷
        }

        if (IsHittingPlatformCeiling(m_player, platform, m_VelocityY)) {
            m_VelocityY = 0.0f;
            break;
        }
    }

    if (!isOnPlatformNow && m_player->GetPosition().y > m_GroundY + 1.0f) {
        m_IsJumping = true;
    }

    // 平台左右碰撞限制
    float Speed = 3.0f;
    if (Util::Input::IsKeyPressed(Util::Keycode::LSHIFT)) {
        Speed = 7.5f;
    }

    auto pos = m_player->GetPosition();

    for (const auto& platform : m_platforms) {
        if (Util::Input::IsKeyPressed(Util::Keycode::LEFT)) {
            if (IsHittingPlatformRight(m_player, platform)) {
                Speed = 0.0f;
            }
        } else if (Util::Input::IsKeyPressed(Util::Keycode::RIGHT)) {
            if (IsHittingPlatformLeft(m_player, platform)) {
                Speed = 0.0f;
            }
        }
    }

    // 角色移動
    if (Util::Input::IsKeyPressed(Util::Keycode::LEFT) && pos.x > -531) {
        m_player->SetPosition({pos.x - Speed, pos.y});
        m_player->SetScale({-1.0f, 1.0f});
    }

    if (Util::Input::IsKeyPressed(Util::Keycode::RIGHT) && pos.x < 531) {
        m_player->SetPosition({pos.x + Speed, pos.y});
        m_player->SetScale({1.0f, 1.0f});
    }

    //   移動平台更新（推動玩家）
    for (auto& platform : m_platforms) {
        if (auto moving = std::dynamic_pointer_cast<MovingColoredPlatform>(platform)) {
            moving->Update(m_player, m_player->GetColor(), m_VelocityY);
        } else if (auto movingPlain = std::dynamic_pointer_cast<MovingPlatform>(platform)) {
            movingPlain->Update(m_player, m_VelocityY);
        }
    }

    UpdatePlayerImage(m_IsJumping,
                      Util::Input::IsKeyPressed(Util::Keycode::LEFT) || Util::Input::IsKeyPressed(Util::Keycode::RIGHT));

    if (m_door && !m_SwitchTriggered && m_FlowPhase == FlowPhase::NEW_STAGE) {
        if (m_CollectedBlockCount >= m_TargetBlockCount || Util::Input::IsKeyPressed(Util::Keycode::SPACE)) {  //   要吃夠才能開門
            if (CheckCollision(m_player, m_door) || Util::Input::IsKeyPressed(Util::Keycode::SPACE)) {
                m_door->SetImage(GA_RESOURCE_DIR "/Image/Character/door_open.png");
                LOG_INFO(" 碰到門，準備進入下一關...");
                m_FlowPhase = FlowPhase::NEXT_PHASE;
                m_SwitchDelayTimer = 0.0f;
                m_SwitchTriggered = true;
            }
        }
    }

    for (auto& spawner : m_BlockSpawners) {
        spawner->Update(m_platforms, m_Root, m_player, m_VelocityY);
    }

    if (m_FlowPhase == FlowPhase::NEXT_PHASE  &&
    m_CurrentStage != Stage::STAGE_16) {
        m_SwitchDelayTimer += 1.0f;

        if (m_SwitchDelayTimer >= 30.0f) {
            Stage next = m_PRM->NextStage();
            if (next != Stage::STAGE_16) {
                next = static_cast<Stage>(static_cast<int>(next) + 1);
            }
            LOG_INFO(" 進入下一關 Stage: {}", static_cast<int>(next));
            m_CurrentStage = next;
            LoadStage(next);
            m_FlowPhase = FlowPhase::NEW_STAGE;
            m_SwitchTriggered = false;
        }
    }

    if (Util::Input::IsKeyPressed(Util::Keycode::Q) &&
        m_CurrentState == State::UPDATE ) {
        m_CurrentState = State::END;
    }

    if (Util::Input::IsKeyPressed(Util::Keycode::R)) {
        m_DamageHandler->TriggerHurt();
        LoadStage(m_CurrentStage);
    }

    m_Root.Update();
}

