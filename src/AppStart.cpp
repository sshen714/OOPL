#include "App.hpp"
#include "Phase.hpp"
#include "Spike.hpp"
#include "Platform.hpp"
#include "ColorBlock.hpp"
#include "TaskText.hpp"
#include "BlockSpawner.hpp"
#include "Enemy.hpp"
#include "Util/Logger.hpp"
#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Color.hpp"

void App::SetupStartScreen() {
    m_StartLabel = std::make_shared<Util::GameObject>(
        std::make_shared<Util::Image>(GA_RESOURCE_DIR"/Image/Background/GameStartScreen.png"), 100.0f);
    m_StartLabel->m_Transform.translation = {0.0f, 0.0f};
    m_StartLabel->m_Transform.scale = {0.9f, 0.75f};
    m_Root.AddChild(m_StartLabel);
}

void App::Start() {
    static bool initialized = false;

    if (!initialized) {
        LOG_TRACE("Start 畫面初始化");
        SetupStartScreen();
        initialized = true;
    }

    if (Util::Input::IsKeyPressed(Util::Keycode::RETURN)) {
        LOG_INFO(" 玩家按下 Enter，開始遊戲！");
        m_CurrentState = State::UPDATE;
        m_CurrentStage = Stage::STAGE_1;
        LoadStage(m_CurrentStage);
        initialized = false;
    }
    m_Root.Update();
}

void App::LoadStage(Stage stage) {
    if (!m_PRM) {
        m_PRM = std::make_shared<PhaseResourceManger>();
    }

    m_GameObjects.clear();
    m_Root = Util::Renderer();
    m_platforms.clear();
    m_enemies.clear();
    m_BlockSpawners.clear();
    m_springs.clear();

    if (m_Background) {
        m_Background->NextPhase(static_cast<int>(stage));
        m_Root.AddChild(m_Background);
    }

    m_TaskText = std::make_shared<TaskText>(GA_RESOURCE_DIR"/Font/Inkfree.ttf", 20, glm::vec2{-400.0f, 300.0f}, Util::Color::FromRGB(0, 0, 0));
    m_Root.AddChild(m_TaskText);

    SetupCommonUI();

    switch (stage) {
        case Stage::STAGE_1: InitStage1(); break;
        case Stage::STAGE_2: InitStage2(); break;
        case Stage::STAGE_3: InitStage3(); break;
        case Stage::STAGE_4: InitStage4(); break;
        case Stage::STAGE_5: InitStage5(); break;
        case Stage::STAGE_6: InitStage6(); break;
        case Stage::STAGE_7: InitStage7(); break;
        case Stage::STAGE_8: InitStage8(); break;
        case Stage::STAGE_9: InitStage9(); break;
        case Stage::STAGE_10: InitStage10(); break;
        case Stage::STAGE_11: InitStage11(); break;
        case Stage::STAGE_12: InitStage12(); break;
        case Stage::STAGE_13: InitStage13(); break;
        case Stage::STAGE_14: InitStage14(); break;
        case Stage::STAGE_15: InitStage15(); break;
        case Stage::STAGE_16: InitStage16(); break;
        default: break;
    }

    if (m_player) {
        m_DamageHandler = std::make_shared<DamageHandler>(m_player, m_player->GetPosition());
    }
}

void App::SetupCommonUI() {
    // 玩家角色初始化
    if (!m_player) {
        m_player = std::make_shared<Character>(GA_RESOURCE_DIR "/Image/Character/player/Green_idle.png");
        m_player->SetZIndex(50);
    }
    m_player->SetPosition({-500.0f, -240.0f});
    m_Root.AddChild(m_player);

    // 目標任務文字初始化
    if (!m_TaskText) {
        m_TaskText = std::make_shared<TaskText>();
        m_TaskText->SetZIndex(100);
        m_Root.AddChild(m_TaskText);
    }

}

void App::InitStage1() {

    AddPlatformRow(m_platforms, m_Root, GA_RESOURCE_DIR"/Image/Character/platform1.png", -531.0f, -278.0f, 60);


    float startX = 9.5f;
    float startY = -260.0f;
    int startCount = 30;

    for (int i = 0; i < 9; ++i) {
        AddPlatformRow(
            m_platforms,
            m_Root,
            GA_RESOURCE_DIR"/Image/Character/platform1.png",
            startX + i * 36.0f,
            startY + i * 18.0f,
            startCount - i * 2
        );
    }

    m_door = std::make_shared<Character>(GA_RESOURCE_DIR"/Image/Character/door_close.png");
    m_door->SetPosition({500.0f, -88.0f});
    m_door->SetScale({0.3f, 0.3f});
    m_door->SetZIndex(20);
    m_Root.AddChild(m_door);

    if (m_PRM) {
        m_Root.AddChildren(m_PRM->GetChildren());
    }
}

