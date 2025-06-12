#ifndef APP_HPP
#define APP_HPP

#include "pch.hpp"
#include "Phase.hpp"
#include "Enemy.hpp"
#include "BlockSpawner.hpp"
#include "Spring.hpp"
#include "DamageHandler.hpp"
#include "Util/Renderer.hpp"
#include "Character.hpp"
#include "Util/Text.hpp"
#include "PhaseResourceManger.hpp"

class Spike;
class BackgroundImage;

class App {
public:
    enum class State {
        START,
        UPDATE,
        END,
    };

    State GetCurrentState() const { return m_CurrentState; }
    void Start();
    void Update();
    void End();

    void UpdatePlatforms();
    void EnemyUpdate();

    void LoadStage(Stage stage);
    void SetupCommonUI();

private:
    void InitStage1();
    void InitStage2();
    void InitStage3();
    void InitStage4();
    void InitStage5();
    void InitStage6();
    void InitStage7();
    void InitStage8();
    void InitStage9();
    void InitStage10();
    void InitStage11();
    void InitStage12();
    void InitStage13();
    void InitStage14();
    void InitStage15();
    void InitStage16();

    // 狀態
    State m_CurrentState = State::START;
    FlowPhase m_FlowPhase = FlowPhase::NEW_STAGE;
    Stage m_CurrentStage = Stage::STAGE_0;

    // 遊戲物件
    bool m_IsJumping = false;
    float m_VelocityY = 0.0f;
    float m_JumpCooldown = 0.0f;           // 跳躍冷卻計時器（秒）
    const float m_JumpCooldownTime = 0.8f; // 冷卻秒數：秒
    const float m_Gravity = -0.5f;
    const float m_JumpStrength = 9.0f;
    const float m_GroundY = -400.0f;
    int m_HurtFrameCounter = 0;
    bool m_IsHurting = false;
    bool m_IsWall = false;
    int m_TargetBlockCount = 0;       // 目標總數
    int m_CollectedBlockCount = 0;

    std::shared_ptr<DamageHandler> m_DamageHandler;

    glm::vec2 m_RespawnPoint = {-500.0f, -250.0f};  // 設定角色統一的重生點


    void HandleColorSwitching();
    void UpdatePlayerImage(bool isJumping, bool isMoving);
    void SetBlockGoal(int count);


    std::shared_ptr<Util::GameObject> m_StartLabel;
    void SetupStartScreen();

    std::shared_ptr<Util::GameObject> m_StartHintLabel;
    std::shared_ptr<Character> m_player;
    std::shared_ptr<Character> m_door;
    std::vector<std::shared_ptr<Character>> m_platforms;
    std::vector<std::shared_ptr<Character>> m_movingPlatforms;

    std::vector<std::shared_ptr<Character>> m_colorBlocks;
    std::vector<std::shared_ptr<BlockSpawner>> m_BlockSpawners;

    std::vector<std::shared_ptr<Enemy>> m_enemies; //敵人

    std::shared_ptr<BackgroundImage> m_Background;
    std::shared_ptr<TaskText> m_TaskText;

    std::vector<std::shared_ptr<Util::GameObject>> m_GameObjects;
    std::vector<std::shared_ptr<Spike>> m_Spikes;
    std::vector<std::shared_ptr<Spring>> m_springs;


    std::shared_ptr<PhaseResourceManger> m_PRM;
    Util::Renderer m_Root;

    float m_SwitchDelayTimer = 0.0f;
    bool m_SwitchTriggered = false;
    bool m_EnterDown = false;
};

#endif // APP_HPP