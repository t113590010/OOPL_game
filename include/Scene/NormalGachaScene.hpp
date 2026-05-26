#ifndef Normal_GachaScene_HPP
#define Normal_GachaScene_HPP

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "Button.hpp"
#include <memory>
#include <functional>
#include "NumberSystem.hpp"
#include <vector>
#include "UnitData.hpp"
#include "StorageItem.hpp"
#include <filesystem>
#include <iostream>

class NormalGachaScene {
public:
    enum class GachaState {
        IDLE,       // 閒置 (可點擊按鈕)
        ANIMATING,  // 播放動畫中 (鎖定按鈕)
        RESULT      // 顯示抽卡結果
    };

    NormalGachaScene();

    void Update();
    void Draw();
    void UpdateGachaButtons();

    // 綁定返回主畫面的事件
    void SetOnReturnBtnClick(std::function<void()> callback);
    void SetOnStorageBtnClick(std::function<void()> callback);
    void SetOnspanClick(std::function<void()> callback);
    void SetOn_muti_spanClick(std::function<void()> callback);

private:
    Util::GameObject m_Background;

    // 預留按鈕與回調
    std::shared_ptr<Button> m_ReturnBtn;
    std::function<void()> m_OnReturn;
    std::shared_ptr<Button> m_StorageBtn;
    std::function<void()> m_OnStorageBtnClick;
    std::shared_ptr<Button> m_span;
    std::function<void()> m_Onspan;
    std::shared_ptr<Button> m_muti_span;
    std::function<void()> m_On_muti_span;

    std::shared_ptr<NumberSystem> m_XPNumber;
    std::shared_ptr<NumberSystem> m_CatFoodNumber; // 🥫 罐頭
    std::shared_ptr<NumberSystem> m_TicketNumber;  // 🎫 票卷

    // 🪙 後台資源假資料
    int m_Tickets = 100;
    int m_CatFood = 139;
    int m_XP = 3000;

    // 🚀 按鈕上面的消耗數字
    std::shared_ptr<NumberSystem> m_SpanCostNumber;
    std::shared_ptr<NumberSystem> m_MutiSpanCostNumber;
    std::shared_ptr<Button> m_SpanCatFoodIcon;
    std::shared_ptr<Button> m_MutiSpanCatFoodIcon;

    // 🚀 狀態機變數
    GachaState m_State = GachaState::IDLE;
    int m_AnimTimer = 0;
    std::vector<UnitID> m_PulledCats; // 儲存抽到的貓咪序列

    std::vector<std::shared_ptr<Util::Image>> m_AnimFrames;
    Util::GameObject m_AnimObject;
    int animSpeed = 4;
    Util::GameObject m_Background_black;

    // ==========================================
    // 🚀 彈窗序列系統專屬變數 (完美對齊你的需求)
    // ==========================================
    int m_CurrentResultIndex = 0; // 記錄現在播到第幾隻貓
    std::shared_ptr<StorageItem> m_SelectedPopupCat; // 目前彈窗正中央顯示的貓咪

    // 彈窗內放大顯示巨神貓的 UI 部件 (如果 StorageItem 內部沒有接管繪製，我們在場景手動繪製防呆)
    std::shared_ptr<Button> m_PopupDisplayBody;
    std::shared_ptr<Button> m_PopupDisplayLegs;

    void ShowNextResultCat(); // 核心：載入下一隻抽到的貓

    bool m_IsPopupOpen = false;
    std::shared_ptr<Button> m_PopupUseBtn;
    std::shared_ptr<Button> m_PopupXpBtn;
    std::shared_ptr<Button> m_PopupCancelBtn;
};

#endif