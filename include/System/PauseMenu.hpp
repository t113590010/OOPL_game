#ifndef PAUSE_MENU_HPP
#define PAUSE_MENU_HPP

#include <memory>
#include <functional>

#include "System/Button.hpp"
#include "Util/Image.hpp"
#include "Util/GameObject.hpp"

class PauseMenu {
public:
    PauseMenu();

    void Update();
    void Draw();
    void DrawBackgroundOnly();
    void SetOnQuit(std::function<void()> callback) {
        m_OnQuit = callback;
    }

    void SetOnClose(std::function<void()> callback) {
        m_OnClose = callback;
    }

    void SetOnHelp(std::function<void()> callback) {
        m_OnHelp = callback;
    }

    void SetBgZindex(int zIndex) {
        m_Background.SetZIndex(zIndex);
    }
    void SetOnBgmVolumeChanged(std::function<void(int)> callback) {
        m_OnBgmVolumeChanged = callback;
    }

    void SetOnSfxVolumeChanged(std::function<void(int)> callback) {
        m_OnSfxVolumeChanged = callback;
    }
private:
    // 暫停時的半透明黑底
    Util::GameObject m_Background;

    // 整張暫停選單背景圖
    std::shared_ptr<Button> m_MenuBg;

    // 右上角 X 關閉按鈕
    std::shared_ptr<Button> m_CloseBtn;

    // 幫助 / 未來 Debug Menu
    std::shared_ptr<Button> m_HelpBtn;

    // BGM / SFX 音量按鈕
    std::shared_ptr<Button> m_BgmVolumeBtn;
    std::shared_ptr<Button> m_SfxVolumeBtn;

    int m_BgmVolumeLevel = 3;
    int m_SfxVolumeLevel = 3;
    std::function<void(int)> m_OnBgmVolumeChanged;
    std::function<void(int)> m_OnSfxVolumeChanged;
    // 脫離戰鬥：透明點擊區，蓋在背景圖的「脫離戰鬥」上
    std::shared_ptr<Button> m_QuitBtn;

    // callbacks
    std::function<void()> m_OnQuit;
    std::function<void()> m_OnClose;
    std::function<void()> m_OnHelp;

};

#endif