void App::InitStage2() {
    SetBlockGoal(6);
    m_player->SetImage(GA_RESOURCE_DIR "/Image/Character/player/Green_Idle.png");

    AddPlatformRow(m_platforms, m_Root, GA_RESOURCE_DIR"/Image/Character/platform1.png", -531.0f, -278.0f, 60);

    AddColoredBlockColumn(m_platforms , m_Root,GA_RESOURCE_DIR "/Image/Character/Color_block/blue_block.png",-189.0f, -260.0f, 7, ColorState::BLUE );
    AddColoredBlockColumn(m_platforms , m_Root,GA_RESOURCE_DIR "/Image/Character/Color_block/blue_block.png",-171.0f, -260.0f, 7, ColorState::BLUE );
    AddColoredBlockColumn(m_platforms , m_Root,GA_RESOURCE_DIR "/Image/Character/Color_block/red_block.png",-115.0f, -260.0f, 7, ColorState::RED );
    AddColoredBlockColumn(m_platforms , m_Root,GA_RESOURCE_DIR "/Image/Character/Color_block/red_block.png",-97.0f, -260.0f, 7, ColorState::RED );
    AddColoredBlockColumn(m_platforms , m_Root,GA_RESOURCE_DIR "/Image/Character/Color_block/green_block.png",-41.0f, -260.0f, 7, ColorState::GREEN );
    AddColoredBlockColumn(m_platforms , m_Root,GA_RESOURCE_DIR "/Image/Character/Color_block/green_block.png",-23.0f, -260.0f, 7, ColorState::GREEN );

    m_door = std::make_shared<Character>(GA_RESOURCE_DIR"/Image/Character/door_close.png");
    m_door->SetPosition({450.0f, -250.0f});
    m_door->m_Transform.scale = {0.3f, 0.3f};
    m_door->SetZIndex(20);
    m_Root.AddChild(m_door);

    if (m_PRM) {
        m_Root.AddChildren(m_PRM->GetChildren());
    }
}

void App::InitStage3() {
    SetBlockGoal(10);
    AddPlatformRow(m_platforms, m_Root, GA_RESOURCE_DIR"/Image/Character/platform1.png", -531.0f, -278.0f, 60);

    float startX = 9.5f;
    float startY = -260.0f;
    int startCount = 30;

    for (int i = 0; i < 9; ++i) {
        AddPlatformRow(m_platforms,m_Root,GA_RESOURCE_DIR"/Image/Character/platform1.png",
            startX + i * 36.0f,
            startY + i * 18.0f,
            startCount - i * 2
        );
    }

    AddColoredBlockRow( m_platforms, m_Root ,  GA_RESOURCE_DIR"/Image/Character/Color_block/blue_block.png",9.5f,-242,2, ColorState::BLUE);
    AddColoredBlockRow( m_platforms, m_Root ,  GA_RESOURCE_DIR"/Image/Character/Color_block/blue_block.png",45.5f,-224,2, ColorState::BLUE);
    AddColoredBlockRow( m_platforms, m_Root ,  GA_RESOURCE_DIR"/Image/Character/Color_block/blue_block.png",81.5f,-206,2, ColorState::BLUE);
    AddColoredBlockRow( m_platforms, m_Root ,  GA_RESOURCE_DIR"/Image/Character/Color_block/red_block.png",117.5f,-188,2, ColorState::RED);
    AddColoredBlockRow( m_platforms, m_Root ,  GA_RESOURCE_DIR"/Image/Character/Color_block/red_block.png",153.5f,-170,2, ColorState::RED);
    AddColoredBlockRow( m_platforms, m_Root ,  GA_RESOURCE_DIR"/Image/Character/Color_block/red_block.png",189.5f,-152,2, ColorState::RED);
    AddColoredBlockRow( m_platforms, m_Root ,  GA_RESOURCE_DIR"/Image/Character/Color_block/green_block.png",225.5f,-134,2, ColorState::GREEN);
    AddColoredBlockRow( m_platforms, m_Root ,  GA_RESOURCE_DIR"/Image/Character/Color_block/green_block.png",261.5f,-116,2, ColorState::GREEN);
    AddColoredBlockRow( m_platforms, m_Root ,  GA_RESOURCE_DIR"/Image/Character/Color_block/green_block.png",297.5f,-98,2, ColorState::GREEN);

    m_door = std::make_shared<Character>(GA_RESOURCE_DIR"/Image/Character/door_close.png");
    m_door->SetPosition({500.0f, -88.0f});
    m_door->SetScale({0.3f, 0.3f});
    m_Root.AddChild(m_door);

    if (m_PRM) {
        m_Root.AddChildren(m_PRM->GetChildren());
    }
}

void App::InitStage4() {
    SetBlockGoal(8);
    m_player->SetImage(GA_RESOURCE_DIR "/Image/Character/player/Green_Idle.png");
    auto spawner = std::make_shared<BlockSpawner>(glm::vec2{200, -200}, 1.0f, false, 100.0f);
    m_BlockSpawners.push_back(spawner);

    m_door = std::make_shared<Character>(GA_RESOURCE_DIR"/Image/Character/door_close.png");
    m_door->SetPosition({440.0f, -250.0f});
    m_door->m_Transform.scale = {0.3f, 0.3f};
    m_Root.AddChild(m_door);
    AddPlatformRow(m_platforms, m_Root, GA_RESOURCE_DIR"/Image/Character/platform1.png", -531.0f, -278.0f, 60);

    if (m_PRM) {
        m_Root.AddChildren(m_PRM->GetChildren());
    }

}

void App::InitStage5() {

    SetBlockGoal(8);
    auto spawner = std::make_shared<BlockSpawner>(glm::vec2{400, -200}, 1.0f, false, 150.0f);
    m_BlockSpawners.push_back(spawner);

    m_door = std::make_shared<Character>(GA_RESOURCE_DIR"/Image/Character/door_close.png");
    m_door->SetPosition({440.0f, -215.0f});
    m_door->m_Transform.scale = {0.3f, 0.3f};
    m_Root.AddChild(m_door);
    AddPlatformRow(m_platforms, m_Root, GA_RESOURCE_DIR"/Image/Character/platform1.png", -531.0f, -278.0f, 7);
    AddPlatformRow(m_platforms, m_Root, GA_RESOURCE_DIR"/Image/Character/platform2.png", -531.0f, -170.0f, 60);
    AddPlatformRow(m_platforms, m_Root, GA_RESOURCE_DIR"/Image/Character/platform1.png", -405.0f, -242.0f, 53);

    if (m_PRM) {
        m_Root.AddChildren(m_PRM->GetChildren());
    }
}

