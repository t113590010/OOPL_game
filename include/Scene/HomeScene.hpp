#ifndef HOME_SCENE_HPP
#define HOME_SCENE_HPP
#include "NumberSystem.hpp"
#include "Util/GameObject.hpp"
#include "System/Button.hpp"
#include <functional>
#include <memory>

class HomeScene : public Util::GameObject {
public:
    HomeScene();

    // 🚀 對 App 開放的事件接口
    void SetOnStartBtnClick(std::function<void()> callback);
    void SetOnUpgradeBtnClick(std::function<void()> callback);
    void SetOnTeamBtnClick(std::function<void()> callback);
    void SetOnStorageBtnClick(std::function<void()> callback);
    void SetOnRareGachaBtnClick(std::function<void()> callback);
    void SetOnNormalGachaBtnClick(std::function<void()> callback);

    // 🚀 新增：返回初始場景
    void SetOnReturnBtnClick(std::function<void()> callback);

    void Update();
    void Draw();

private:
    // =========================
    // 背景
    // =========================
    Util::GameObject m_Background;
    Util::GameObject m_Background2;

    // =========================
    // 主選單按鈕
    // =========================
    std::shared_ptr<Button> m_StartBtn;
    std::shared_ptr<Button> m_UpgradeBtn;
    std::shared_ptr<Button> m_TeamBtn;
    std::shared_ptr<Button> m_StorageBtn;
    std::shared_ptr<Button> m_RareGachaBtn;
    std::shared_ptr<Button> m_NormalGachaBtn;

    // =========================
    // 返回按鈕
    // =========================
    std::shared_ptr<Button> m_ReturnBtn;

    // =========================
    // Callback
    // =========================
    std::function<void()> m_OnReturn;
    std::function<void()> m_OnStart;
    std::function<void()> m_OnUpgradeBtnClick;
    std::function<void()> m_OnTeamBtnClick;
    std::function<void()> m_OnStorageBtnClick;
    std::function<void()> m_OnRareGachaBtnClick;
    std::function<void()> m_OnNormalGachaBtnClick;
    // 資源顯示
    std::shared_ptr<NumberSystem> m_XPNumber;
    std::shared_ptr<NumberSystem> m_CatFoodNumber;

    int m_XP = 0;
    int m_CatFood = 0;
};

#endif