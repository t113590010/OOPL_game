#ifndef LevelUpgrade_HPP
#define LevelUpgrade_HPP

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "Button.hpp"
#include <memory>
#include <functional>
#include "NumberSystem.hpp"
#include "DeckUI.hpp"
// 💡 注意：不用引入 SlotUI，因為升級畫面沒有出陣的格子

class LevelUpgradeScene {
public:
    LevelUpgradeScene();

    void Update();
    void Draw();

    void SetOnReturnBtnClick(std::function<void()> callback);
    void Refresh(); // 每次進來時同步最新的 XP 與貓咪等級

private:
    Util::GameObject m_Background;

    // 返回按鈕
    std::shared_ptr<Button> m_ReturnBtn;
    std::function<void()> m_OnReturn;

    // 資源顯示
    std::shared_ptr<NumberSystem> m_XPNumber;
    std::shared_ptr<NumberSystem> m_CatFoodNumber;
    int m_CatFood = 0;
    int m_XP = 0;

    // 沿用下方滑動列表與邊框
    std::shared_ptr<DeckUI> m_DeckUI;
    std::shared_ptr<Button> m_LeftBorder;
    std::shared_ptr<Button> m_RightBorder;

    // ==========================================
    // 🚀 升級畫面專屬的變數
    // ==========================================
    UnitID m_SelectedCatID = static_cast<UnitID>(0); // 記錄玩家目前「點擊選中」了哪一隻貓
    // 🚀 新增：選中貓咪的放大圖
    // std::shared_ptr<Util::GameObject> m_SelectedCatIcon;

    // 🚀 新增：「升級」按鈕
    std::shared_ptr<Button> m_UpgradeBtn;

    // 🚀 新增：輔助函式，用來刷新上半部的大圖與按鈕狀態
    // void RefreshSelectedCatInfo();
    void RefreshResourceNumbers();
    void RefreshUpgradeCost();
    std::shared_ptr<NumberSystem> m_UpgradeCostXP;   // 顯示升級需要花費多少 XP

    // TODO: 之後你可以在這裡加上顯示「選中貓咪」的放大圖、名稱、等級數字等 UI 元件
};

#endif