void App::InitStage6() {

    SetBlockGoal(6);
    m_player->SetPosition({-400.0f, -240.0f});
    AddPlatformRow(m_platforms, m_Root, GA_RESOURCE_DIR"/Image/Character/platform1.png", -531.0f, -278.0f, 60);
    auto spawner = std::make_shared<BlockSpawner>(glm::vec2{400, -250}, 0.8f, false, 250.0f);
    m_BlockSpawners.push_back(spawner);


    m_door = std::make_shared<Character>(GA_RESOURCE_DIR"/Image/Character/door_close.png");
    m_door->SetPosition({500.0f, -250.0f});
    m_door->SetScale({0.3f, 0.3f});
    m_door->SetZIndex(20);
    m_Root.AddChild(m_door);

    for (int i = 0; i < 4; ++i) {
        float x = -500.0f + i * 18.0f;
        auto spike = std::make_shared<Spike>(glm::vec2{x, -260.0f});
        m_GameObjects.push_back(spike);
        m_Root.AddChild(spike);
    }

    if (m_PRM) {
        m_Root.AddChildren(m_PRM->GetChildren());
    }

}

void App::InitStage7() {

    SetBlockGoal(72);
    m_player->SetImage(GA_RESOURCE_DIR "/Image/Character/player/Green_Idle.png");

    AddPlatformRow(m_platforms, m_Root, GA_RESOURCE_DIR"/Image/Character/platform1.png", -531.0f, -278.0f, 60);

    std::vector<std::tuple<float, ColorState, std::string>> rows;
    float startY = -260.0f;
    float gap = 18.0f;

    // 每個顏色三行，一組總共 9 行，共兩組
    for (int group = 0; group < 2; ++group) {
        // 3 行 BLUE
        for (int i = 0; i < 3; ++i) {
            rows.emplace_back(startY + (rows.size() * gap), ColorState::BLUE, GA_RESOURCE_DIR "/Image/Character/Color_block/blue_block.png");
        }
        // 3 行 RED
        for (int i = 0; i < 3; ++i) {
            rows.emplace_back(startY + (rows.size() * gap), ColorState::RED, GA_RESOURCE_DIR "/Image/Character/Color_block/red_block.png");
        }
        // 3 行 GREEN
        for (int i = 0; i < 3; ++i) {
            rows.emplace_back(startY + (rows.size() * gap), ColorState::GREEN, GA_RESOURCE_DIR "/Image/Character/Color_block/green_block.png");
        }
    }
    for (const auto& [y, color, path] : rows) {
        AddColoredBlockRow(m_platforms, m_Root, path, -189.0f, y, 4, color);
    }


    m_door = std::make_shared<Character>(GA_RESOURCE_DIR"/Image/Character/door_close.png");
    m_door->SetPosition({450.0f, -250.0f});
    m_door->m_Transform.scale = {0.3f, 0.3f};
    m_door->SetZIndex(20);
    m_Root.AddChild(m_door);

    if (m_PRM) {
        m_Root.AddChildren(m_PRM->GetChildren());
    }

}

void App::InitStage8() {

    SetBlockGoal(3);
    m_player->SetImage(GA_RESOURCE_DIR "/Image/Character/player/Green_Idle.png");

    AddPlatformRow(m_platforms, m_Root, GA_RESOURCE_DIR"/Image/Character/platform1.png", -531.0f, -278.0f, 10);
    AddPlatformRow(m_platforms, m_Root, GA_RESOURCE_DIR"/Image/Character/platform1.png", -351.0f, -332.0f, 7);
    AddColoredBlockRow( m_platforms, m_Root ,  GA_RESOURCE_DIR"/Image/Character/Color_block/blue_block.png",-297.0f,-170,1, ColorState::BLUE);
    AddPlatformRow(m_platforms, m_Root, GA_RESOURCE_DIR"/Image/Character/platform1.png", -225.0f, -278.0f, 10);
    AddPlatformRow(m_platforms, m_Root, GA_RESOURCE_DIR"/Image/Character/platform1.png", -45.0f, -332.0f, 7);
    AddColoredBlockRow( m_platforms, m_Root ,  GA_RESOURCE_DIR"/Image/Character/Color_block/red_block.png",9.0f,-170,1, ColorState::RED);
    AddPlatformRow(m_platforms, m_Root, GA_RESOURCE_DIR"/Image/Character/platform1.png", 81.0f, -278.0f, 10);
    AddPlatformRow(m_platforms, m_Root, GA_RESOURCE_DIR"/Image/Character/platform1.png", 261.0f, -332.0f, 7);
    AddColoredBlockRow( m_platforms, m_Root ,  GA_RESOURCE_DIR"/Image/Character/Color_block/green_block.png",315.0f,-170,1, ColorState::GREEN);
    AddPlatformRow(m_platforms, m_Root, GA_RESOURCE_DIR"/Image/Character/platform1.png", 387.0f, -278.0f, 9);

    for (int i = 0; i < 7; ++i) {
        float x = -351.0f + i * 18.0f;
        auto spike = std::make_shared<Spike>(glm::vec2{x, -314.0f});
        m_GameObjects.push_back(spike);
        m_Root.AddChild(spike);
    }

    for (int i = 0; i < 7; ++i) {
        float x = -45.0f + i * 18.0f;
        auto spike = std::make_shared<Spike>(glm::vec2{x, -314.0f});
        m_GameObjects.push_back(spike);
        m_Root.AddChild(spike);
    }

    for (int i = 0; i < 7; ++i) {
        float x = 261.0f + i * 18.0f;
        auto spike = std::make_shared<Spike>(glm::vec2{x, -314.0f});
        m_GameObjects.push_back(spike);
        m_Root.AddChild(spike);
    }


    m_door = std::make_shared<Character>(GA_RESOURCE_DIR"/Image/Character/door_close.png");
    m_door->SetPosition({440.0f, -250.0f});
    m_door->m_Transform.scale = {0.3f, 0.3f};
    m_Root.AddChild(m_door);

    if (m_PRM) {
        m_Root.AddChildren(m_PRM->GetChildren());
    }
}

