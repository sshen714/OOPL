#ifndef TASKTEXT_HPP
#define TASKTEXT_HPP

#include "Util/GameObject.hpp"
#include "Util/Text.hpp"
#include "Util/Color.hpp"
#include <memory>

class TaskText : public Util::GameObject {
public:
    TaskText() = default;

    TaskText(const std::string& fontPath, int fontSize, const glm::vec2& pos, const Util::Color& color)
        : m_FontPath(fontPath), m_FontSize(fontSize), m_Color(color) {
        m_Drawable = std::make_shared<Util::Text>(fontPath, fontSize, "Target : 0 / 0", color);
        m_Transform.translation = pos;
    }

    void UpdateText(int collected, int target) {
        if (auto text = std::dynamic_pointer_cast<Util::Text>(m_Drawable)) {
            text->SetText( " Target:" +std::to_string(collected) + " / " + std::to_string(target));
        }
    }


private:
    std::string m_FontPath;
    int m_FontSize = 20;
    Util::Color m_Color = Util::Color::FromRGB(0, 0, 0);
};

#endif // TASKTEXT_HPP
