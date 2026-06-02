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
};

#endif