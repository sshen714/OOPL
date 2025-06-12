#include "PhaseResourceManger.hpp"
#include "Util/Logger.hpp"

PhaseResourceManger::PhaseResourceManger() {
    m_TaskText = std::make_shared<TaskText>();
    m_Background = std::make_shared<BackgroundImage>();
}

Stage PhaseResourceManger::NextStage() {
    int current = m_Phase++;

    if (m_Phase > 16) return Stage::STAGE_16;
    LOG_DEBUG("Passed! NextStage: {}", m_Phase);
    m_Background->NextPhase(m_Phase);

    switch (current) {
        case 1: return Stage::STAGE_1;
        case 2: return Stage::STAGE_2;
        case 3: return Stage::STAGE_3;
        case 4: return Stage::STAGE_4;
        case 5: return Stage::STAGE_5;
        case 6: return Stage::STAGE_6;
        case 7: return Stage::STAGE_7;
        case 8: return Stage::STAGE_8;
        case 9: return Stage::STAGE_9;
        case 10: return Stage::STAGE_10;
        case 11: return Stage::STAGE_11;
        case 12: return Stage::STAGE_12;
        case 13: return Stage::STAGE_13;
        case 14: return Stage::STAGE_14;
        case 15: return Stage::STAGE_15;
        default: return Stage::STAGE_16;
    }
}

std::vector<std::shared_ptr<Util::GameObject>> PhaseResourceManger::GetChildren() const {
    return {
        std::static_pointer_cast<Util::GameObject>(m_Background),
    };
}
