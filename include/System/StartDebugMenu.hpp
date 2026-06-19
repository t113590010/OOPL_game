#ifndef START_DEBUG_MENU_HPP
#define START_DEBUG_MENU_HPP

#include <functional>
#include <memory>

#include "Button.hpp"

class StartDebugMenu
{
public:
    StartDebugMenu();

    void Update();
    void Draw();

    void SetOnBack(std::function<void()> callback);

private:
    std::shared_ptr<Button> m_PanelBg;

    std::shared_ptr<Button> m_AddNormalTicketBtn;
    std::shared_ptr<Button> m_AddRareTicketBtn;
    std::shared_ptr<Button> m_AddCatFoodBtn;

    std::shared_ptr<Button> m_BackBtn;
    std::function<void()> m_OnBack;
};

#endif