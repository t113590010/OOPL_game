#ifndef DeckScene_HPP
#define DeckScene_HPP

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "Button.hpp"
#include <memory>
#include <functional>
#include "NumberSystem.hpp"
#include "DeckUI.hpp"
#include "SlotUI.hpp" // 假設你未來會建這個檔案
class DeckScene {
public:
    DeckScene();

    void Update();
    void Draw();

    // 綁定返回主畫面的事件
    void SetOnReturnBtnClick(std::function<void()> callback);
    void Refresh();
    // 幫助判斷落點的輔助函數
    int GetHoveredSlotIndex(glm::vec2 mousePos);
    void EquipCatToSlot(UnitID draggedCat, int targetSlotIndex);
private:
    Util::GameObject m_Background;

    // 預留返回按鈕
    std::shared_ptr<Button> m_ReturnBtn;
    std::function<void()> m_OnReturn;

    // 這裡之後可以放你的貓咪網格、分頁按鈕等
    // std::vector<std::shared_ptr<Button>> m_CatSlots;
    std::shared_ptr<NumberSystem> m_XPNumber;
    std::shared_ptr<NumberSystem> m_CatFoodNumber; // 🥫 罐頭
    int m_CatFood = 139;
    int m_XP = 3000;

    std::shared_ptr<DeckUI> m_DeckUI;
    std::shared_ptr<Button> m_LeftBorder;
    std::shared_ptr<Button> m_RightBorder;
    std::vector<std::shared_ptr<SlotUI>> m_Slots;
    int m_CurrentSelectedSlot = -1;
    bool m_IsDraggingGhost = false;
    int m_DraggedFromSlot = -1; // 🚀 新增：記錄是從哪一格抓起來的 (-1代表冰箱)
    UnitID m_DraggedUnitID = static_cast<UnitID>(0);
    std::shared_ptr<Util::GameObject> m_DragGhostIcon; // 跟著滑鼠跑的頭像


};

#endif