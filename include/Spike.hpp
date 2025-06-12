#ifndef OBSTACLE_SPIKE_HPP
#define OBSTACLE_SPIKE_HPP

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"


class Spike : public Util::GameObject {
public:
    explicit Spike(const glm::vec2& position, float zIndex = -1.0f);

    void Update(); // 預留行為
};

#endif // OBSTACLE_SPIKE_HPP
