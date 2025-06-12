#ifndef SPRING_HPP
#define SPRING_HPP

#include "Character.hpp"
#include <vector>
#include <string>

// 圖片所在目錄（請確認 GA_RESOURCE_DIR 已在專案中定義）
#define SPRING_DIR GA_RESOURCE_DIR "/Image/Character"

class Spring : public Character
{
public:

    explicit Spring(float boostStrength = 10.0f,
                    float frameIntervalMs = 50.0f);

    void Update(bool isPlayerOnSpring);
    bool IsSquishing() const { return m_IsSquishing; }


    float GetBoostStrength() const;

private:
    /** 載入 spring3 → spring2 → spring1 三張圖片 */
    void LoadAnimationFrames();
    /** 切下一張動畫圖；若已到最後一張則回到初始狀態 */
    void AdvanceAnimationFrame();

    std::vector<std::string> m_Frames;
    std::size_t m_FrameIndex { 0 };
    float m_Timer { 0.0f };
    const float m_FrameIntervalMs;        // 動畫幀間隔
    bool  m_IsSquishing { false };
    const float m_BoostStrength;          // 彈簧推力
};

#endif // SPRING_HPP
