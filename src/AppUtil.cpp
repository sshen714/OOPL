#include "AppUtil.hpp"
#include "App.hpp"
#include "Util/Logger.hpp"

bool IsStandingOnPlatform(const std::shared_ptr<Character>& player,
                          const std::shared_ptr<Character>& platform,
                          float velocityY) {
    if (!platform->GetVisibility()) return false;

    auto playerPos = player->GetPosition();
    auto platformPos = platform->GetPosition();
    auto playerSize = player->GetSize();
    auto platformSize = platform->GetSize();

    float playerBottom = playerPos.y - playerSize.y / 2.0f;
    float platformTop = platformPos.y + platformSize.y / 2.0f;

    float playerLeft = playerPos.x - playerSize.x / 2.0f;
    float playerRight = playerPos.x + playerSize.x / 2.0f;
    float platformLeft = platformPos.x - platformSize.x / 2.0f;
    float platformRight = platformPos.x + platformSize.x / 2.0f;

    bool horizontallyAligned = playerRight >= platformLeft && playerLeft <= platformRight;
    bool verticallyAligned = playerBottom >= platformTop - 5.0f &&
                             playerBottom <= platformTop + 10.0f;

    return horizontallyAligned && verticallyAligned && velocityY <= 0.0f;
}

bool IsHittingPlatformCeiling(const std::shared_ptr<Character>& player,
                               const std::shared_ptr<Character>& platform,
                               float velocityY) {
    if (!platform->GetVisibility()) return false;

    auto playerPos = player->GetPosition();
    auto platformPos = platform->GetPosition();
    auto playerSize = player->GetSize();
    auto platformSize = platform->GetSize();

    float playerTop = playerPos.y + playerSize.y / 2.0f;
    float platformBottom = platformPos.y - platformSize.y / 2.0f;

    float playerLeft = playerPos.x - playerSize.x / 2.0f;
    float playerRight = playerPos.x + playerSize.x / 2.0f;
    float platformLeft = platformPos.x - platformSize.x / 2.0f;
    float platformRight = platformPos.x + platformSize.x / 2.0f;

    bool horizontallyAligned = playerRight >= platformLeft && playerLeft <= platformRight;
    bool verticallyTouching = playerTop >= platformBottom - 5.0f &&
                              playerTop <= platformBottom + 5.0f;

    return horizontallyAligned && verticallyTouching && velocityY > 0.0f;
}

bool IsHittingPlatformLeft(std::shared_ptr<Character> player, std::shared_ptr<Character> platform) {
    auto playerPos = player->GetPosition();
    auto platformPos = platform->GetPosition();

    float playerRight = playerPos.x + player->GetSize().x / 2.0f;
    float playerTop = playerPos.y + player->GetSize().y / 2.0f;
    float playerBottom = playerPos.y - player->GetSize().y / 2.0f;

    float platformLeft = platformPos.x - platform->GetSize().x / 2.0f;
    float platformTop = platformPos.y + platform->GetSize().y / 2.0f;
    float platformBottom = platformPos.y - platform->GetSize().y / 2.0f;

    bool horizontallyTouching = std::abs(playerRight - platformLeft) < 5.0f; // 邊界靠近
    bool verticallyOverlapping = (playerBottom < platformTop) && (playerTop > platformBottom);

    return horizontallyTouching && verticallyOverlapping;
}

bool IsHittingPlatformRight(std::shared_ptr<Character> player, std::shared_ptr<Character> platform) {
    auto playerPos = player->GetPosition();
    auto platformPos = platform->GetPosition();

    float playerLeft = playerPos.x - player->GetSize().x / 2.0f;
    float playerTop = playerPos.y + player->GetSize().y / 2.0f;
    float playerBottom = playerPos.y - player->GetSize().y / 2.0f;

    float platformRight = platformPos.x + platform->GetSize().x / 2.0f;
    float platformTop = platformPos.y + platform->GetSize().y / 2.0f;
    float platformBottom = platformPos.y - platform->GetSize().y / 2.0f;

    bool horizontallyTouching = std::abs(playerLeft - platformRight) < 5.0f;
    bool verticallyOverlapping = (playerBottom < platformTop) && (playerTop > platformBottom);

    return horizontallyTouching && verticallyOverlapping;
}






