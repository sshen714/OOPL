#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <string>
#include "Util/GameObject.hpp"
#include "Util/Image.hpp"

// 顏色狀態
enum class ColorState {
    NONE,
    RED,
    GREEN,
    BLUE
};

class Character : public Util::GameObject {
public:
    explicit Character(const std::string& ImagePath);

    // 顏色設定
    void SetColor(ColorState color) { m_Color = color; }
    ColorState GetColor() const { return m_Color; }

    void UpdateColorImage();

    // 運動邏輯（怪物也可共用）
    void SetVelocityY(float vy) { m_VelocityY = vy; }
    float GetVelocityY() const { return m_VelocityY; }

    void ApplyGravity(float gravity, float groundY);

    virtual void Update();
    void UpdateCooldown();

    // 圖片與位置
    void SetImage(const std::string& ImagePath);
    void SetPosition(const glm::vec2& Position) { m_Transform.translation = Position; }
    void SetScale(const glm::vec2& scale) { m_Transform.scale = scale; }
    const std::string& GetImagePath() const { return m_ImagePath; }
    const glm::vec2& GetPosition() const { return m_Transform.translation; }
    bool GetVisibility() const { return m_Visible; }
    glm::vec2 GetScale() const { return m_Transform.scale; }

    glm::vec2 GetSize() const {
        if (auto image = std::dynamic_pointer_cast<Util::Image>(m_Drawable)) {
            return image->GetSize();
        }
        return {0.0f, 0.0f};
    }

    Character(const Character&) = delete;
    Character(Character&&) = delete;
    Character& operator=(const Character&) = delete;
    Character& operator=(Character&&) = delete;

protected:
    float m_VelocityY = 0.0f;

private:
    std::string m_ImagePath;
    int m_Health = 1000;
    float m_DamageCooldown = 0.0f;
    ColorState m_Color = ColorState::GREEN;
    void ResetPosition() { m_Transform.translation = {0, 0}; }
};

#endif // CHARACTER_HPP