void App::InitStage9() {
    SetBlockGoal(5);
    m_player->SetImage(GA_RESOURCE_DIR "/Image/Character/player/Green_Idle.png");
    auto spawner = std::make_shared<BlockSpawner>(glm::vec2{500, -250}, 1.5f, false, 150.0f);
    m_BlockSpawners.push_back(spawner);

    AddPlatformRow(m_platforms, m_Root, GA_RESOURCE_DIR"/Image/Character/platform1.png", -531.0f, -278.0f, 10);
    AddPlatformRow(m_platforms, m_Root, GA_RESOURCE_DIR"/Image/Character/platform1.png", -351.0f, -332.0f, 7);
    AddColoredBlockRow( m_platforms, m_Root ,  GA_RESOURCE_DIR"/Image/Character/Color_block/blue_block.png",-297.0f,-170,1, ColorState::BLUE);
    AddPlatformRow(m_platforms, m_Root, GA_RESOURCE_DIR"/Image/Character/platform1.png", -225.0f, -278.0f, 10);
    AddPlatformRow(m_platforms, m_Root, GA_RESOURCE_DIR"/Image/Character/platform1.png", -45.0f, -332.0f, 7);
    AddColoredBlockRow( m_platforms, m_Root ,  GA_RESOURCE_DIR"/Image/Character/Color_block/red_block.png",9.0f,-170,1, ColorState::RED);
    AddPlatformRow(m_platforms, m_Root, GA_RESOURCE_DIR"/Image/Character/platform1.png", 81.0f, -278.0f, 10);
    AddPlatformRow(m_platforms, m_Root, GA_RESOURCE_DIR"/Image/Character/platform1.png", 261.0f, -332.0f, 7);
    AddColoredBlockRow( m_platforms, m_Root ,  GA_RESOURCE_DIR"/Image/Character/Color_block/green_block.png",315.0f,-170,1, ColorState::GREEN);
    AddPlatformRow(m_platforms, m_Root, GA_RESOURCE_DIR"/Image/Character/platform1.png", 387.0f, -278.0f, 9);

    for (int i = 0; i < 7; ++i) {
        float x = -351.0f + i * 18.0f;
        auto spike = std::make_shared<Spike>(glm::vec2{x, -314.0f});
        m_GameObjects.push_back(spike);
        m_Root.AddChild(spike);
    }

    for (int i = 0; i < 7; ++i) {
        float x = -45.0f + i * 18.0f;
        auto spike = std::make_shared<Spike>(glm::vec2{x, -314.0f});
        m_GameObjects.push_back(spike);
        m_Root.AddChild(spike);
    }

    for (int i = 0; i < 7; ++i) {
        float x = 261.0f + i * 18.0f;
        auto spike = std::make_shared<Spike>(glm::vec2{x, -314.0f});
        m_GameObjects.push_back(spike);
        m_Root.AddChild(spike);
    }

    m_door = std::make_shared<Character>(GA_RESOURCE_DIR"/Image/Character/door_close.png");
    m_door->SetPosition({440.0f, -250.0f});
    m_door->m_Transform.scale = {0.3f, 0.3f};
    m_Root.AddChild(m_door);

    if (m_PRM) {
        m_Root.AddChildren(m_PRM->GetChildren());
    }

}

void App::InitStage10() {

    SetBlockGoal(65);
    m_player->SetImage(GA_RESOURCE_DIR "/Image/Character/player/Green_Idle.png");
    m_player->SetPosition({-450.0f,-175.0f});


    AddPlatformRow(m_platforms, m_Root, GA_RESOURCE_DIR"/Image/Character/platform1.png", -531.0f, -206.0f, 10);
    AddPlatformRow(m_platforms, m_Root, GA_RESOURCE_DIR"/Image/Character/platform1.png", -351.0f, -242.0f, 13);
    AddPlatformColumn(m_platforms, m_Root, GA_RESOURCE_DIR"/Image/Character/platform2.png", -135.0f, -350.0f, 5);
    AddPlatformRow(m_platforms, m_Root, GA_RESOURCE_DIR"/Image/Character/platform1.png", -117.0f, -350.0f, 12);
    AddPlatformColumn(m_platforms, m_Root, GA_RESOURCE_DIR"/Image/Character/platform2.png", 99.0f, -350.0f, 5);
    AddPlatformRow(m_platforms, m_Root, GA_RESOURCE_DIR"/Image/Character/platform1.png", 99.0f, -242.0f, 25);


    float startX = -117.0f; // 起始 X 座標
    float startY = -332.0f; // 起始 Y 座標
    float gap = 18.0f;

    std::vector<std::tuple<ColorState, std::string>> colors = {
        {ColorState::GREEN, GA_RESOURCE_DIR "/Image/Character/Color_block/green_block.png"},
        {ColorState::RED,   GA_RESOURCE_DIR "/Image/Character/Color_block/red_block.png"},
        {ColorState::BLUE,  GA_RESOURCE_DIR "/Image/Character/Color_block/blue_block.png"},
    };

    for (int colorIndex = 0; colorIndex < colors.size(); ++colorIndex) {
        auto [color, path] = colors[colorIndex];
        float offsetX = startX + colorIndex * (4 * gap);  // 每個顏色區塊向右偏移 4 格寬度

        for (int col = 0; col < 4; ++col) {
            for (int row = 0; row < 6; ++row) {
                float x = offsetX + col * gap;
                float y = startY + row * gap;

                auto block = std::make_shared<ColoredPlatform>(path, color);
                block->SetPosition({x, y});
                block->SetZIndex(10);
                m_platforms.push_back(block);
                m_Root.AddChild(block);
            }
        }
    }


    m_door = std::make_shared<Character>(GA_RESOURCE_DIR"/Image/Character/door_close.png");
    m_door->SetPosition({440.0f, -215.0f});
    m_door->m_Transform.scale = {0.3f, 0.3f};
    m_Root.AddChild(m_door);

    if (m_PRM) {
        m_Root.AddChildren(m_PRM->GetChildren());
    }

}

