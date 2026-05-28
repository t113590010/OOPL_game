#ifndef DECK_UI_HPP
#define DECK_UI_HPP

#include <vector>
#include <memory>
#include <functional>
#include "Entity/UnitID.hpp"
#include "CatCardUI.hpp" // 🚀 引入剛剛寫好的單個卡片組件

class DeckUI {
public:
    DeckUI();

    void Update();
    void Draw();

    void LoadCats(const std::vector<UnitID>& catList);

    // 💡 分開兩種點擊事件
    void SetOnCatSelected(std::function<void(UnitID)> callback);
    void SetOnSwapClick(std::function<void(UnitID)> callback);
    void SetOnCatDraggedOut(std::function<void(UnitID)> callback);

    // 🚀 取得正中央那張卡片的 ID (拖拉時通常是抓正中央那張)
    UnitID GetCenterCatID() const;
private:
    std::vector<std::shared_ptr<CatCardUI>> m_Cards; // 🚀 改成存放卡片組件

    std::function<void(UnitID)> m_OnCatSelected;
    std::function<void(UnitID)> m_OnSwapClick;

    // 滑動控制變數
    bool m_IsDragging = false;
    float m_LastMouseX = 0.0f;
    float m_ScrollX = 0.0f;

    // 排版參數
    float m_BaseYRatio = -0.55f; // 下方列表的 Y 軸比例
    float m_Spacing = 280.0f;    // 🚀 每張卡片的間距 (因為你的卡片變寬了，這裡要調大)
    std::function<void(UnitID)> m_OnCatDraggedOut;
    bool m_IsDraggingCatOut = false; // 是否進入了往上拖拉的模式
    float m_TargetScrollX = 0.0f;
    int m_CurrentCenterIndex = 0;
};

#endif