#ifndef APP_UTIL_HPP
#define APP_UTIL_HPP

#include "Util/Logger.hpp"

#include "Character.hpp"

// 玩家與任意 GameObject 的 AABB 碰撞檢查
[[nodiscard]] inline bool CheckCollision(const std::shared_ptr<Character>& player,
                                         const std::shared_ptr<Util::GameObject>& obj) {
    auto posA = player->GetTransform().translation;
    auto sizeA = abs(player->GetScaledSize());
    auto posB = obj->GetTransform().translation;
    auto sizeB = abs(obj->GetScaledSize());

    float leftA   = posA.x - sizeA.x / 2.0f;
    float rightA  = posA.x + sizeA.x / 2.0f;
    float topA    = posA.y + sizeA.y / 2.0f;
    float bottomA = posA.y - sizeA.y / 2.0f;

    float leftB   = posB.x - sizeB.x / 2.0f;
    float rightB  = posB.x + sizeB.x / 2.0f;
    float topB    = posB.y + sizeB.y / 2.0f;
    float bottomB = posB.y - sizeB.y / 2.0f;

    bool result = !(leftA > rightB || rightA < leftB || bottomA > topB || topA < bottomB);
    return result;
}

bool IsStandingOnPlatform(const std::shared_ptr<Character>& player, const std::shared_ptr<Character>& platform, float velocityY);
bool IsHittingPlatformCeiling(const std::shared_ptr<Character>& player, const std::shared_ptr<Character>& platform, float velocityY);
bool IsHittingPlatformLeft(std::shared_ptr<Character> player, std::shared_ptr<Character> platform);
bool IsHittingPlatformRight(std::shared_ptr<Character> player, std::shared_ptr<Character> platform);

// 玩家與指定區域（方形開關區域）是否重疊
[[nodiscard]] inline bool CheckCollisionWithBox(const std::shared_ptr<Character>& player,
                                                const glm::vec2& boxPos,
                                                const glm::vec2& boxSize) {
    auto playerPos = player->GetTransform().translation;
    auto playerSize = player->GetScaledSize();

    return playerPos.x < boxPos.x + boxSize.x &&
           playerPos.x + playerSize.x > boxPos.x &&
           playerPos.y < boxPos.y + boxSize.y &&
           playerPos.y + playerSize.y > boxPos.y;
}

#endif // APP_UTIL_HPP