void App::InitStage11() {

    SetBlockGoal(3);
    m_player->SetImage(GA_RESOURCE_DIR "/Image/Character/player/Green_Idle.png");
    m_player->SetPosition({-510.0f,160.0f});

    AddPlatformRow(m_platforms, m_Root, GA_RESOURCE_DIR"/Image/Character/platform2.png", -531.0f, 196.0f, 60);
    AddPlatformRow(m_platforms, m_Root, GA_RESOURCE_DIR"/Image/Character/platform1.png", -531.0f, 126.0f, 4);
    AddColoredBlockRow( m_platforms, m_Root ,  GA_RESOURCE_DIR"/Image/Character/Color_block/red_block.png",-477.0f,144.0f,1, ColorState::RED);
    AddPlatformRow(m_platforms, m_Root, GA_RESOURCE_DIR"/Image/Character/platform1.png", -459.0f, 36.0f, 12);
    AddPlatformRow(m_platforms, m_Root, GA_RESOURCE_DIR"/Image/Character/platform1.png", -243.0f, 126.0f, 8);
    AddColoredBlockRow( m_platforms, m_Root ,  GA_RESOURCE_DIR"/Image/Character/Color_block/green_block.png",-117.0f,144.0f,1, ColorState::GREEN);
    AddPlatformRow(m_platforms, m_Root, GA_RESOURCE_DIR"/Image/Character/platform1.png", -99.0f, 36.0f, 12);
    AddPlatformRow(m_platforms, m_Root, GA_RESOURCE_DIR"/Image/Character/platform1.png", 117.0f, 126.0f, 8);
    AddColoredBlockRow( m_platforms, m_Root ,  GA_RESOURCE_DIR"/Image/Character/Color_block/blue_block.png",243.0f,144.0f,1, ColorState::BLUE);
    AddPlatformRow(m_platforms, m_Root, GA_RESOURCE_DIR"/Image/Character/platform1.png", 261.0f, 36.0f, 12);
    AddPlatformRow(m_platforms, m_Root, GA_RESOURCE_DIR"/Image/Character/platform1.png", 477.0f, 126.0f, 4);

    AddColoredBlockRow( m_platforms, m_Root ,  GA_RESOURCE_DIR"/Image/Character/Color_block/red_block.png",-459.0f,126.0f,4, ColorState::RED);
    AddColoredBlockRow( m_platforms, m_Root ,  GA_RESOURCE_DIR"/Image/Character/Color_block/blue_block.png",-387.0f,126.0f,4, ColorState::BLUE);
    AddColoredBlockRow( m_platforms, m_Root ,  GA_RESOURCE_DIR"/Image/Character/Color_block/green_block.png",-315.0f,126.0f,4, ColorState::GREEN);

    AddColoredBlockRow( m_platforms, m_Root ,  GA_RESOURCE_DIR"/Image/Character/Color_block/blue_block.png",-99.0f,126.0f,4, ColorState::BLUE);
    AddColoredBlockRow( m_platforms, m_Root ,  GA_RESOURCE_DIR"/Image/Character/Color_block/red_block.png",-27.0f,126.0f,4, ColorState::RED);
    AddColoredBlockRow( m_platforms, m_Root ,  GA_RESOURCE_DIR"/Image/Character/Color_block/green_block.png",45.0f,126.0f,4, ColorState::GREEN);

    AddColoredBlockRow( m_platforms, m_Root ,  GA_RESOURCE_DIR"/Image/Character/Color_block/green_block.png",261.0f,126.0f,4, ColorState::GREEN);
    AddColoredBlockRow( m_platforms, m_Root ,  GA_RESOURCE_DIR"/Image/Character/Color_block/red_block.png",333.0f,126.0f,4, ColorState::RED);
    AddColoredBlockRow( m_platforms, m_Root ,  GA_RESOURCE_DIR"/Image/Character/Color_block/blue_block.png",405.0f,126.0f,4, ColorState::BLUE);

    for (int i = 0; i < 12; ++i) {
        float x_left = -459.0f + i * 18.0f;
        float x_mid  = -99.0f  + i * 18.0f;
        float x_right = 261.0f + i * 18.0f;

        auto spikeL = std::make_shared<Spike>(glm::vec2{x_left, 54.0f});
        auto spikeM = std::make_shared<Spike>(glm::vec2{x_mid,  54.0f});
        auto spikeR = std::make_shared<Spike>(glm::vec2{x_right,54.0f});

        m_GameObjects.push_back(spikeL);
        m_Root.AddChild(spikeL);
        m_GameObjects.push_back(spikeM);
        m_Root.AddChild(spikeM);
        m_GameObjects.push_back(spikeR);
        m_Root.AddChild(spikeR);
    }

    m_door = std::make_shared<Character>(GA_RESOURCE_DIR"/Image/Character/door_close.png");
    m_door->SetPosition({510.0f, 155.0f});
    m_door->m_Transform.scale = {0.3f, 0.3f};
    m_Root.AddChild(m_door);

    if (m_PRM) {
        m_Root.AddChildren(m_PRM->GetChildren());
    }
}

