#include "HomeScene.hpp"


struct SpriteFrame { int x, y, w, h; };
namespace Cut {
    const float FRAME_X = 255.0f, FRAME_Y = 207.0f, FRAME_W = 260.0f, FRAME_H = 50.0f;
    const float TEAM_X = 54.0f, TEAM_Y = 93.0f, TEAM_W = 163.0f, TEAM_H = 50.0f;
    const float UPGRADE_X = 85.0f, UPGRADE_Y = 208.0f, UPGRADE_W = 85.0f, UPGRADE_H = 43.0f;

    // 👇 冰箱 (img010)
    const float STORAGE_X = 0.0f, STORAGE_Y = 0.0f, STORAGE_W = 72.0f, STORAGE_H = 82.0f;

    // 👇 扭蛋 (img007)
    const float RARE_GACHA_X = 116.0f, RARE_GACHA_Y = 262.0f, RARE_GACHA_W = 99.0f, RARE_GACHA_H = 85.0f;
    const float NORMAL_GACHA_X = 215.0f, NORMAL_GACHA_Y = 262.0f, NORMAL_GACHA_W = 99.0f, NORMAL_GACHA_H = 85.0f;
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
    const std::string gachaPath = RESOURCE_DIR"/img/img007_tw.png";
    // ⬆️ 升級按鈕
    m_UpgradeBtn = std::make_shared<Button>(offset_x, offset_y + difY, width, height, atlasPath, " ", 30, Util::Color(255, 255, 255, 255));
    m_UpgradeBtn->SetZIndex(20);
    m_UpgradeBtn->SetOnClick([this]() { if (m_OnUpgradeBtnClick) m_OnUpgradeBtnClick(); });

    // 🛡️ 隊伍按鈕
    m_TeamBtn = std::make_shared<Button>(offset_x, offset_y + difY * 2, width, height, atlasPath, " ", 30, Util::Color(255, 255, 255, 255));
    m_TeamBtn->SetZIndex(20);
    m_TeamBtn->SetOnClick([this]() { if (m_OnTeamBtnClick) m_OnTeamBtnClick(); });

    // 🧊 冰箱按鈕 (維持 atlasPath)
    m_StorageBtn = std::make_shared<Button>(0.6, -0.65 , width, height, atlasPath, " ", 30, Util::Color(255, 255, 255, 255));
    m_StorageBtn->SetZIndex(20);
    m_StorageBtn->SetOnClick([this]() { if (m_OnStorageBtnClick) m_OnStorageBtnClick(); });

    // 🎰 稀有轉蛋按鈕 (改吃 gachaPath ！)
    m_RareGachaBtn = std::make_shared<Button>(0.82, -0.65, width, height, gachaPath, " ", 30, Util::Color(255, 255, 255, 255));
    m_RareGachaBtn->SetZIndex(20);
    m_RareGachaBtn->SetOnClick([this]() { if (m_OnRareGachaBtnClick) m_OnRareGachaBtnClick(); });

    // 🎰 貓咪轉蛋按鈕 (改吃 gachaPath ！)
    m_NormalGachaBtn = std::make_shared<Button>(0.71, -0.65 , width, height, gachaPath, " ", 30, Util::Color(255, 255, 255, 255));
    m_NormalGachaBtn->SetZIndex(20);
    m_NormalGachaBtn->SetOnClick([this]() { if (m_OnNormalGachaBtnClick) m_OnNormalGachaBtnClick(); });
}

void HomeScene::SetOnStartBtnClick(std::function<void()> callback) {
    m_OnStart = callback;
}
void HomeScene::SetOnUpgradeBtnClick(std::function<void()> callback) { m_OnUpgradeBtnClick = callback; }
void HomeScene::SetOnTeamBtnClick(std::function<void()> callback) { m_OnTeamBtnClick = callback; }
void HomeScene::SetOnStorageBtnClick(std::function<void()> callback) { m_OnStorageBtnClick = callback; }
void HomeScene::SetOnRareGachaBtnClick(std::function<void()> callback) { m_OnRareGachaBtnClick = callback; }
void HomeScene::SetOnNormalGachaBtnClick(std::function<void()> callback) { m_OnNormalGachaBtnClick = callback; }
void HomeScene::Update() {
    if (m_StartBtn) {
        m_StartBtn->Update();
    }
    if (m_UpgradeBtn) m_UpgradeBtn->Update();
    if (m_TeamBtn) m_TeamBtn->Update();
    if (m_StorageBtn) m_StorageBtn->Update();
    if (m_RareGachaBtn) m_RareGachaBtn->Update();
    if (m_NormalGachaBtn) m_NormalGachaBtn->Update();
}

