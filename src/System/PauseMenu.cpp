#include "System/PauseMenu.hpp"
#include "Core/Context.hpp"
#include "GameConfig.hpp"

PauseMenu::PauseMenu() {
    // 🚀 調整1：把 Y 軸拉高到 0.25f (大約在畫面上半部 1/4 處)
    // 🚀 調整2：寬度變 250，高度變 90
    m_ResumeBtn = std::make_shared<Button>(
        0.0f, 0.15f,
        120.0f, 30.0f,
        RESOURCE_DIR"/img/slot_frame.png",
        "Continue", 30, // 字體從 24 放大到 30
        Util::Color(255, 255, 0, 255)
    );

    m_ResumeBtn->SetOnClick([this]() {
        if (m_OnResume) m_OnResume();
    });

    // 🚀 調整1：把 Y 軸往下拉到 -0.25f (保持對稱)
    m_QuitBtn = std::make_shared<Button>(
        0.0f, -0.15f,
        120.0f, 30.0f,
        RESOURCE_DIR"/img/slot_frame.png",
        "Quit", 30,
        Util::Color(255, 0, 0, 255)
    );

    m_QuitBtn->SetOnClick([this]() {
        if (m_OnQuit) m_OnQuit();
    });
}

void PauseMenu::Update() {
    // 讓選單裡的按鈕也能吃滑鼠判定
    if (m_ResumeBtn) m_ResumeBtn->Update();
    if (m_QuitBtn) m_QuitBtn->Update();
}

void PauseMenu::Draw() {
    // 把按鈕畫出來
    if (m_ResumeBtn) m_ResumeBtn->Draw();
    if (m_QuitBtn) m_QuitBtn->Draw();
}