void App::InitStage12() {

SetBlockGoal(7);

    AddPlatformRow(m_platforms, m_Root, GA_RESOURCE_DIR"/Image/Character/platform1.png", -531.0f, -278.0f, 13);
    AddPlatformRow(m_platforms, m_Root, GA_RESOURCE_DIR"/Image/Character/platform1.png", 369.0f, -278.0f, 10);
    AddPlatformRow(m_platforms, m_Root, GA_RESOURCE_DIR"/Image/Character/platform1.png", -27.0f, -296.0f, 7);
    AddColoredBlockRow( m_platforms, m_Root ,  GA_RESOURCE_DIR"/Image/Character/Color_block/green_block.png",-27.0f,-168.0f,7, ColorState::GREEN);
    AddPlatformColumn(m_platforms, m_Root, GA_RESOURCE_DIR"/Image/Character/platform2.png", -45.0f, -278.0f, 5);
    AddPlatformColumn(m_platforms, m_Root, GA_RESOURCE_DIR"/Image/Character/platform2.png", 99.0f, -278.0f, 5);

    float startX = -297.0f;
    float startY = -260.0f;

    for (int i = 0; i < 5; ++i) {
        AddPlatformRow(
            m_platforms,
            m_Root,
            GA_RESOURCE_DIR"/Image/Character/platform1.png",
            startX + i * 54.0f,
            startY + i * 18.0f,
            3
        );
    }

    float x = 99.0f;
    float y = -186.0f;

    for (int i = 0; i < 5; ++i) {
        AddPlatformRow(
            m_platforms,
            m_Root,
            GA_RESOURCE_DIR"/Image/Character/platform1.png",
            x + i * 54.0f,
            y - i * 18.0f,
            3
        );
    }

    auto enemy = std::make_shared<Enemy>();
    enemy->SetPosition({0, -278.0});
    enemy->SetPatrolRange(-9.0f, 81);
    enemy->m_Transform.scale = {1.5f, 1.5f};
    enemy->SetGroundY(-278.0);
    m_enemies.push_back(enemy);
    m_Root.AddChild(enemy);

    m_door = std::make_shared<Character>(GA_RESOURCE_DIR"/Image/Character/door_close.png");
    m_door->SetPosition({450.0f, -250.0f});
    m_door->m_Transform.scale = {0.3f, 0.3f};
    m_door->SetZIndex(20);
    m_Root.AddChild(m_door);

    if (m_PRM) {
        m_Root.AddChildren(m_PRM->GetChildren());
    }

}

void App::InitStage13() {
    SetBlockGoal(9);
    m_player->SetPosition({-480.0f,-168.0f});
    m_player->SetImage(GA_RESOURCE_DIR "/Image/Character/player/Green_Idle.png");

    AddPlatformRow(m_platforms, m_Root, GA_RESOURCE_DIR"/Image/Character/platform1.png", -531.0f, -206.0f, 6);
    AddPlatformRow(m_platforms, m_Root, GA_RESOURCE_DIR"/Image/Character/platform1.png", -423.0f, -304.0f, 10);
    AddPlatformRow(m_platforms, m_Root, GA_RESOURCE_DIR"/Image/Character/platform1.png", -243.0f, -206.0f, 3);
    AddColoredBlockRow( m_platforms, m_Root ,  GA_RESOURCE_DIR"/Image/Character/Color_block/green_block.png",-243.0f,-188.0f,3, ColorState::GREEN);
    AddPlatformRow(m_platforms, m_Root, GA_RESOURCE_DIR"/Image/Character/platform1.png", -189.0f, -304.0f, 10);
    AddPlatformRow(m_platforms, m_Root, GA_RESOURCE_DIR"/Image/Character/platform1.png", -9.0f, -206.0f, 3);
    AddColoredBlockRow( m_platforms, m_Root ,  GA_RESOURCE_DIR"/Image/Character/Color_block/red_block.png",-9.0f,-188.0f,3, ColorState::RED);
    AddPlatformRow(m_platforms, m_Root, GA_RESOURCE_DIR"/Image/Character/platform1.png", 45.0f, -304.0f, 10);
    AddPlatformRow(m_platforms, m_Root, GA_RESOURCE_DIR"/Image/Character/platform1.png", 225.0f, -206.0f, 3);
    AddColoredBlockRow( m_platforms, m_Root ,  GA_RESOURCE_DIR"/Image/Character/Color_block/blue_block.png",225.0f,-188.0f,3, ColorState::BLUE);
    AddPlatformRow(m_platforms, m_Root, GA_RESOURCE_DIR"/Image/Character/platform1.png", 279.0f, -304.0f, 10);
    AddPlatformRow(m_platforms, m_Root, GA_RESOURCE_DIR"/Image/Character/platform1.png", 459.0f, -206.0f, 5);


    for (int i = 0; i < 10; ++i) {
        float x = -423.0f + i * 18.0f;
        auto spike = std::make_shared<Spike>(glm::vec2{x, -285.0f});

        m_GameObjects.push_back(spike);
        m_Root.AddChild(spike);
    }

    for (int i = 0; i < 10; ++i) {
        float x = -189.0f + i * 18.0f;
        auto spike = std::make_shared<Spike>(glm::vec2{x, -285.0f});
        m_GameObjects.push_back(spike);
        m_Root.AddChild(spike);
    }

    for (int i = 0; i < 10; ++i) {
        float x = 45.0f + i * 18.0f;
        auto spike = std::make_shared<Spike>(glm::vec2{x, -285.0f});
        m_GameObjects.push_back(spike);
        m_Root.AddChild(spike);
    }

    for (int i = 0; i < 10; ++i) {
        float x = 279.0f + i * 18.0f;
        auto spike = std::make_shared<Spike>(glm::vec2{x, -285.0f});
        m_GameObjects.push_back(spike);
        m_Root.AddChild(spike);
    }

    auto enemy = std::make_shared<Enemy>();
    enemy->SetPosition({-423.0, -269.0});
    enemy->SetPatrolRange(-423.0, -261.0);
    enemy->m_Transform.scale = {1.5f, 1.5f};
    enemy->SetGroundY(-269.0);
    m_enemies.push_back(enemy);
    m_Root.AddChild(enemy);

    auto enemy2 = std::make_shared<Enemy>();
    enemy2->SetPosition({-189, -269.0});
    enemy2->SetPatrolRange(-189, -27);
    enemy2->m_Transform.scale = {1.5f, 1.5f};
    enemy2->SetGroundY(-269.0);
    m_enemies.push_back(enemy2);
    m_Root.AddChild(enemy2);

    auto enemy3 = std::make_shared<Enemy>();
    enemy3->SetPosition({45.0, -269.0});
    enemy3->SetPatrolRange(45.0, 207.0);
    enemy3->m_Transform.scale = {1.5f, 1.5f};
    enemy3->SetGroundY(-269.0);
    m_enemies.push_back(enemy3);
    m_Root.AddChild(enemy3);

    auto enemy4 = std::make_shared<Enemy>();
    enemy4->SetPosition({279, -269.0});
    enemy4->SetPatrolRange(279, 441);
    enemy4->m_Transform.scale = {1.5f, 1.5f};
    enemy4->SetGroundY(-269.0);
    m_enemies.push_back(enemy4);
    m_Root.AddChild(enemy4);

    m_door = std::make_shared<Character>(GA_RESOURCE_DIR"/Image/Character/door_close.png");
    m_door->SetPosition({490.0f, -178.0f});
    m_door->m_Transform.scale = {0.3f, 0.3f};
    m_Root.AddChild(m_door);

    if (m_PRM) {
        m_Root.AddChildren(m_PRM->GetChildren());
    }

}

