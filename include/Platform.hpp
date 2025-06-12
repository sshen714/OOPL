#ifndef PLATFORM_HPP
#define PLATFORM_HPP

#include "Character.hpp"  // 包含 ColorState 定義
#include "AppUtil.hpp"
#include "Util/Renderer.hpp"
#include <memory>
#include <vector>

//  普通靜態平台
class Platform : public Character {
public:
    Platform(const std::string& path) : Character(path) {}
};

//  彩色平台
class ColoredPlatform : public Character {
public:
    ColoredPlatform(const std::string& path, ColorState color)
        : Character(path), m_Color(color) {}

    ColorState GetColor() const { return m_Color; }

     bool CanStand(ColorState playerColor) const {
        return m_Color != playerColor;
    }

private:
    ColorState m_Color;
};

//  會移動的普通平台
class MovingPlatform : public Character, public std::enable_shared_from_this<MovingPlatform> {
public:
    MovingPlatform(const std::string& path, float left, float right, float speed)
        : Character(path), m_LeftX(left), m_RightX(right), m_Speed(speed) {}

    void Update(std::shared_ptr<Character> player, float velocityY) {
        auto pos = GetPosition();
        pos.x += m_Speed;
        if (pos.x < m_LeftX || pos.x > m_RightX) {
            m_Speed = -m_Speed;
            pos.x += m_Speed;
        }
        SetPosition(pos);

        bool check = IsStandingOnPlatform(player, shared_from_this(), velocityY)
                  || IsHittingPlatformLeft(player, shared_from_this())
                  || IsHittingPlatformRight(player, shared_from_this());

        if (check) {
            auto playerPos = player->GetPosition();
            player->SetPosition({playerPos.x + m_Speed, playerPos.y});
        }
    }

private:
    float m_LeftX, m_RightX;
    float m_Speed;
};

//  會移動的彩色平台
class MovingColoredPlatform : public ColoredPlatform, public std::enable_shared_from_this<MovingColoredPlatform> {
public:
    MovingColoredPlatform(const std::string& path, float left, float right, float speed, ColorState color)
        : ColoredPlatform(path, color), m_LeftX(left), m_RightX(right), m_Speed(speed) {}

    void SetDirection(int dir)
    {   // dir >= 0：往右，dir < 0：往左
        m_Speed = std::abs(m_Speed) * (dir >= 0 ? 1.0f : -1.0f);
    }

    void Update(std::shared_ptr<Character> player, ColorState playerColor, float velocityY) {
        auto pos = GetPosition();
        pos.x += m_Speed;
        if (pos.x < m_LeftX || pos.x > m_RightX) {
            m_Speed = -m_Speed;
            pos.x += m_Speed;
        }
        SetPosition(pos);

        bool check = IsStandingOnPlatform(player, shared_from_this(), velocityY)
                  || IsHittingPlatformLeft(player, shared_from_this())
                  || IsHittingPlatformRight(player, shared_from_this());

        if (check) {
            auto playerPos = player->GetPosition();
            player->SetPosition({playerPos.x + m_Speed, playerPos.y});
        }
    }

private:
    float m_LeftX, m_RightX;
    float m_Speed;
};

//  工具：橫排平台
inline void AddPlatformRow(std::vector<std::shared_ptr<Character>>& platforms,
                           Util::Renderer& root,
                           const std::string& imagePath,
                           float startX, float y,
                           int count, bool toRight = true) {
    float tileSize = 18.0f;
    for (int i = 0; i < count; ++i) {
        float x = toRight ? startX + i * tileSize : startX - i * tileSize;
        auto platform = std::make_shared<Character>(imagePath);
        platform->SetPosition({x, y});
        platform->SetZIndex(10);
        platforms.push_back(platform);
        root.AddChild(platform);
    }
}

//   工具：直排平台
inline void AddPlatformColumn(std::vector<std::shared_ptr<Character>>& platforms,
                              Util::Renderer& root,
                              const std::string& imagePath,
                              float x, float startY,
                              int count) {
    float tileSize = 18.0f;
    for (int i = 0; i < count; ++i) {
        float y = startY + i * tileSize;
        auto platform = std::make_shared<Character>(imagePath);
        platform->SetPosition({x, y});
        platform->SetZIndex(10);
        platforms.push_back(platform);
        root.AddChild(platform);
    }
}

//  工具：建立普通移動平台
inline std::shared_ptr<MovingPlatform> AddMovingPlatform(
    std::vector<std::shared_ptr<Character>>& platforms,
    std::vector<std::shared_ptr<Character>>& movingPlatforms,
    Util::Renderer& root,
    const std::string& imagePath,
    float left, float right,
    float y,
    float speed) {

    auto platform = std::make_shared<MovingPlatform>(imagePath, left, right, speed);
    platform->SetPosition({left, y});
    platform->SetZIndex(10);
    platforms.push_back(platform);
    movingPlatforms.push_back(platform);
    root.AddChild(platform);
    return platform;
}

#endif // PLATFORM_HPP
