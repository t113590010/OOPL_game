#ifndef HOME_SCENE_HPP
#define HOME_SCENE_HPP

#include "Util/GameObject.hpp"
#include "System/Button.hpp"
#include <functional>
#include <memory>

class HomeScene : public Util::GameObject {
public:
    HomeScene(); // 建構子宣告

    // 🚀 給 App.cpp 對接用的接口宣告
    void SetOnStartBtnClick(std::function<void()> callback);
    void SetOnUpgradeBtnClick(std::function<void()> callback); // 升級 (預留)
    void SetOnTeamBtnClick(std::function<void()> callback);    // 隊伍 (預留)
    void Update() ;
    void Draw() ;

private:
    Util::GameObject m_Background; // 大廳背景
    std::shared_ptr<Button> m_StartBtn;
    std::shared_ptr<Button> m_UpgradeBtn; // 升級按鈕
    std::shared_ptr<Button> m_TeamBtn;    // 隊伍編成按鈕

    std::function<void()> m_OnStart;
    std::function<void()> m_OnUpgradeBtnClick;
    std::function<void()> m_OnTeamBtnClick;
};

#endif