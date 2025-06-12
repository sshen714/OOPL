#ifndef BACKGROUND_IMAGE_HPP
#define BACKGROUND_IMAGE_HPP

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "Util/Logger.hpp"

class BackgroundImage : public Util::GameObject {

public:
    BackgroundImage() : GameObject(
    std::make_shared<Util::Image>(GA_RESOURCE_DIR"/Image/Background/Background1.png"), -10) {
    }

    void NextPhase(const int phase) {
        auto temp = std::dynamic_pointer_cast<Util::Image>(m_Drawable);
        if (temp) {
            std::string path = ImagePath(phase);
            temp->SetImage(path);
            LOG_INFO("切換背景圖：{}", path);
        } else {
            LOG_WARN("背景轉型失敗，無法切換圖片");
        }
    }

private:
    inline std::string ImagePath(const int phase) {
        return GA_RESOURCE_DIR"/Image/Background/background" + std::to_string(phase) + ".png";
    }
};

#endif //BACKGROUND_IMAGE_HPP
