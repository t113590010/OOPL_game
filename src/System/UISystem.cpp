#include "System/UISystem.hpp"
#include "Entity/UnitFactory.hpp"
#include "Util/Text.hpp"
#include "Util/Color.hpp"
#include <string>

UISystem::UISystem() {
    // 💡 保持路徑註解，避免破圖，等到你有素材再解開
    m_PanelBg = nullptr;
    m_SlotBg = nullptr;
    m_CooldownMask = nullptr;
}
void UISystem::Draw(const std::vector<UnitID>& deck, const float* cooldowns, float money) {
    // --- 座標基準 ---
    const float UI_Y = -310.0f;
    const float SLOT_X_START = -250.0f;
    const float SLOT_SPACING = 120.0f;
    const float ICON_SIZE = 80.0f;

    // 💡 這裡！我把 X 和 Y 的偏移量全部拉出來讓你調
    // 如果文字沒置中，你就調這個 X_OFFSET (例如 -10.0f 或 10.0f)
    const float TEXT_X_OFFSET = 12.0f;
    const float TEXT_Y_OFFSET = 50.0f;

    for (size_t i = 0; i < deck.size() && i < 5; ++i) {
        float x = SLOT_X_START + (static_cast<float>(i) * SLOT_SPACING);
        UnitID id = deck[i];
        if (id == UnitID::NONE) continue;

        int cost = UnitFactory::GetUnitCost(id);

        // 1. 畫頭像 (不變)
        auto icon = std::make_shared<Util::Image>(UnitFactory::GetUnitIconPath(id));
        if (icon && icon->GetSize().x > 0) {
            m_Renderer.SetDrawable(icon);
            m_Renderer.m_Transform.translation = {x, UI_Y};
            float finalScale = (money < static_cast<float>(cost)) ? ICON_SIZE * 0.75f : ICON_SIZE;
            m_Renderer.m_Transform.scale = { finalScale / icon->GetSize().x, finalScale / icon->GetSize().y };
            m_Renderer.Draw();
        }

        // 2. 💡 價格文字：同時套用 X 和 Y 的偏移
        // 確保路徑與你電腦一致 /Font/arial.ttf
        auto costText = std::make_shared<Util::Text>(
            RESOURCE_DIR"/Font/arial.ttf",
            20,
            std::to_string(cost),
            Util::Color(255, 255, 0, 255) // 黃色
        );

        if (costText) {
            m_Renderer.SetDrawable(costText);
            // 💡 這裡！x + TEXT_X_OFFSET 確保你左右能微調，UI_Y + TEXT_Y_OFFSET 確保上下對齊
            m_Renderer.m_Transform.translation = { x + TEXT_X_OFFSET, UI_Y + TEXT_Y_OFFSET };
            m_Renderer.m_Transform.scale = { 1.0f, 1.0f };
            m_Renderer.Draw();
        }

        // 3. 冷卻遮罩 (維持原樣)
        if (cooldowns[i] > 0 && m_CooldownMask && m_CooldownMask->GetSize().x > 0) {
            float ratio = cooldowns[i] / UnitFactory::GetUnitSpawnCooldown(id);
            m_Renderer.SetDrawable(m_CooldownMask);
            m_Renderer.m_Transform.scale = { ICON_SIZE / m_CooldownMask->GetSize().x, (ICON_SIZE * ratio) / m_CooldownMask->GetSize().y };
            float yOffset = (ICON_SIZE * (1.0f - ratio)) / 2.0f;
            m_Renderer.m_Transform.translation = { x, UI_Y - yOffset };
            m_Renderer.Draw();
        }
    }
}