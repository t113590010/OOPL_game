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
#include <filesystem> // 👈 記得在檔案最上面引入這個
#include <iostream> // 👈 記得在檔案最上面引入這個
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

    // 預留返回按鈕
    std::shared_ptr<Button> m_ReturnBtn;
    std::function<void()> m_OnReturn;

    std::shared_ptr<Button> m_StorageBtn;     // 冰箱(儲藏庫)
    std::function<void()> m_OnStorageBtnClick;

    std::shared_ptr<Button> m_span;
    std::function<void()> m_Onspan;

    std::shared_ptr<Button> m_muti_span;
    std::function<void()> m_On_muti_span;
    std::shared_ptr<NumberSystem> m_XPNumber;
    std::shared_ptr<NumberSystem> m_CatFoodNumber; // 🥫 罐頭
    std::shared_ptr<NumberSystem> m_TicketNumber;  // 🎫 票卷
    // 這裡之後可以放你的貓咪網格、分頁按鈕等
    // 🪙 後台假資料 (用來測試運算邏輯)
    int m_Tickets = 1333;     // 👈 更改這個數字（例如改為 12、3、0）測試按鈕變化
    int m_CatFood = 139;
    int m_XP = 3000;

    // 🚀 新增：按鈕上面的消耗數字
    std::shared_ptr<NumberSystem> m_SpanCostNumber;     // 單抽消耗
    std::shared_ptr<NumberSystem> m_MutiSpanCostNumber; // 多抽消耗
    std::shared_ptr<Button> m_SpanCatFoodIcon;
    std::shared_ptr<Button> m_MutiSpanCatFoodIcon;
    // 🚀 扭蛋動畫與結果專用變數
    GachaState m_State = GachaState::IDLE;
    int m_AnimTimer = 0; // 用來簡單計時 (60 = 1秒)

    // 記錄這次抽到的貓咪 ID (為了支援十連抽，用 vector 裝)
    std::vector<UnitID> m_PulledCats;

    // 用來在畫面上顯示抽到的貓咪 (類似你冰箱的 Popup)
    std::vector<std::shared_ptr<Button>> m_ResultCatImages;
    std::vector<std::shared_ptr<Util::Image>> m_AnimFrames;
    Util::GameObject m_AnimObject; // 用來當作播放動畫的畫布
    int animSpeed = 4; // 👈 調整播放速度 (每 2 幀換一張圖，數字越大播越慢)
    Util::GameObject m_Background_black;
};

#endif