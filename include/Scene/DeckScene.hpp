#ifndef DeckScene_HPP
#define DeckScene_HPP

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "Button.hpp"
#include <memory>
#include <functional>

class DeckScene {
public:
    DeckScene();

    void Update();
    void Draw();

    // 綁定返回主畫面的事件
    void SetOnReturnBtnClick(std::function<void()> callback);

private:
    Util::GameObject m_Background;

    // 預留返回按鈕
    std::shared_ptr<Button> m_ReturnBtn;
    std::function<void()> m_OnReturn;

    // 這裡之後可以放你的貓咪網格、分頁按鈕等
    // std::vector<std::shared_ptr<Button>> m_CatSlots;
};

#endif