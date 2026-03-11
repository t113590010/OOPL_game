#ifndef UI_SYSTEM_HPP
#define UI_SYSTEM_HPP

#include <vector>
#include <memory>
#include "Entity/UnitID.hpp"
#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "System/UIText.hpp"
class UISystem {
public:
    UISystem();
    void Draw(const std::vector<UnitID>& deck, const float* cooldowns, float money);

private:
    std::shared_ptr<Util::Image> m_PanelBg;
    std::shared_ptr<Util::Image> m_SlotBg;
    std::shared_ptr<Util::Image> m_CooldownMask;
    std::vector<std::shared_ptr<UIText>> m_PriceTexts;
    Util::GameObject m_Renderer;

    // UI 常數設定
    const float SLOT_START_X = -450.0f;
    const float SLOT_Y = -300.0f;
    const float SLOT_SPACING = 120.0f;
};

#endif