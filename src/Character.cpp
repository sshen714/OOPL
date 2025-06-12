#include "Character.hpp"
#include "App.hpp"
#include "Util/Image.hpp"
#include "Util/Time.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"

Character::Character(const std::string& ImagePath) {
    SetImage(ImagePath);
    ResetPosition();
}

std::string GetColorPrefix(ColorState color) {
    switch (color) {
        case ColorState::RED:   return "Red";
        case ColorState::GREEN: return "Green";
        case ColorState::BLUE:  return "Blue";
        default:                return "Green";
    }
}

void App::HandleColorSwitching() {
    if (Util::Input::IsKeyPressed(Util::Keycode::Z)) {
        m_player->SetColor(ColorState::RED);
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::X)) {
        m_player->SetColor(ColorState::GREEN);
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::C)) {
        m_player->SetColor(ColorState::BLUE);
    }
}

void App::UpdatePlayerImage(bool isJumping, bool ) {

    std::string colorPrefix;
    switch (m_player->GetColor()) {
        case ColorState::RED: colorPrefix = "Red"; break;
        case ColorState::GREEN: colorPrefix = "Green"; break;
        case ColorState::BLUE: colorPrefix = "Blue"; break;
        default: colorPrefix = "Green"; break;
    }

    static int frame = 0;
    frame++;

    if (isJumping) {
        m_player->SetImage(GA_RESOURCE_DIR"/Image/Character/player/" + colorPrefix + "_jump(9).png");
    } else if ((Util::Input::IsKeyPressed(Util::Keycode::LEFT) && !Util::Input::IsKeyPressed(Util::Keycode::RIGHT))
                 || (Util::Input::IsKeyPressed(Util::Keycode::RIGHT) && !Util::Input::IsKeyPressed(Util::Keycode::LEFT))) {
        switch ((frame / 6) % 3) {
            case 0:
                m_player->SetImage(GA_RESOURCE_DIR"/Image/Character/player/" + colorPrefix + "_walk(1).png");
            break;
            case 1:
                m_player->SetImage(GA_RESOURCE_DIR"/Image/Character/player/" + colorPrefix + "_walk(2).png");
            break;
            case 2:
                m_player->SetImage(GA_RESOURCE_DIR"/Image/Character/player/" + colorPrefix + "_walk(3).png");
            break;
        }
    } else {
        m_player->SetImage(GA_RESOURCE_DIR"/Image/Character/player/" + colorPrefix  + "_idle.png");
    }
}


void Character::SetImage(const std::string& ImagePath) {
    m_ImagePath = ImagePath;
    m_Drawable = std::make_shared<Util::Image>(m_ImagePath);
}

void Character::Update() {
    // 預設可以不做事，讓子類自己 override
}

void Character::UpdateCooldown() {
    if (m_DamageCooldown > 0.0f) {
        m_DamageCooldown -= Util::Time::GetDeltaTimeMs();
    }
}

void App::SetBlockGoal(int count) {
    m_TargetBlockCount = count;
    m_CollectedBlockCount = 0;

    if (m_TaskText) {
        m_TaskText->UpdateText(m_CollectedBlockCount, m_TargetBlockCount);
    }
}

void Character::ApplyGravity(float gravity, float groundY) {
    m_VelocityY += gravity;
    auto pos = GetPosition();
    float newY = pos.y + m_VelocityY;
    if (newY <= groundY) {
        newY = groundY;
        m_VelocityY = 0.0f;
    }
    SetPosition({pos.x, newY});
}

