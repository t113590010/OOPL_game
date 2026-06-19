#ifndef DEBUG_MENU_HPP
#define DEBUG_MENU_HPP

#include <memory>
#include <functional>

#include "System/Button.hpp"
#include "Util/GameObject.hpp"
#include "Util/Image.hpp"

class DebugMenu
{
public:
    DebugMenu();

    void Update();
    void Draw();

    void SetOnAddXP(std::function<void()> callback);
    void SetOnMaxMoney(std::function<void()> callback);
    void SetOnToggleCatAttack(std::function<void()> callback);
    void SetOnToggleCatSpeed(std::function<void()> callback);
    void SetOnToggleCooldown(std::function<void()> callback);
    void SetOnInstantWin(std::function<void()> callback);
    void SetOnBack(std::function<void()> callback);

private:
    std::shared_ptr<Button> m_PanelBg;

    std::shared_ptr<Button> m_AddXPBtn;
    std::shared_ptr<Button> m_MaxMoneyBtn;
    std::shared_ptr<Button> m_ToggleCatAttackBtn;
    std::shared_ptr<Button> m_ToggleCatSpeedBtn;
    std::shared_ptr<Button> m_ToggleCooldownBtn;
    std::shared_ptr<Button> m_InstantWinBtn;
    std::shared_ptr<Button> m_BackBtn;

    std::function<void()> m_OnAddXP;
    std::function<void()> m_OnMaxMoney;
    std::function<void()> m_OnToggleCatAttack;
    std::function<void()> m_OnToggleCatSpeed;
    std::function<void()> m_OnToggleCooldown;
    std::function<void()> m_OnInstantWin;
    std::function<void()> m_OnBack;

    bool m_MaxMoneyEnabled = false;
    bool m_CatAttackBoostEnabled = false;
    bool m_CatSpeedBoostEnabled = false;
};

#endif