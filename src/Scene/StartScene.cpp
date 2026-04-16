#include "StartScene.hpp"
#include "GameConfig.hpp"

StartScene::StartScene() {
    // ==========================================
    // 1. 設定背景圖
    // ==========================================
    // 請把 RESOURCE_DIR"/img/start_bg.png" 換成你真正的主畫面背景圖
    auto bgImage = std::make_shared<Util::Image>(RESOURCE_DIR"/img/homeBackground.png");
    m_Background.SetDrawable(bgImage);
    m_Background.SetZIndex(10);
    // 🚀 召喚大管家，抓取最準確的視窗尺寸
    auto context = Core::Context::GetInstance();
    float windowWidth = (float)context->GetWindowWidth();
    float windowHeight = (float)context->GetWindowHeight();

    // 🚀 精準算出滿屏的倍率
    glm::vec2 imgSize = bgImage->GetSize();
    m_Background.m_Transform.scale = glm::vec2(windowWidth / imgSize.x, windowHeight / imgSize.y);
    // ==========================================
    // 2. 設定開始按鈕
    // ==========================================
    m_StartBtn = std::make_shared<Button>(
        0.0f, -0.2f,               // X 在正中間，Y 稍微偏下一點 (-0.2f)
        250.0f, 80.0f,             // 按鈕長寬
        RESOURCE_DIR"/img/battleStart.png", // 按鈕底圖
        " ", 40,          // 文字與大小
        Util::Color(255, 255, 255, 255)
    );
    m_StartBtn->SetZIndex(20); // 按鈕蓋在背景上面

    // 綁定點擊事件
    m_StartBtn->SetOnClick([this]() {
        if (m_OnStartGame) {
            m_OnStartGame(); // 大喊：「有人點開始遊戲了！」
        }
    });
}

void StartScene::SetOnStartGame(std::function<void()> callback) {
    m_OnStartGame = callback;
}

void StartScene::Update() {
    // 主畫面只需要更新按鈕的滑鼠偵測
    if (m_StartBtn) {
        m_StartBtn->Update();
    }
}

void StartScene::Draw() {
    // 先畫底圖，再畫按鈕
    m_Background.Draw();

    if (m_StartBtn) {
        m_StartBtn->Draw();
    }
}