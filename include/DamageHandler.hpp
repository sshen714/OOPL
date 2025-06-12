#ifndef DAMAGE_HANDLER_HPP
#define DAMAGE_HANDLER_HPP

#include "Character.hpp"
#include "Util/Logger.hpp"
#include "Util/Renderer.hpp"
#include <memory>

inline std::string GetColorPrefixFromColorState(ColorState color) {
    switch (color) {
        case ColorState::RED:   return "Red";
        case ColorState::GREEN: return "Green";
        case ColorState::BLUE:  return "Blue";
        default: return "Green";
    }
}

class DamageHandler {
public:
    DamageHandler(std::shared_ptr<Character> player, const glm::vec2& respawnPoint)
        : m_player(std::move(player)), m_respawnPoint(respawnPoint) {}

    void TriggerHurt() {
        if (!m_IsHurting) {
            m_IsHurting = true;
            m_HurtFrameCounter = 0;
            LOG_INFO(" 玩家受傷，啟動受傷動畫");
        }
    }

    bool UpdateHurtAnimation(Util::Renderer& root) {
        if (!m_player) return false;

        if (m_IsHurting) {
            if (m_HurtFrameCounter < 12) {
                int index = (m_HurtFrameCounter / 6) + 1;
                std::string colorPrefix = GetColorPrefixFromColorState(m_player->GetColor());
                std::string hurtPath = std::string(GA_RESOURCE_DIR) + "/Image/Character/player/" + colorPrefix + "_Hurt(" + std::to_string(index) + ").png";
                m_player->SetImage(hurtPath);
                m_HurtFrameCounter++;
                root.Update();
                return true;
            }
            m_IsHurting = false;
            m_HurtFrameCounter = 0;
            m_player->SetPosition(m_respawnPoint);
            LOG_INFO("玩家重生於: ({}, {})", m_respawnPoint.x, m_respawnPoint.y);

        }

        return false;
    }

    bool IsHurting() const { return m_IsHurting; }

private:
    std::shared_ptr<Character> m_player;
    glm::vec2 m_respawnPoint;
    bool m_IsHurting = false;
    int m_HurtFrameCounter = 0;
};

#endif // DAMAGE_HANDLER_HPP
