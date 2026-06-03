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

private:
    // 暫停時的半透明黑底
    Util::GameObject m_Background;

    // 中央面板
    std::shared_ptr<Button> m_PanelBg;
    std::shared_ptr<Button> m_TitleBar;

    // 右上角 X
    std::shared_ptr<Button> m_CloseBtn;

    // 幫助 / Debug Menu
    std::shared_ptr<Button> m_HelpBtn;

    // 音量滑桿：先做假 UI
    std::shared_ptr<Button> m_BgmLabel;
    std::shared_ptr<Button> m_BgmBar;
    std::shared_ptr<Button> m_BgmKnob;

    std::shared_ptr<Button> m_SfxLabel;
    std::shared_ptr<Button> m_SfxBar;
    std::shared_ptr<Button> m_SfxKnob;

    // 脫離戰鬥
    std::shared_ptr<Button> m_QuitBtn;

    // callbacks
    std::function<void()> m_OnQuit;
    std::function<void()> m_OnClose;
    std::function<void()> m_OnHelp;
};

#endif