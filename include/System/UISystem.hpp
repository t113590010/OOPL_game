// #ifndef UI_SYSTEM_HPP
// #define UI_SYSTEM_HPP
//
// #include <vector>
// #include <memory>
// #include <unordered_map>
// #include <glm/vec2.hpp>
// #include "Entity/UnitID.hpp"
// #include "Util/GameObject.hpp"
// #include "Util/Image.hpp"
//
// class UISystem {
// public:
//     UISystem();
//     void Draw(const std::vector<UnitID>& deck, const float* cooldowns, float money);
//     int GetClickedSlot(const glm::vec2& mousePos) const;
//
// private:
//     // --- 1. 資源成員 (只宣告，不賦值) ---
//     std::shared_ptr<Util::Image> m_SlotBg;
//     std::shared_ptr<Util::Image> m_SlotFrame;
//     std::shared_ptr<Util::Image> m_Rank1Bg;
//     std::shared_ptr<Util::Image> m_Rank2Bg;
//     std::shared_ptr<Util::Image> m_Rank3Bg;
//
//     std::unordered_map<UnitID, std::shared_ptr<Util::Image>> m_IconCache;
//
//     // --- 2. 依照剛才討論的，拆分層級渲染器 (確保 ZIndex 絕對正確) ---
//     Util::GameObject m_BgRenderer;
//     Util::GameObject m_RankRenderer;
//     Util::GameObject m_IconRenderer;
//     Util::GameObject m_FrameRenderer;
//     Util::GameObject m_TextRenderer;
//     Util::GameObject m_CooldownRenderer;
//     Util::GameObject m_MaskRenderer;
//     // --- 3. UI 常數 ---
//     const float UI_Y = -300.0f;
//     const float SLOT_X_START = -250.0f;
//     const float SLOT_SPACING = 120.0f;
//     const float SLOT_SIZE = 110.0f;
// };
//
// #endif
//

#ifndef UI_SYSTEM_HPP
#define UI_SYSTEM_HPP

#include <vector>
#include <memory>
#include <unordered_map>
#include <glm/vec2.hpp>
#include "Entity/UnitID.hpp"
#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "System/Button.hpp" // 💡 引入你的 Button

class UISystem {
public:
    UISystem();

    // 💡 新增：初始化 10 顆邊框按鈕
    void Init(const std::vector<UnitID>& deck);

    // 💡 新增：每幀更新按鈕狀態與文字
    void Update(const std::vector<UnitID>& deck, const float* cooldowns, float money);

    // 畫圖維持原樣
    void Draw(const std::vector<UnitID>& deck, const float* cooldowns, float money);

    // 💡 取得與重置點擊
    int GetClickedSlot() const { return m_ClickedSlot; }
    void ResetClick() { m_ClickedSlot = -1; }

private:
    std::shared_ptr<Util::Image> m_SlotBg;
    // ❌ m_SlotFrame 刪除，改用 Button 載入圖片
    std::shared_ptr<Util::Image> m_Rank1Bg;
    std::shared_ptr<Util::Image> m_Rank2Bg;
    std::shared_ptr<Util::Image> m_Rank3Bg;

    std::unordered_map<UnitID, std::shared_ptr<Util::Image>> m_IconCache;

    Util::GameObject m_BgRenderer;
    Util::GameObject m_RankRenderer;
    Util::GameObject m_IconRenderer;
    // ❌ m_FrameRenderer 刪除
    // ❌ m_TextRenderer 刪除
    Util::GameObject m_CooldownRenderer;
    Util::GameObject m_MaskRenderer;

    // 💡 新增：用來取代 Frame 和 Text 的邊框按鈕陣列
    std::vector<std::shared_ptr<Button>> m_SlotButtons;
    int m_ClickedSlot = -1;

    const float UI_Y = -300.0f;
    const float SLOT_X_START = -250.0f;
    const float SLOT_SPACING = 120.0f;
    const float SLOT_SIZE = 110.0f;
};

#endif