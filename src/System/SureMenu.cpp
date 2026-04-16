#include "SureMenu.hpp"

SureMenu::SureMenu() {
    // 建立「是」按鈕 (放在畫面中央偏左)
    // 參數請根據你實際的解析度跟 Button 寫法微調
    m_YesBtn = std::make_shared<Button>(
        -0.15f, 0.0f,           // X 往左推一點, Y 在中間
        120.0f, 120.0f,         // 寬高
        RESOURCE_DIR"/img/yes.png", // 你的黃色「是」圖片
        " ", 3, Util::Color(255, 255, 255, 255), // 不需要額外加文字
        0.0f, 0.0f
    );

    // 建立「否」按鈕 (放在畫面中央偏右)
    m_NoBtn = std::make_shared<Button>(
        0.15f, 0.0f,            // X 往右推一點, Y 在中間
        120.0f, 120.0f,
        RESOURCE_DIR"/img/no.png", // 你的黃色「否」圖片
        " ", 3, Util::Color(255, 255, 255, 255),
        0.0f, 0.0f
    );

    // 綁定點擊事件
    m_YesBtn->SetOnClick([this]() {
        if (m_OnConfirm) m_OnConfirm();
    });

    m_NoBtn->SetOnClick([this]() {
        if (m_OnCancel) m_OnCancel();
    });
    m_Background.SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR"/img/black.png"));

    // 2. 把它拉大到蓋滿全螢幕 (假設畫面是 1280x720)
    // 因為 DrawRect 是切一小塊，所以這裡的 scale 設大一點沒關係
    m_Background.m_Transform.scale = glm::vec2(1280.0f, 720.0f);
    m_NoBtn->SetZIndex(52);
    m_YesBtn->SetZIndex(52);
    m_Background.SetZIndex(51);
}

void SureMenu::SetOnConfirm(std::function<void()> callback) {
    m_OnConfirm = callback;
}

void SureMenu::SetOnCancel(std::function<void()> callback) {
    m_OnCancel = callback;
}

void SureMenu::Update() {
    if (m_YesBtn) m_YesBtn->Update();
    if (m_NoBtn) m_NoBtn->Update();
}

void SureMenu::Draw() {
    float cutX = 0.0f;
    float cutY = 0.0f;
    float cutW = 10.0f;
    float cutH = 10.0f;
    // m_Background.DrawRect(cutX, cutY, cutW, cutH);
    if (m_YesBtn) m_YesBtn->Draw();
    if (m_NoBtn) m_NoBtn->Draw();
}