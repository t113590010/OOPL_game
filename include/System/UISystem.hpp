
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
#include "NumberSystem.hpp"
class UISystem {
public:
    UISystem();

    // 💡 新增：初始化 10 顆邊框按鈕
    void Init(const std::vector<UnitID>& deck);

    // 幫 Update 函數多加一個參數，用來傳遞現在升級錢包要多少錢 (如果是 -1 代表滿級)
    void Update(const std::vector<UnitID>& deck, const float* cooldowns, float money, int walletUpgradeCost, float cannonProgress, bool isCannonReady);

    // 畫圖維持原樣
    void Draw(const std::vector<UnitID>& deck, const float* cooldowns, float money);

    // 💡 取得與重置點擊
    int GetClickedSlot() const { return m_ClickedSlot; }
    void ResetClick() { m_ClickedSlot = -1; }
    // 讓 GameScene 可以把升級錢包的邏輯傳進來
    void SetOnWalletUpgrade(std::function<void()> callback) { m_OnWalletUpgrade = callback; }
    void SetOnFireCannon(std::function<void()> callback) { m_OnFireCannon = callback; }

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
    std::vector<std::shared_ptr<NumberSystem>> m_SlotNumbers;
    std::shared_ptr<NumberSystem> m_WalletNumber;
    std::shared_ptr<NumberSystem> m_CannonNumber;
    // 💡 新增：用來取代 Frame 和 Text 的邊框按鈕陣列
    std::vector<std::shared_ptr<Button>> m_SlotButtons;
    int m_ClickedSlot = -1;

    const float UI_Y = -300.0f;
    const float SLOT_X_START = -250.0f;
    const float SLOT_SPACING = 120.0f;
    const float SLOT_SIZE = 110.0f;
    std::shared_ptr<Button> m_WalletBtn;
    std::function<void()> m_OnWalletUpgrade;
    std::shared_ptr<Button> m_CannonBtn;
    std::function<void()> m_OnFireCannon;
};

#endif