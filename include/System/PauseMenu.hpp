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


    void SetOnQuit(std::function<void()> callback) { m_OnQuit = callback; }
    void SetBgZindex(int zIndex) {
        m_Background.SetZIndex(zIndex);
    }


private:
    Util::GameObject m_Background; // 暫停時的半透明黑底

    std::shared_ptr<Button> m_QuitBtn;
    std::function<void()> m_OnQuit;
};

#endif