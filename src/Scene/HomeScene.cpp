#include "HomeScene.hpp"

HomeScene::HomeScene() {
    // 建立開始按鈕 (假設放在畫面正中央)
    m_StartBtn = std::make_shared<Button>(
        0.0f, 0.0f,
        300.0f, 100.0f,
        RESOURCE_DIR"/img/slot_frame.png",
        "START BATTLE", 30,
        Util::Color(255, 255, 255, 255)
    );

    // 將按鈕點擊事件對接到內部的 m_OnStart
    m_StartBtn->SetOnClick([this]() {
        if (m_OnStart) {
            m_OnStart();
        }
    });
}

void HomeScene::SetOnStartBtnClick(std::function<void()> callback) {
    m_OnStart = callback;
}

void HomeScene::Update() {
    if (m_StartBtn) {
        m_StartBtn->Update();
    }
}

void HomeScene::Draw() {
    if (m_StartBtn) {
        m_StartBtn->Draw();
    }
}