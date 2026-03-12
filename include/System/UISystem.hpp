#ifndef UI_SYSTEM_HPP
#define UI_SYSTEM_HPP

#include <vector>
#include <memory>
#include <unordered_map>
#include <glm/vec2.hpp>
#include "Entity/UnitID.hpp"
#include "Util/GameObject.hpp"
#include "Util/Image.hpp"

class UISystem {
public:
    UISystem();
    void Draw(const std::vector<UnitID>& deck, const float* cooldowns, float money);
    int GetClickedSlot(const glm::vec2& mousePos) const;

private:
    // --- 1. 資源成員 (只宣告，不賦值) ---
    std::shared_ptr<Util::Image> m_SlotBg;
    std::shared_ptr<Util::Image> m_SlotFrame;
    std::shared_ptr<Util::Image> m_Rank1Bg;
    std::shared_ptr<Util::Image> m_Rank2Bg;
    std::shared_ptr<Util::Image> m_Rank3Bg;

    std::unordered_map<UnitID, std::shared_ptr<Util::Image>> m_IconCache;

    // --- 2. 依照剛才討論的，拆分層級渲染器 (確保 ZIndex 絕對正確) ---
    Util::GameObject m_BgRenderer;
    Util::GameObject m_RankRenderer;
    Util::GameObject m_IconRenderer;
    Util::GameObject m_FrameRenderer;
    Util::GameObject m_TextRenderer;
    Util::GameObject m_CooldownRenderer;
    Util::GameObject m_MaskRenderer;
    // --- 3. UI 常數 ---
    const float UI_Y = -310.0f;
    const float SLOT_X_START = -250.0f;
    const float SLOT_SPACING = 120.0f;
    const float SLOT_SIZE = 110.0f;
};

#endif