void App::InitStage14() {
     SetBlockGoal(32);
    // m_player->SetPosition({-480.0f,-168.0f});
    m_player->SetImage(GA_RESOURCE_DIR "/Image/Character/player/Green_Idle.png");

    AddPlatformRow(m_platforms, m_Root, GA_RESOURCE_DIR"/Image/Character/platform1.png", -531.0f, -278.0f, 10);
    AddPlatformRow(m_platforms, m_Root, GA_RESOURCE_DIR"/Image/Character/platform1.png", -351.0f, -206, 2);
    AddPlatformRow(m_platforms, m_Root, GA_RESOURCE_DIR"/Image/Character/platform1.png", -315.0f, -152.0f, 2);
    AddPlatformRow(m_platforms, m_Root, GA_RESOURCE_DIR"/Image/Character/platform1.png", -279.0f, -98.0f, 2);
    AddPlatformRow(m_platforms, m_Root, GA_RESOURCE_DIR"/Image/Character/platform1.png", -243.0f, -278.0f, 8);
    AddColoredBlockRow( m_platforms, m_Root ,  GA_RESOURCE_DIR"/Image/Character/Color_block/green_block.png",-243.0f,-98.0f,8, ColorState::GREEN);
    AddColoredBlockRow( m_platforms, m_Root ,  GA_RESOURCE_DIR"/Image/Character/Color_block/red_block.png",-243.0f,-224.0f,8, ColorState::RED);

    AddPlatformRow(m_platforms, m_Root, GA_RESOURCE_DIR"/Image/Character/platform1.png", -99.0f, -98.0f, 9);
    AddPlatformRow(m_platforms, m_Root, GA_RESOURCE_DIR"/Image/Character/platform1.png", 63.0f, -278.0f, 8);
    AddColoredBlockRow( m_platforms, m_Root ,  GA_RESOURCE_DIR"/Image/Character/Color_block/blue_block.png",63.0f,-98.0f,8, ColorState::BLUE);
    AddColoredBlockRow( m_platforms, m_Root ,  GA_RESOURCE_DIR"/Image/Character/Color_block/green_block.png",63.0f,-224.0f,8, ColorState::GREEN);

    AddPlatformRow(m_platforms, m_Root, GA_RESOURCE_DIR"/Image/Character/platform1.png", 207.0f, -98.0f, 9);
    AddPlatformRow(m_platforms, m_Root, GA_RESOURCE_DIR"/Image/Character/platform1.png", 369.0f, -132.0f, 10);

    AddPlatformColumn(m_platforms, m_Root, GA_RESOURCE_DIR"/Image/Character/platform2.png", -351.0f, -260.0f, 2);
    AddPlatformColumn(m_platforms, m_Root, GA_RESOURCE_DIR"/Image/Character/platform2.png", -315.0f, -188, 2);
    AddPlatformColumn(m_platforms, m_Root, GA_RESOURCE_DIR"/Image/Character/platform2.png", -279.0f, -132.0f, 2);
    AddPlatformColumn(m_platforms, m_Root, GA_RESOURCE_DIR"/Image/Character/platform2.png", -261.0f, -260.0f, 10);
    AddPlatformColumn(m_platforms, m_Root, GA_RESOURCE_DIR"/Image/Character/platform2.png", -99.0f, -260.0f, 10);
    AddPlatformColumn(m_platforms, m_Root, GA_RESOURCE_DIR"/Image/Character/platform2.png", 45.0f, -260.0f, 9);
    AddPlatformColumn(m_platforms, m_Root, GA_RESOURCE_DIR"/Image/Character/platform2.png", 207.0f, -260.0f, 10);

    auto spring = std::make_shared<Spring>(12.0f);
    spring->SetPosition({-185.0f, -265.0f});
    m_springs.push_back(spring);
    m_Root.AddChild(spring);

    auto spring2 = std::make_shared<Spring>(12.0f);
    spring2->SetPosition({120.0f, -265.0f});
    m_springs.push_back(spring2);
    m_Root.AddChild(spring2);

    auto enemy = std::make_shared<Enemy>();
    enemy->SetPosition({-243.0f, -205.0f});
    enemy->SetPatrolRange(-243.0f, -110);
    enemy->m_Transform.scale = {1.5f, 1.5f};
    enemy->SetGroundY(-205.0f);
    m_enemies.push_back(enemy);
    m_Root.AddChild(enemy);

    auto enemy2 = std::make_shared<Enemy>();
    enemy2->SetPosition({63.0f, -205.0f});
    enemy2->SetPatrolRange(63.0f, 190);
    enemy2->m_Transform.scale = {1.5f, 1.5f};
    enemy2->SetGroundY(-205.0f);
    m_enemies.push_back(enemy2);
    m_Root.AddChild(enemy2);

    m_door = std::make_shared<Character>(GA_RESOURCE_DIR"/Image/Character/door_close.png");
    m_door->SetPosition({470.0f, -105.0f});
    m_door->m_Transform.scale = {0.3f, 0.3f};
    m_Root.AddChild(m_door);

    if (m_PRM) {
        m_Root.AddChildren(m_PRM->GetChildren());
    }
}

