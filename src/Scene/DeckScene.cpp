#include "DeckScene.hpp"
#include "Core/Context.hpp"

// 如果有專屬的切圖座標，可以建一個獨立的 namespace
namespace StorageCut {
    // const float CAT_FRAME_X = ...;
}

DeckScene::DeckScene() {
    // 1. 設定冰箱背景 (路徑請換成你實際的背景圖)
    auto bgImage = std::make_shared<Util::Image>(RESOURCE_DIR"/img/homeBackground.png");
    m_Background.SetDrawable(bgImage);
    m_Background.SetZIndex(10);

    auto context = Core::Context::GetInstance();
    float windowWidth = (float)context->GetWindowWidth();
    float windowHeight = (float)context->GetWindowHeight();
    glm::vec2 imgSize = bgImage->GetSize();

    // 自動算比例塞滿螢幕
    m_Background.m_Transform.scale = glm::vec2(windowWidth / imgSize.x, windowHeight / imgSize.y);

    // 2. 建立返回按鈕 (假設放在左上角)
    // 這裡先借用你的 battleStart 圖，之後你可以換成專用的返回圖示或合圖切圖
    m_ReturnBtn = std::make_shared<Button>(
        -0.8f, 0.8f,     // 左上角
        150.0f, 60.0f,   // 寬高
        RESOURCE_DIR"/img/battleStart.png",
        " ", 30,
        Util::Color(255, 255, 255, 255)
    );
    m_ReturnBtn->SetZIndex(20);

    // 對接點擊事件
    m_ReturnBtn->SetOnClick([this]() {
        if (m_OnReturn) {
            m_OnReturn();
        }
    });
}

void DeckScene::SetOnReturnBtnClick(std::function<void()> callback) {
    m_OnReturn = callback;
}

void DeckScene::Update() {
    if (m_ReturnBtn) {
        m_ReturnBtn->Update();
    }
    // 之後這裡放其他冰箱內按鈕的 Update
}

void DeckScene::Draw() {
    m_Background.Draw();
        LOG_DEBUG("StartLevelUpgradeScene cpp！");

    if (m_ReturnBtn) {
        m_ReturnBtn->Draw();
    }
    // 之後這裡放其他冰箱內按鈕的 Draw (如果要用 DrawRect 疊圖一樣寫在這裡)
}