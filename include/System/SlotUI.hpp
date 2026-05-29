#ifndef SLOT_UI_HPP
#define SLOT_UI_HPP

#include <memory>
#include <functional>
#include "Button.hpp"
#include "Entity/UnitData.hpp"
#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "NumberSystem.hpp"

class SlotUI {
public:
    // 傳入 index (0~9) 讓我們知道這是第幾個格子，以及它的螢幕比例座標
    SlotUI(int index, float pos_ratio_x, float pos_ratio_y);

    void Update();
    void Draw();

    void SetOnClick(std::function<void(int)> callback);

    // 🚀 核心功能：把貓咪裝進這個格子裡
    void SetUnit(UnitID id);
    UnitID GetUnitID() const { return m_UnitID; }

    // 🚀 核心功能：標記這個格子目前是否被玩家點擊選中了
    void SetSelected(bool selected) { m_IsSelected = selected; }
    bool IsSelected() const { return m_IsSelected; }
    float GetBaseX() const { return m_SlotBtn->m_Transform.translation.x; }
    float GetBaseY() const { return m_SlotBtn->m_Transform.translation.y; }
private:
    int m_Index;
    UnitID m_UnitID = static_cast<UnitID>(0); // 預設 0 代表沒有裝備貓咪
    bool m_IsSelected = false;

    float m_Width = 82.0f;  // TODO: 依照你實際切出來的格子大小微調
    float m_Height = 30.0f;
    glm::vec2 m_BaseCenter;

    std::shared_ptr<Button> m_SlotBtn; // 格子底圖 (負責判定點擊與切圖)
    std::shared_ptr<Util::GameObject> m_CatIcon; // 裝備的貓咪頭像
    std::shared_ptr<NumberSystem> m_LevelNumber; // 貓咪的等級

    // 相對偏移量
    const glm::vec2 OFFSET_ICON = { 0.0f, 0.0f };
    const glm::vec2 OFFSET_LEVEL = { 20.0f, -30.0f };
};

#endif