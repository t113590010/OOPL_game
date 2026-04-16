#include "HomeScene.hpp"

#include <X11/X.h>
struct SpriteFrame { int x, y, w, h; };
namespace Cut {
    const float FRAME_X = 255.0f, FRAME_Y = 207.0f, FRAME_W = 260.0f, FRAME_H = 50.0f;
    const float TEAM_X = 54.0f, TEAM_Y = 93.0f, TEAM_W = 163.0f, TEAM_H = 50.0f;
    const float UPGRADE_X = 85.0f, UPGRADE_Y = 208.0f, UPGRADE_W = 85.0f, UPGRADE_H = 43.0f;
}
HomeScene::HomeScene() {
    auto bgImage = std::make_shared<Util::Image>(RESOURCE_DIR"/img/homeBackground.png");
    m_Background.SetDrawable(bgImage);
    m_Background.SetZIndex(10); // 背景放最底下

    auto context = Core::Context::GetInstance();
    float windowWidth = (float)context->GetWindowWidth();
    float windowHeight = (float)context->GetWindowHeight();
    glm::vec2 imgSize = bgImage->GetSize();
    // 讓電腦自動算比例，完美塞滿螢幕
    m_Background.m_Transform.scale = glm::vec2(windowWidth / imgSize.x, windowHeight / imgSize.y);

    // 建立開始按鈕 (假設放在畫面正中央)
    float offset_x = -0.5;
    float offset_y = 0.1;
    float height =80.f;
    float width = 400.f;
    float difY = -0.4;

    m_StartBtn = std::make_shared<Button>(
        offset_x, offset_y,
         width,height,
        RESOURCE_DIR"/img/battleStart.png",
        " ", 30,
        Util::Color(255, 255, 255, 255)
    );

    // 將按鈕點擊事件對接到內部的 m_OnStart
    m_StartBtn->SetOnClick([this]() {
        if (m_OnStart) {
            m_OnStart();
        }
    });

    const std::string atlasPath = RESOURCE_DIR"/img/img010_tw.png";
    // ⬆️ 升級按鈕
    m_UpgradeBtn = std::make_shared<Button>(offset_x, offset_y + difY, width, height, atlasPath, " ", 30, Util::Color(255, 255, 255, 255));
    m_UpgradeBtn->SetZIndex(20);
    m_UpgradeBtn->SetOnClick([this]() { if (m_OnUpgradeBtnClick) m_OnUpgradeBtnClick(); });

    // 🛡️ 隊伍按鈕
    m_TeamBtn = std::make_shared<Button>(offset_x, offset_y + difY * 2, width, height, atlasPath, " ", 30, Util::Color(255, 255, 255, 255));
    m_TeamBtn->SetZIndex(20);
    m_TeamBtn->SetOnClick([this]() { if (m_OnTeamBtnClick) m_OnTeamBtnClick(); });
}

void HomeScene::SetOnStartBtnClick(std::function<void()> callback) {
    m_OnStart = callback;
}
void HomeScene::SetOnUpgradeBtnClick(std::function<void()> callback) { m_OnUpgradeBtnClick = callback; }
void HomeScene::SetOnTeamBtnClick(std::function<void()> callback) { m_OnTeamBtnClick = callback; }
void HomeScene::Update() {
    if (m_StartBtn) {
        m_StartBtn->Update();
    }
    if (m_UpgradeBtn) m_UpgradeBtn->Update();
    if (m_TeamBtn) m_TeamBtn->Update();
}

void HomeScene::Draw() {
    m_Background.Draw();

    if (m_StartBtn) m_StartBtn->Draw();
    // 💡 取得合圖大小做比例抵銷 (照你的 Entity 邏輯)
    static auto uiAtlas = std::make_shared<Util::Image>(RESOURCE_DIR"/img/img010_tw.png");
    glm::vec2 sheetSize = uiAtlas->GetSize();

    // ==========================================
    // 🚀 不准呼叫 m_UpgradeBtn->Draw()！
    // 我們改用你的 DrawRect 直接切圖畫底框和文字
    // ==========================================
    float baseScale = 1.5f;   // 👈 調這個，數值越小按鈕越小 (試試 0.2 ~ 0.5)
    float textBoost = 0.5f;   // 👈 這是文字相對於底框的放大倍率，讓字大一點點

    if (m_UpgradeBtn) {
        // 1. 底框縮放
        m_UpgradeBtn->m_Transform.scale = {
            ((float)Cut::FRAME_W / sheetSize.x) * baseScale,
            ((float)Cut::FRAME_H / sheetSize.y) * baseScale
        };
        m_UpgradeBtn->SetZIndex(21);
        m_UpgradeBtn->DrawRect(Cut::FRAME_X, Cut::FRAME_Y, Cut::FRAME_W, Cut::FRAME_H);

        // 2. 文字縮放 (疊上去)
        m_UpgradeBtn->m_Transform.scale = {
            ((float)Cut::UPGRADE_W / sheetSize.x) * baseScale * textBoost,
            ((float)Cut::UPGRADE_H / sheetSize.y) * baseScale * textBoost
        };
        m_UpgradeBtn->SetZIndex(22);
        m_UpgradeBtn->DrawRect(Cut::UPGRADE_X, Cut::UPGRADE_Y, Cut::UPGRADE_W, Cut::UPGRADE_H);
    }

    if (m_TeamBtn) {
        // 1. 底框縮放
        m_TeamBtn->m_Transform.scale = {
            ((float)Cut::FRAME_W / sheetSize.x) * baseScale,
            ((float)Cut::FRAME_H / sheetSize.y) * baseScale
        };
        m_TeamBtn->SetZIndex(21);
        m_TeamBtn->DrawRect(Cut::FRAME_X, Cut::FRAME_Y, Cut::FRAME_W, Cut::FRAME_H);

        // 2. 文字縮放
        m_TeamBtn->m_Transform.scale = {
            ((float)Cut::TEAM_W / sheetSize.x) * baseScale * textBoost,
            ((float)Cut::TEAM_H / sheetSize.y) * baseScale * textBoost
        };
        m_TeamBtn->SetZIndex(22);
        m_TeamBtn->DrawRect(Cut::TEAM_X, Cut::TEAM_Y, Cut::TEAM_W, Cut::TEAM_H);
    }
}