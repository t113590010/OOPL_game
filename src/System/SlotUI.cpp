#include "SlotUI.hpp"
#include "Core/Context.hpp"
#include "PlayerData.hpp"

SlotUI::SlotUI(int index, float pos_ratio_x, float pos_ratio_y)
    : m_Index(index) {

    // 建立格子底圖 (使用 img029.png)
    m_SlotBtn = std::make_shared<Button>(
        pos_ratio_x, pos_ratio_y, m_Width, m_Height,
        RESOURCE_DIR"/img/img029.png", " ", 20, Util::Color(0,0,0,0)
    );
    m_SlotBtn->SetZIndex(0); // Z-Index 放在背景(10)之上，邊框(80)之下
    m_BaseCenter = m_SlotBtn->m_Transform.translation;

    // 貓咪圖示與等級 (預設不顯示，直到呼叫 SetUnit 才生出來)
    m_CatIcon = nullptr;
    // m_CatIcon->SetZIndex(50);
    m_LevelNumber = nullptr;
}

void SlotUI::SetUnit(UnitID id) {
    m_UnitID = id;

    // 如果傳入的是空 ID，清空這個格子的貓咪
    if (static_cast<int>(id) == 0) {
        m_CatIcon = nullptr;
        m_LevelNumber = nullptr;
        return;
    }

    // 讀取這隻貓的資料
    const UnitStats& stats = UnitData::Get(id);
    int currentLevel = PlayerData::GetInstance()->GetCatLevel(id).base;
    if (currentLevel == 0) currentLevel = 1;

    // 生成頭像
    m_CatIcon = std::make_shared<Util::GameObject>(std::make_unique<Util::Image>(stats.iconPath), 16);
    m_CatIcon->m_Transform.scale = { 0.8f, 0.8f }; // TODO: 依照格子大小調整縮放
    m_CatIcon->m_Transform.translation = m_BaseCenter + OFFSET_ICON;

    // 生成等級數字
    m_LevelNumber = std::make_shared<NumberSystem>(0.0f, 0.0f, 15.0f, 20.0f, RESOURCE_DIR"/img/moneyInfo.png");
    m_LevelNumber->SetPosition(m_BaseCenter.x + OFFSET_LEVEL.x, m_BaseCenter.y + OFFSET_LEVEL.y);
    m_LevelNumber->SetValue(std::to_string(stats.cost)+"$");
}

void SlotUI::Update() {
    if (m_SlotBtn) m_SlotBtn->Update();
}

void SlotUI::Draw() {
    if (m_SlotBtn) {
        float w = 113.0f;
        float h = 91.0f;       // 932 - 841 = 91

        // 🚀 真相大白：步長真的是 143，但起點是 3！
        float startX = 3.0f;   // 第一格前面的空白邊界
        float stepX = 146.0f;  // 每個格子的距離
        float stepY = 92.0f;   // 第二排與第一排的 Y 距離

        // 加上 startX，一切數學就完美吻合了！
        float clipX = startX + (m_Index % 5) * stepX;
        float clipY = 841.0f + (m_Index / 5) * stepY;

        m_SlotBtn->DrawRect(clipX, clipY, w, h);
    }

    if (m_CatIcon) m_CatIcon->Draw();
    if (m_LevelNumber) m_LevelNumber->Draw();

    // 🚀 如果被選中，可以在這裡疊加畫一個發光框框
    if (m_IsSelected) {
        // TODO: 之後有發光框框的圖，可以加在這裡 Draw出來
    }
}

void SlotUI::SetOnClick(std::function<void(int)> callback) {
    if (m_SlotBtn) {
        m_SlotBtn->SetOnClick([this, callback]() {
            callback(m_Index);
        });
    }
}