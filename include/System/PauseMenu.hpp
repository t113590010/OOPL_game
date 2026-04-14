#ifndef PAUSE_MENU_HPP
#define PAUSE_MENU_HPP

#include <vector>
#include <memory>
#include "System/Button.hpp"
#include "Util/Image.hpp"
#include "Util/GameObject.hpp"
#include <functional>

class PauseMenu {
public:
    PauseMenu();

    void Update();
    void Draw();

    // 給外部設定 Callback，例如點了「繼續」要幹嘛、「回主畫面」要幹嘛
    void SetOnResume(std::function<void()> callback) { m_OnResume = callback; }
    void SetOnQuit(std::function<void()> callback) { m_OnQuit = callback; }

private:
    Util::GameObject m_Background; // 暫停時的半透明黑底
    std::shared_ptr<Button> m_ResumeBtn;
    std::shared_ptr<Button> m_QuitBtn;

    std::function<void()> m_OnResume;
    std::function<void()> m_OnQuit;
};

#endif