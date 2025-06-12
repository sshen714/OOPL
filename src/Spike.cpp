#include "Spike.hpp"

Spike::Spike(const glm::vec2& position, float zIndex)
    : Util::GameObject(
        std::make_shared<Util::Image>(GA_RESOURCE_DIR "/Image/Background/obstacle1.png"),
        zIndex,
        glm::vec2{1.0f, 1.0f} // pivot
    )
{
    m_Transform.translation = position;
    m_Transform.scale = glm::vec2{0.18f, 0.18f}; // 可調整地刺大小
}


