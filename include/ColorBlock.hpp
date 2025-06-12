#ifndef COLORBLOCK_HPP
#define COLORBLOCK_HPP

#include "Platform.hpp"
#include "Util/Renderer.hpp"
#include <vector>
#include <memory>

//   一排彩色方塊
inline void AddColoredBlockRow(std::vector<std::shared_ptr<Character>>& platforms,
                               Util::Renderer& root,
                               const std::string& imagePath,
                               float startX, float y,
                               int count, ColorState color,
                               bool toRight = true) {
    float tileSize = 18.0f;
    for (int i = 0; i < count; ++i) {
        float x = toRight ? startX + i * tileSize : startX - i * tileSize;
        auto platform = std::make_shared<ColoredPlatform>(imagePath, color);
        platform->SetPosition({x, y});
        platform->SetZIndex(10);
        platforms.push_back(platform);
        root.AddChild(platform);
    }
}

//   一欄彩色方塊
inline void AddColoredBlockColumn(std::vector<std::shared_ptr<Character>>& platforms,
                                  Util::Renderer& root,
                                  const std::string& imagePath,
                                  float x, float startY,
                                  int count, ColorState color,
                                  bool toBottom = true) {
    float tileSize = 18.0f;
    for (int i = 0; i < count; ++i) {
        float y = toBottom ? startY + i * tileSize : startY - i * tileSize;
        auto platform = std::make_shared<ColoredPlatform>(imagePath, color);
        platform->SetPosition({x, y});
        platform->SetZIndex(10);
        platforms.push_back(platform);
        root.AddChild(platform);
    }
}

//  加入的可移動彩色方塊
inline std::shared_ptr<MovingColoredPlatform> AddMovingColoredBlock(
        std::vector<std::shared_ptr<Character>>& platforms,
        std::vector<std::shared_ptr<Character>>& movingPlatforms,
        Util::Renderer& root,
        const std::string& imagePath,
        float left, float right,
        float y,
        float speed,
        ColorState color,                 // ★ 新增：顏色
        bool startFromRight = false)
{
    /* ★ 1. 建立 MovingColoredPlatform 而不是 MovingPlatform */
    auto platform = std::make_shared<MovingColoredPlatform>(
                        imagePath, left, right, speed, color);

    if (startFromRight) {                 // 右端起跑 → 先往左
        platform->SetPosition({right, y});
        platform->SetDirection(-1);       // ★ 2. 方向 setter 仍可用
    } else {                              // 左端起跑 → 先往右
        platform->SetPosition({left,  y});
        platform->SetDirection(+1);
    }

    platform->SetZIndex(10);
    platforms.push_back(platform);
    movingPlatforms.push_back(platform);
    root.AddChild(platform);
    return platform;
}



#endif // COLORBLOCK_HPP
