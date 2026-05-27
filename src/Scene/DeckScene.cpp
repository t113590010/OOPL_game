#include "DeckScene.hpp"
#include "Core/Context.hpp"

// 如果有專屬的切圖座標，可以建一個獨立的 namespace
#include "PlayerData.hpp"

namespace Cut {
    const float RETURN_ICON_X = 423.0f;
    const float RETURN_ICON_Y = 170.0f;
    const float RETURN_ICON_W = 511.0f - RETURN_ICON_X;
    const float RETURN_ICON_H = 232.0f - RETURN_ICON_Y;

    const float RETURN_BASE_X = 256.0f;
    const float RETURN_BASE_Y = 0.0f;
    const float RETURN_BASE_W = 99.0f;
    const float RETURN_BASE_H = 97.0f;

    const float STORAGE_X = 0.0f, STORAGE_Y = 0.0f, STORAGE_W = 72.0f, STORAGE_H = 82.0f;

    const float SPAN_BTN_X = 297.0f;
    const float SPAN_BTN_Y = 426.0f;
    const float SPAN_BTN_W = 511.0f - SPAN_BTN_X;
    const float SPAN_BTN_H = 508.0f - SPAN_BTN_Y;

    const float CATFOOD_ICON_X = 161.0f;
    const float CATFOOD_ICON_Y = 158.0f;
    const float CATFOOD_ICON_W = 219.0f - CATFOOD_ICON_X;
    const float CATFOOD_ICON_H = 203.0f - CATFOOD_ICON_Y;

    const float POPUP_BTN_BASE_X = 255.0f, POPUP_BTN_BASE_Y = 207.0f, POPUP_BTN_BASE_W = 260.0f, POPUP_BTN_BASE_H = 50.0f;

    const float POPUP_USE_TXT_X = 0.0f, POPUP_USE_TXT_Y = 0.0f, POPUP_USE_TXT_W = 100.0f, POPUP_USE_TXT_H = 40.0f;
    const float POPUP_XP_TXT_X = 0.0f, POPUP_XP_TXT_Y = 0.0f, POPUP_XP_TXT_W = 100.0f, POPUP_XP_TXT_H = 40.0f;
    const float POPUP_CANCEL_TXT_X = 446.0f, POPUP_CANCEL_TXT_Y = 0.0f, POPUP_CANCEL_TXT_W = 512.0f - POPUP_CANCEL_TXT_X, POPUP_CANCEL_TXT_H = 63.0f;
}

