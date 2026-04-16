#include "System/PauseMenu.hpp"
#include "Core/Context.hpp"
#include "GameConfig.hpp"

PauseMenu::PauseMenu() {


    // 🚀 調整1：把 Y 軸往下拉到 -0.25f (保持對稱)
    m_QuitBtn = std::make_shared<Button>(
        0.0f, -0.15f,
        230.0f, 70.0f,
        RESOURCE_DIR"/img/escapeBattle.png",
        " ", 30,
        Util::Color(255, 0, 0, 255)
    );

    m_QuitBtn->SetOnClick([this]() {
        if (m_OnQuit) m_OnQuit();
    });

    m_Background.SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR"/img/black.png"));

    // 2. 把它拉大到蓋滿全螢幕 (假設畫面是 1280x720)
    // 因為 DrawRect 是切一小塊，所以這裡的 scale 設大一點沒關係
    m_Background.m_Transform.scale = glm::vec2(1280.0f, 720.0f);

    m_QuitBtn->SetZIndex(50);


    m_Background.SetZIndex(49);

}

void PauseMenu::Update() {

    if (m_QuitBtn) m_QuitBtn->Update();
}


void PauseMenu::Draw() {
    float cutX = 0.0f;
    float cutY = 0.0f;
    float cutW = 10.0f;
    float cutH = 10.0f;
    m_Background.DrawRect(cutX, cutY, cutW, cutH);

    if (m_QuitBtn) m_QuitBtn->Draw();
}