void HomeScene::Draw() {
    m_Background.Draw();

    if (m_StartBtn) m_StartBtn->Draw();
    // 💡 取得合圖大小做比例抵銷 (照你的 Entity 邏輯)
    static auto uiAtlas = std::make_shared<Util::Image>(RESOURCE_DIR"/img/img010_tw.png");
    static auto gachaAtlas = std::make_shared<Util::Image>(RESOURCE_DIR"/img/img007_tw.png");
    glm::vec2 sheetSize = uiAtlas->GetSize();
    glm::vec2 gachaSheetSize = gachaAtlas->GetSize();
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

        // 🚀 3. 歸位！把 scale 撐回大底框的大小，碰撞判定才會正常！
        m_UpgradeBtn->m_Transform.scale = {
            ((float)Cut::FRAME_W / sheetSize.x) * baseScale,
            ((float)Cut::FRAME_H / sheetSize.y) * baseScale
        };
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

        // 🚀 3. 歸位！把 scale 撐回大底框的大小，碰撞判定才會正常！
        m_TeamBtn->m_Transform.scale = {
            ((float)Cut::FRAME_W / sheetSize.x) * baseScale,
            ((float)Cut::FRAME_H / sheetSize.y) * baseScale
        };
    }


    // 🧊 畫冰箱 (完全使用 img010)
    if (m_StorageBtn) {
        //底層
        // 👉 1. 記錄原本座標
        glm::vec2 pos = m_StorageBtn->m_Transform.translation;

        // 👉 2. 直接改這裡！調整底框的 XY 偏移量 (例如 X往左10，Y往下5)
        m_StorageBtn->m_Transform.translation += glm::vec2(+70.0f, -0.0f);

        m_StorageBtn->m_Transform.scale = { ((float)Cut::FRAME_W / sheetSize.x) * 0.9f, ((float)Cut::FRAME_H / sheetSize.y) * baseScale };
        m_StorageBtn->SetZIndex(21);
        m_StorageBtn->DrawRect(Cut::FRAME_X, Cut::FRAME_Y, Cut::FRAME_W, Cut::FRAME_H);

        // 👉 3. 畫完底框馬上歸位！(不然你上面疊的字跟滑鼠點擊判定會歪掉)
        m_StorageBtn->m_Transform.translation = pos;
        //冰箱
        m_StorageBtn->m_Transform.scale = { ((float)Cut::STORAGE_W / sheetSize.x) * 0.8f, ((float)Cut::STORAGE_H / sheetSize.y) * 0.8f };
        m_StorageBtn->SetZIndex(22);
        m_StorageBtn->DrawRect(Cut::STORAGE_X, Cut::STORAGE_Y, Cut::STORAGE_W, Cut::STORAGE_H);
    }

    // 🎰 畫稀有轉蛋 (文字 img007)
    // 🎰 畫稀有轉蛋 (不需要 SetImage 了)
    if (m_RareGachaBtn) {
        m_RareGachaBtn->m_Transform.scale = {
            ((float)Cut::RARE_GACHA_W / gachaSheetSize.x) * 0.75,
            ((float)Cut::RARE_GACHA_H / gachaSheetSize.y) * 0.75
        };
        m_RareGachaBtn->SetZIndex(22);
        m_RareGachaBtn->DrawRect(Cut::RARE_GACHA_X, Cut::RARE_GACHA_Y, Cut::RARE_GACHA_W, Cut::RARE_GACHA_H);
    }

    // 🎰 畫貓咪轉蛋 (不需要 SetImage 了)
    if (m_NormalGachaBtn) {
        m_NormalGachaBtn->m_Transform.scale = {
            ((float)Cut::NORMAL_GACHA_W / gachaSheetSize.x) * 0.75 ,
            ((float)Cut::NORMAL_GACHA_H / gachaSheetSize.y) * 0.75
        };
        m_NormalGachaBtn->SetZIndex(22);
        m_NormalGachaBtn->DrawRect(Cut::NORMAL_GACHA_X, Cut::NORMAL_GACHA_Y, Cut::NORMAL_GACHA_W, Cut::NORMAL_GACHA_H);
    }
}