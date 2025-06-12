#ifndef PHASE_RESOURCE_MANAGER_HPP
#define PHASE_RESOURCE_MANAGER_HPP

#include "TaskText.hpp"
#include "BackgroundImage.hpp"
#include "Phase.hpp"
#include "Util/GameObject.hpp"

class PhaseResourceManger {
public:
    PhaseResourceManger();
    Stage NextStage();

    std::vector<std::shared_ptr<Util::GameObject>> GetChildren() const;

private:
    int m_Phase = 1;
    std::shared_ptr<TaskText> m_TaskText;
    std::shared_ptr<BackgroundImage> m_Background;

};

#endif // PHASE_RESOURCE_MANAGER_HPP