void App::InitStage15() {

    SetBlockGoal(4);
    m_player->SetPosition({-480.0f,0.0f});
    m_player->SetImage(GA_RESOURCE_DIR "/Image/Character/player/Green_Idle.png");


    AddPlatformRow(m_platforms, m_Root, GA_RESOURCE_DIR"/Image/Character/platform1.png", -531.0f, -44.0f, 20);
    AddPlatformRow(m_platforms, m_Root, GA_RESOURCE_DIR"/Image/Character/platform1.png", -171.0f, -224.0f, 28);
    AddPlatformRow(m_platforms, m_Root, GA_RESOURCE_DIR"/Image/Character/platform1.png", 333.0f, -296.0f, 4);
    AddPlatformRow(m_platforms, m_Root, GA_RESOURCE_DIR"/Image/Character/platform1.png", 405.0f, -44.0f, 8);

    AddPlatformColumn(m_platforms, m_Root, GA_RESOURCE_DIR"/Image/Character/platform2.png", -189.0f, -224.0f, 10);
    AddPlatformColumn(m_platforms, m_Root, GA_RESOURCE_DIR"/Image/Character/platform2.png", 405.0f, -260.0f, 12);

    AddMovingColoredBlock(m_platforms,m_movingPlatforms,m_Root, GA_RESOURCE_DIR "/Image/Character/Color_block/blue_block.png",
        -170.0f, -20.0f,
        -44.0f,
        1.0f,
        ColorState::BLUE , true);
    AddMovingColoredBlock(m_platforms,m_movingPlatforms,m_Root, GA_RESOURCE_DIR "/Image/Character/Color_block/green_block.png",
        -30.0f, 120.0f,
        -80,
        1.0f,
        ColorState::GREEN ,false);
    AddMovingColoredBlock(m_platforms,m_movingPlatforms,m_Root, GA_RESOURCE_DIR "/Image/Character/Color_block/red_block.png",
        110.0f, 260.0f,
        -116,
        1.0f,
        ColorState::RED,true);
    AddMovingColoredBlock(m_platforms,m_movingPlatforms,m_Root, GA_RESOURCE_DIR "/Image/Character/Color_block/blue_block.png",
        250.0f, 400.0f
        ,
        -152.0f,
        1.0f,
        ColorState::BLUE , false);

    for (int i = 0; i < 28; ++i) {
        float x = -171.0f + i * 18.0f;
        auto spike = std::make_shared<Spike>(glm::vec2{x, -206.0f});
        m_GameObjects.push_back(spike);
        m_Root.AddChild(spike);
    }

    for (int i = 0; i < 9; ++i) {
        float y = -200.0f + i * 18.0f;
        auto spikeRight = std::make_shared<Spike>(glm::vec2{-171.0f, y});
        spikeRight->m_Transform.rotation = glm::radians(-90.0f);
        m_Spikes.push_back(spikeRight);
        m_Root.AddChild(spikeRight);
    }



    m_door = std::make_shared<Character>(GA_RESOURCE_DIR"/Image/Character/door_close.png");
    m_door->SetPosition({360.0f, -266.0f});
    m_door->m_Transform.scale = {0.3f, 0.3f};
    m_Root.AddChild(m_door);

    if (m_PRM) {
        m_Root.AddChildren(m_PRM->GetChildren());
    }

}

void App::InitStage16() {


    AddPlatformRow(m_platforms, m_Root, GA_RESOURCE_DIR"/Image/Character/platform1.png", -531.0f, -278.0f, 60);


    if (m_PRM) {
        m_Root.AddChildren(m_PRM->GetChildren());
    }
}