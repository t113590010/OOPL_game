#include "StorageScene.hpp"
#include "Core/Context.hpp"
#include <iostream>

// 如果有專屬的切圖座標，可以建一個獨立的 namespace
namespace Cut {
    // 🟡 返回按鈕：底板 (你之後自己填)
    const float RETURN_ICON_X = 423.0f;
    const float RETURN_ICON_Y = 170.0f;
    const float RETURN_ICON_W = 511.0f - RETURN_ICON_X;
    const float RETURN_ICON_H = 232.0f-RETURN_ICON_Y;

    // ↩️ 返回按鈕：上層迴轉箭頭 (你之後自己填)
    const float RETURN_BASE_X = 256.0f;
    const float RETURN_BASE_Y = 0.0f;
    const float RETURN_BASE_W = 99.0f;
    const float RETURN_BASE_H = 97.0f;

    // 👇 冰箱 (img010)
    const float STORAGE_X = 0.0f, STORAGE_Y = 0.0f, STORAGE_W = 72.0f, STORAGE_H = 82.0f;

    // 這裡只是預估座標，你需要自己量測並修改
    const float SPAN_BTN_X = 297.0f;
    const float SPAN_BTN_Y = 426.0f;
    const float SPAN_BTN_W = 511.0f-SPAN_BTN_X;
    const float SPAN_BTN_H = 508.0f-SPAN_BTN_Y;

    // 🥫 罐頭圖示 (img006_tw.png 裡面) - 座標請自己填
    const float CATFOOD_ICON_X = 161.0f;
    const float CATFOOD_ICON_Y = 158.0f;
    const float CATFOOD_ICON_W = 219.0f - CATFOOD_ICON_X;
    const float CATFOOD_ICON_H = 203.0f-CATFOOD_ICON_Y;
}
StorageScene::StorageScene() {
    // 1. 設定冰箱背景 (路徑請換成你實際的背景圖)
    auto bgImage = std::make_shared<Util::Image>(RESOURCE_DIR"/img/StorageScene.png");
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
    const std::string img006Path = RESOURCE_DIR"/img/img006_tw.png";
    const std::string img007Path = RESOURCE_DIR"/img/img007_tw.png";
    const std::string atlasPath = RESOURCE_DIR"/img/img010_tw.png";

    float btnSize = 100.0f; // 視覺基準大小

    m_ReturnBtn = std::make_shared<Button>(
        -0.82f, -0.87f,   // 左下角座標
        btnSize, btnSize,
        img006Path,
        " ", 30,
        Util::Color(255, 255, 255, 255)
    );

    m_ReturnBtn->SetZIndex(20);
    m_ReturnBtn->SetOnClick([this]() {
        if (m_OnReturn) {
            m_OnReturn();
        }
    });

    m_XPNumber = std::make_shared<NumberSystem>(
       0.86f, 0.94f,
       29.0f, 38.0f,
       RESOURCE_DIR"/img/moneyInfo.png"
   );

    // 設定一個測試數字
    m_XPNumber->SetValue(m_XP);
    // 🥫 設定罐頭數字 (預估在右下方底部)
    m_CatFoodNumber = std::make_shared<NumberSystem>(
        0.86f, -0.94f,  // 👈 記得微調這裡的 X 和 Y 比例
        29.0f, 38.0f,
        RESOURCE_DIR"/img/moneyInfo.png"
    );
    m_CatFoodNumber->SetValue(m_CatFood); // 假資料：30 個罐頭

    m_cat_count = std::make_shared<NumberSystem>(
          0.78f, -0.82f,  // 👈 記得微調這裡的 X 和 Y 比例
          15.0f, 19.0f,
          RESOURCE_DIR"/img/moneyInfo.png",
          NumberSystem::FontType::WHITE_SMALL
      );
    m_cat_count->SetValue(cat_count); // 假資料：n隻貓咪

    auto img007 = std::make_shared<Util::Image>(img007Path);
    glm::vec2 sheetSize007 = img007->GetSize();
    float b_size_w_1 = 185.0f;
    float b_size_h_1 = 40.0f;
    float b_size_w_2 = 225.0f;
    float b_y = -0.94;
    m_ToUse = std::make_shared<Button>(-0.05f, b_y, b_size_w_1, b_size_h_1, img007Path, " ", 30, Util::Color(255, 255, 255, 255));
    m_ToUse->SetZIndex(1); // 設為 1，藏在背景底下，讓背景圖案顯示出來
    m_ToUse->SetScale(b_size_w_1 / sheetSize007.x, b_size_h_1 / sheetSize007.y);
    m_ToUse->SetOnClick([this]() {
        if (m_OnToUse) m_OnToUse();
    });

    // 💰 「換 XP」按鈕空殼 (設定在畫面更右下)
    m_ToXp = std::make_shared<Button>(0.2825f, b_y, b_size_w_2, b_size_h_1, img007Path, " ", 30, Util::Color(255, 255, 255, 255));
    m_ToXp->SetZIndex(1); // 藏在背景底下
    m_ToXp->SetScale(b_size_w_2 / sheetSize007.x, b_size_h_1 / sheetSize007.y);
    m_ToXp->SetOnClick([this]() {
        if (m_OnToXp) m_OnToXp();
    });
}

