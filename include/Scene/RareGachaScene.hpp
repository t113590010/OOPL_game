#ifndef Rare_GachaScene_HPP
#define Rare_GachaScene_HPP

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "Button.hpp"
#include <memory>
#include <functional>
#include "NumberSystem.hpp"

class RareGachaScene {
public:
    RareGachaScene();

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
    int m_Tickets = 3;     // 👈 更改這個數字（例如改為 12、3、0）測試按鈕變化
    int m_CatFood = 139;
    int m_XP = 3000;

    // 🚀 新增：按鈕上面的消耗數字
    std::shared_ptr<NumberSystem> m_SpanCostNumber;     // 單抽消耗
    std::shared_ptr<NumberSystem> m_MutiSpanCostNumber; // 多抽消耗
    std::shared_ptr<Button> m_SpanCatFoodIcon;
    std::shared_ptr<Button> m_MutiSpanCatFoodIcon;
};

#endif