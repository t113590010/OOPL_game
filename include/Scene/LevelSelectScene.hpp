#ifndef LEVEL_SELECT_SCENE_HPP
#define LEVEL_SELECT_SCENE_HPP

#include <functional>
#include <memory>
#include "System/Button.hpp"
#include "System/Background.hpp"
#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "System/StageSelectUI.hpp"
class LevelSelectScene {
public:

    LevelSelectScene();

    void Update();

    void Draw();

    void SetOnStageSelected(
        std::function<void(int)> callback
    );

    void SetOnReturnBtnClick(
        std::function<void()> callback
    );

private:

    std::function<void(int)> m_OnStageSelected;

    std::function<void()> m_OnReturn;

    std::shared_ptr<Button> m_BackBtn;

    /*std::vector<std::shared_ptr<Button>>
        m_StageButtons;
    */
    std::shared_ptr<Button>
    m_StartBattleBtn;

    int m_SelectedStageID = 1;
    std::shared_ptr<StageSelectUI>
    m_StageSelectUI;
    Util::GameObject m_Background;

    void StartBattleTransition(int stageID);
    void UpdateBattleTransition();

    bool m_IsStartingBattle = false;
    float m_StartBattleTimer = 0.0f;
    int m_PendingStageID = -1;
    Util::GameObject m_BattleStartFont;

    glm::vec2 m_BattleStartFontBaseScale =
        glm::vec2(1.0f, 1.0f);

    static constexpr float
        BATTLE_START_MIN_SCALE = 1.25f;

    static constexpr float
        BATTLE_START_MAX_SCALE = 4.0f;

    static constexpr float
        BATTLE_START_SCALE_DURATION = 0.6f;

    static constexpr float
        BATTLE_START_FONT_DURATION = 3.0f;

    static constexpr float
        BLACK_FADE_DURATION = 0.5f;

    static constexpr float
        BATTLE_TRANSITION_DURATION =
            BATTLE_START_FONT_DURATION +
            BLACK_FADE_DURATION;

    Util::GameObject m_BlackOverlay;

    float m_WindowWidth = 0.0f;
    float m_WindowHeight = 0.0f;

    float m_BlackOverlayStartX = 0.0f;
    float m_BlackOverlayEndX = 180.0f;
    static constexpr float
        BLACK_OVERLAY_OVERSCAN = 1.15f;
};

#endif