void StorageScene::SetOnReturnBtnClick(std::function<void()> callback) {
    m_OnReturn = callback;
}
void StorageScene::SetOnToUse(std::function<void()> callback) {
    m_OnToUse = callback;
}
void StorageScene::SetOnToXp(std::function<void()> callback) {
    m_OnToXp = callback;
}
void StorageScene::Update() {
    if (m_ReturnBtn) {
        m_ReturnBtn->Update();
    }
    // 之後這裡放其他冰箱內按鈕的 Update
    if (m_ToUse) m_ToUse->Update();
    if (m_ToXp) m_ToXp->Update();
}

void StorageScene::Draw() {
    m_Background.Draw();
    static auto uiAtlas = std::make_shared<Util::Image>(RESOURCE_DIR"/img/img006_tw.png");
    glm::vec2 sheetSize = uiAtlas->GetSize();

    static auto img007Atlas = std::make_shared<Util::Image>(RESOURCE_DIR"/img/img007_tw.png");
    glm::vec2 sheetSize007 = img007Atlas->GetSize();
    float iconBoost = 0.8f;
    float baseScale = 1.0f;

    if (m_ReturnBtn) {

        // ==========================================
        // 1. 畫黃色底層 (Base)
        // ==========================================
        m_ReturnBtn->m_Transform.scale = {
            ((float)Cut::RETURN_BASE_W / sheetSize.x) * baseScale,
            ((float)Cut::RETURN_BASE_H / sheetSize.y) * baseScale
        };
        m_ReturnBtn->SetZIndex(21);
        m_ReturnBtn->DrawRect(Cut::RETURN_BASE_X, Cut::RETURN_BASE_Y, Cut::RETURN_BASE_W, Cut::RETURN_BASE_H);

        // ==========================================
        // 2. 畫上層迴轉箭頭 (Icon 往左偏移)
        // ==========================================
        // 👉 備份原座標
        glm::vec2 originalPos = m_ReturnBtn->m_Transform.translation;

        // 👉 往左偏移 0.2
        m_ReturnBtn->m_Transform.translation += glm::vec2(-0.2f, 0.0f);

        m_ReturnBtn->m_Transform.scale = {
            ((float)Cut::RETURN_ICON_W / sheetSize.x) * baseScale * iconBoost,
            ((float)Cut::RETURN_ICON_H / sheetSize.y) * baseScale * iconBoost
        };
        m_ReturnBtn->SetZIndex(22);
        m_ReturnBtn->DrawRect(Cut::RETURN_ICON_X, Cut::RETURN_ICON_Y, Cut::RETURN_ICON_W, Cut::RETURN_ICON_H);

        // ==========================================
        // 3. 🚀 終極歸位 (鎖定碰撞判定為 Base)
        // ==========================================
        // 座標歸位
        m_ReturnBtn->m_Transform.translation = originalPos;

        // 比例歸位為 Base，確保下一幀 Update() 算出的點擊範圍完全等同黃色底板！
        m_ReturnBtn->m_Transform.scale = {
            ((float)Cut::RETURN_BASE_W / sheetSize.x) * baseScale,
            ((float)Cut::RETURN_BASE_H / sheetSize.y) * baseScale
        };
    }

    if (m_XPNumber) m_XPNumber->Draw();
    if (m_CatFoodNumber)m_CatFoodNumber->Draw();
    if (m_cat_count)m_cat_count->Draw();
    if (m_ToUse) {
        // m_ToUse->SetZIndex(50); // 故意拉到最上層，絕對看得見
        // 🚀 注意！這裡我們不寫 m_ToUse->m_Transform.scale = ... 了！
        // 引擎會自動拿建構子設定好的那個完美 100x40 比例來畫！
        m_ToUse->DrawRect(Cut::SPAN_BTN_X, Cut::SPAN_BTN_Y, Cut::SPAN_BTN_W, Cut::SPAN_BTN_H);
    }

    if (m_ToXp) {
        // m_ToXp->SetZIndex(50);
        m_ToXp->DrawRect(Cut::SPAN_BTN_X, Cut::SPAN_BTN_Y, Cut::SPAN_BTN_W, Cut::SPAN_BTN_H);
    }
    // 之後這裡放其他冰箱內按鈕的 Draw (如果要用 DrawRect 疊圖一樣寫在這裡)
}