DeckScene::DeckScene() {
    auto playerData = PlayerData::GetInstance();
    m_XP = playerData->GetXP();
    m_CatFood = playerData->GetCatFood();

    // auto bgImage = std::make_shared<Util::Image>(RESOURCE_DIR"/img/StorageScene.png");
    auto bgImage = std::make_shared<Util::Image>(RESOURCE_DIR"/img/DeckSenceClear.png");

    m_Background.SetDrawable(bgImage);
    m_Background.SetZIndex(10);

    auto context = Core::Context::GetInstance();
    float windowWidth = (float)context->GetWindowWidth();
    float windowHeight = (float)context->GetWindowHeight();
    glm::vec2 imgSize = bgImage->GetSize();

    m_Background.m_Transform.scale = glm::vec2(windowWidth / imgSize.x, windowHeight / imgSize.y);
    // auto blackImg = std::make_shared<Util::Image>(RESOURCE_DIR "/img/black.png");
    // m_Background_black.SetDrawable(blackImg);
    // m_Background_black.SetZIndex(49);
    // m_Background_black.m_Transform.scale = glm::vec2(windowWidth / blackImg->GetSize().x, windowHeight / blackImg->GetSize().y);

    const std::string img006Path = RESOURCE_DIR"/img/img006_tw.png";
    const std::string img007Path = RESOURCE_DIR"/img/img007_tw.png";
    const std::string yesOrNoPath = RESOURCE_DIR"/img/yesOrNo.png";

    float btnSize = 100.0f;

    m_ReturnBtn = std::make_shared<Button>(
        -0.8f, -0.87f, btnSize, btnSize,
        img006Path, " ", 30, Util::Color(255, 255, 255, 255)
    );
    m_ReturnBtn->SetZIndex(20);
    m_ReturnBtn->SetOnClick([this]() {
        if (m_OnReturn) m_OnReturn();
    });
    m_XPNumber = std::make_shared<NumberSystem>(0.84f, 0.93f, 29.0f, 38.0f, RESOURCE_DIR"/img/moneyInfo.png");
    m_XPNumber->SetValue(m_XP);

    m_CatFoodNumber = std::make_shared<NumberSystem>(0.84f, -0.93f, 29.0f, 38.0f, RESOURCE_DIR"/img/moneyInfo.png");
    m_CatFoodNumber->SetValue(m_CatFood);

    // 🚀 初始化滑動 UI
    m_DeckUI = std::make_shared<DeckUI>();

    // 假設你要把圖鑑裡所有已解鎖的貓都丟進去滑動列表
    // 你可以自己寫一個函數把 PlayerData::m_UnlockedCats 的 Key 抽出來，或是先寫死幾隻測試
    std::vector<UnitID> testCatList = {
        UnitID::CAT, UnitID::AXE_CAT, UnitID::LongCat,
        UnitID::LONG_LEG_CAT, UnitID::CowCat, UnitID::FishCat, // 多塞幾隻測滑動長度
        UnitID::DinoCat, UnitID::FlyCat, UnitID::GaintCat
    };

    m_DeckUI->LoadCats(testCatList);

    // 註冊當列表內的貓咪被點擊時，你要做什麼事？
    m_DeckUI->SetOnCatSelected([this](UnitID selectedId) {
        LOG_DEBUG("在隊伍編成畫面，玩家選中了貓咪 ID: " + std::to_string(static_cast<int>(selectedId)));
        // TODO: 之後在這裡處理點擊下方角色，替換掉上方陣容的邏輯
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
    auto pData = PlayerData::GetInstance();

    // 🚀 即時同步資源數值
    if (m_XP != pData->GetXP()) {
        m_XP = pData->GetXP();
        if (m_XPNumber) m_XPNumber->SetValue(m_XP);
    }
    if (m_CatFood != pData->GetCatFood()) {
        m_CatFood = pData->GetCatFood();
        if (m_CatFoodNumber) m_CatFoodNumber->SetValue(m_CatFood);
    }
    if (m_DeckUI) m_DeckUI->Update();
}

void DeckScene::Draw() {
    m_Background.Draw();
        // LOG_DEBUG("StartLevelUpgradeScene cpp！");

    static auto uiAtlas = std::make_shared<Util::Image>(RESOURCE_DIR"/img/img006_tw.png");
    glm::vec2 sheetSize = uiAtlas->GetSize();
    float iconBoost = 0.8f;
    float baseScale = 1.0f;

    if (m_ReturnBtn) {
        m_ReturnBtn->m_Transform.scale = {
            ((float)Cut::RETURN_BASE_W / sheetSize.x) * baseScale,
            ((float)Cut::RETURN_BASE_H / sheetSize.y) * baseScale
        };
        m_ReturnBtn->SetZIndex(21);
        m_ReturnBtn->DrawRect(Cut::RETURN_BASE_X, Cut::RETURN_BASE_Y, Cut::RETURN_BASE_W, Cut::RETURN_BASE_H);

        glm::vec2 originalPos = m_ReturnBtn->m_Transform.translation;
        m_ReturnBtn->m_Transform.translation += glm::vec2(-0.2f, 0.0f);

        m_ReturnBtn->m_Transform.scale = {
            ((float)Cut::RETURN_ICON_W / sheetSize.x) * baseScale * iconBoost,
            ((float)Cut::RETURN_ICON_H / sheetSize.y) * baseScale * iconBoost
        };
        m_ReturnBtn->SetZIndex(22);
        m_ReturnBtn->DrawRect(Cut::RETURN_ICON_X, Cut::RETURN_ICON_Y, Cut::RETURN_ICON_W, Cut::RETURN_ICON_H);

        m_ReturnBtn->m_Transform.translation = originalPos;
        m_ReturnBtn->m_Transform.scale = {
            ((float)Cut::RETURN_BASE_W / sheetSize.x) * baseScale,
            ((float)Cut::RETURN_BASE_H / sheetSize.y) * baseScale
        };
    }
    // 之後這裡放其他冰箱內按鈕的 Draw (如果要用 DrawRect 疊圖一樣寫在這裡)
    if (m_XPNumber) m_XPNumber->Draw();
    if (m_CatFoodNumber) m_CatFoodNumber->Draw();
    if (m_DeckUI) m_DeckUI->Draw();
}