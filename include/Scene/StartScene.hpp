#ifndef STARTSCENE_HPP
#define STARTSCENE_HPP
#include "System/StartOptionMenu.hpp"
#include "Util/GameObject.hpp"
#include "System/Button.hpp"
#include <functional>
#include <memory>
#include "Util/SFX.hpp"
#include "System/DebugMenu.hpp"

class StartScene {
public:
    StartScene();

    void Update();
    void Draw();

    // 留給外面綁定的接口：按下去之後要觸發什麼事（例如切換到選關畫面）
    void SetOnStartGame(std::function<void()> callback);
    void SetOnBgmVolumeChanged(
        std::function<void(int)> callback
    );

    void SetOnSfxVolumeChanged(
        std::function<void(int)> callback
    );

private:
    Util::GameObject m_Background;           // 背景圖
    std::shared_ptr<Button> m_StartBtn;      // 開始按鈕
    std::function<void()> m_OnStartGame;     // 事件推播器
    std::shared_ptr<StartOptionMenu> m_OptionMenu;
    std::shared_ptr<Button> m_OptionHitBtn;
    std::function<void(int)> m_OnBgmVolumeChanged;
    std::function<void(int)> m_OnSfxVolumeChanged;
    std::shared_ptr<DebugMenu> m_DebugMenu;
};

#endif