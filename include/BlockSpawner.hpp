#ifndef BLOCK_SPAWNER_HPP
#define BLOCK_SPAWNER_HPP

#include "Platform.hpp"
#include "Util/GameObject.hpp"
#include "Util/Time.hpp"
#include <memory>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>

class BlockSpawner {
public:
    BlockSpawner(const glm::vec2& position, float intervalSeconds, bool goRight, float speeds)
        : m_Position(position), m_Interval(intervalSeconds), m_Timer(0.0f), m_GoRight(goRight), m_SpawnSpeed(speeds) {
        std::srand(static_cast<unsigned int>(std::time(nullptr))); // 初始化亂數
    }

    void Update(std::vector<std::shared_ptr<Character>>& platforms, Util::Renderer& root, std::shared_ptr<Character> player, float velocityY) {
        m_Timer += Util::Time::GetDeltaTimeMs() / 1000.0f;

        if (m_Timer >= m_Interval) {
            m_Timer = 0.0f;
            SpawnRandomBlock(platforms, root);
        }

        for (auto& spawned : m_SpawnedBlocks) {
            if (!spawned.isAlive) continue;

            auto& block = spawned.block;
            float speed = spawned.speed;

            // 檢查與角色的碰撞
            bool check = IsStandingOnPlatform(player, block, velocityY)
                      || IsHittingPlatformLeft(player, block)
                      || IsHittingPlatformRight(player, block);

            if (check) {
                auto colored = std::dynamic_pointer_cast<ColoredPlatform>(block);
                if (colored) {
                    bool isSameColor = colored->GetColor() == player->GetColor();

                    if (isSameColor) {
                        RemoveBlock(block, root, platforms);  // 吃掉方塊
                    } else {
                        auto playerPos = player->GetPosition();
                        float moveX = speed * (Util::Time::GetDeltaTimeMs() / 1000.0f);
                        player->SetPosition({playerPos.x + moveX, playerPos.y});  // 推動角色
                    }
                }
            }

            // 移動方塊
            auto pos = block->GetPosition();
            pos.x += speed * (Util::Time::GetDeltaTimeMs() / 1000.0f);
            block->SetPosition(pos);

            // 存活時間更新
            spawned.lifetime += Util::Time::GetDeltaTimeMs() / 1000.0f;
            if (spawned.lifetime >= 7.0f) {
                RemoveBlock(block, root, platforms);
            }
        }
    }

    void Clear(Util::Renderer& root, std::vector<std::shared_ptr<Character>>& platforms) {
        for (auto& spawned : m_SpawnedBlocks) {
            if (spawned.isAlive) {
                root.RemoveChild(spawned.block);
                platforms.erase(std::remove(platforms.begin(), platforms.end(), spawned.block), platforms.end());
            }
        }
        m_SpawnedBlocks.clear();
        m_Timer = 0.0f;
    }

private:
    struct SpawnedBlock {
        std::shared_ptr<Character> block;
        float lifetime = 0.0f;
        float speed = 0.0f;
        bool isAlive = true;
    };

    void SpawnRandomBlock(std::vector<std::shared_ptr<Character>>& platforms, Util::Renderer& root) {
        int randomColor = std::rand() % 3;
        ColorState color;
        std::string imagePath;

        switch (randomColor) {
            case 0:
                color = ColorState::RED;
                imagePath = GA_RESOURCE_DIR "/Image/Character/Color_block/Red_block.png";
                break;
            case 1:
                color = ColorState::GREEN;
                imagePath = GA_RESOURCE_DIR "/Image/Character/Color_block/Green_block.png";
                break;
            case 2:
                color = ColorState::BLUE;
                imagePath = GA_RESOURCE_DIR "/Image/Character/Color_block/Blue_block.png";
                break;
        }

        auto block = std::make_shared<ColoredPlatform>(imagePath, color);
        block->SetPosition(m_Position);
        block->SetZIndex(10);
        platforms.push_back(block);
        root.AddChild(block);

        float speed = m_SpawnSpeed;
        if (!m_GoRight) speed = -speed;

        m_SpawnedBlocks.push_back({block, 0.0f, speed});
    }

    glm::vec2 m_Position;
    float m_Interval;
    float m_Timer;
    bool m_GoRight;
    float m_SpawnSpeed;

    std::vector<SpawnedBlock> m_SpawnedBlocks;

    void RemoveBlock(std::shared_ptr<Character> block, Util::Renderer& root, std::vector<std::shared_ptr<Character>>& platforms) {
        root.RemoveChild(block);
        platforms.erase(std::remove(platforms.begin(), platforms.end(), block), platforms.end());
        for (auto& spawned : m_SpawnedBlocks) {
            if (spawned.block == block) {
                spawned.isAlive = false;
                break;
            }
        }
    }
};

#endif // BLOCK_SPAWNER_HPP
