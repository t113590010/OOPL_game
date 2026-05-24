#include "RareGachaScene.hpp"
#include "Core/Context.hpp"

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
RareGachaScene::RareGachaScene() {
    // 1. 設定冰箱背景 (路徑請換成你實際的背景圖)
    auto bgImage = std::make_shared<Util::Image>(RESOURCE_DIR"/img/RareGachaScene.png");
    m_Background.SetDrawable(bgImage);
    m_Background.SetZIndex(10);

    auto context = Core::Context::GetInstance();
    float windowWidth = (float)context->GetWindowWidth();
    float windowHeight = (float)context->GetWindowHeight();
    glm::vec2 imgSize = bgImage->GetSize();

    // 自動算比例塞滿螢幕
    m_Background.m_Transform.scale = glm::vec2(windowWidth / imgSize.x, windowHeight / imgSize.y);

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

    // 🧊 冰箱按鈕 (維持 atlasPath)
    m_StorageBtn = std::make_shared<Button>(-0.66, -0.72 , 85, 65, atlasPath, " ", 30, Util::Color(255, 255, 255, 255));
    m_StorageBtn->SetZIndex(20);
    m_StorageBtn->SetOnClick([this]() { if (m_OnStorageBtnClick) m_OnStorageBtnClick(); });

    float spanOffset_X = 0.11f;
    float spanOffset_Y = -0.73f;
    float spanOffset2_Y = -0.77f;

    // 💰 單抽按鈕空殼 (設定在畫面的右下方)
    m_span = std::make_shared<Button>(0.33f, spanOffset_Y, 300.0f, 86.0f, img007Path, " ", 30, Util::Color(255, 255, 255, 255));
    m_span->SetZIndex(1); // 設為 0，藏在背景下面，因為背景已經畫好按鈕了
    m_span->SetOnClick([this]() {
        if (m_Onspan) m_Onspan();
    });

    // 💰 十連抽按鈕空殼 (設定在畫面更右下方)
    m_muti_span = std::make_shared<Button>(m_span->GetTransform().scale.x+spanOffset_X, spanOffset_Y, 300.0f, 86.0f, img007Path, " ", 30, Util::Color(255, 255, 255, 255));
    m_muti_span->SetZIndex(1); // 藏在背景下面
    m_muti_span->SetOnClick([this]() {
        if (m_On_muti_span) m_On_muti_span();
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

    // 🎫 設定票卷數字 (預估在單抽按鈕的底框上)
    m_TicketNumber = std::make_shared<NumberSystem>(
        0.4f, -0.94f,   // 👈 記得微調這裡的 X 和 Y 比例
        29.0f, 36.0f,
        RESOURCE_DIR"/img/moneyInfo.png"
    );
    m_TicketNumber->SetValue(m_Tickets); // 假資料：3 張票卷

    // 💰 建立單抽按鈕上的數字
    m_SpanCostNumber = std::make_shared<NumberSystem>(
        0.475f, spanOffset2_Y,  // 👈 先對齊單抽按鈕的 Y，X 放按鈕內偏右
        29.0f, 38.0f,         // 讓按鈕上的字稍微縮小一點點
        RESOURCE_DIR"/img/moneyInfo.png"
    );

    // 💰 建立多抽按鈕上的數字
    m_MutiSpanCostNumber = std::make_shared<NumberSystem>(
        0.835f, spanOffset2_Y,  // 👈 對齊多抽按鈕的 Y
        29.0f, 38.0f,
        RESOURCE_DIR"/img/moneyInfo.png"
    );
    // 🥫 建立單抽按鈕上的「罐頭圖示」
    m_SpanCatFoodIcon = std::make_shared<Button>(
        0.32f, spanOffset2_Y,  // 👈 X 放在數字 150 的左邊，Y 和數字對齊
        40.0f, 40.0f,          // 視覺基準大小
        img006Path, " ", 30, Util::Color(255, 255, 255, 255)
    );

    // 🥫 建立多抽按鈕上的「罐頭圖示」
    m_MutiSpanCatFoodIcon = std::make_shared<Button>(
        0.65f, spanOffset2_Y,  // 👈 X 放在數字 1500 的左邊，Y 和數字對齊
        40.0f, 40.0f,
        img006Path, " ", 30, Util::Color(255, 255, 255, 255)
    );
    // 🚀 初始化時，立刻執行一次判斷邏輯
    UpdateGachaButtons();
}
void RareGachaScene::UpdateGachaButtons() {
    // 同步刷新右上角的總票券
    if (m_TicketNumber) m_TicketNumber->SetValue(m_Tickets);

    if (m_Tickets > 0) {
        // 有票券的狀態
        m_SpanCostNumber->SetValue(1); // 單抽 1 張

        // 多抽邏輯：大於 10 寫 10，小於等於 10 寫實際數字 n
        int multiValue = (m_Tickets > 10) ? 10 : m_Tickets;
        m_MutiSpanCostNumber->SetValue(multiValue);
    }
    else {
        // 票券為 0 的狀態，改吃罐頭！
        m_SpanCostNumber->SetValue(150);  // 單抽 150 罐頭
        m_MutiSpanCostNumber->SetValue(1500); // 多抽 1500 罐頭
    }
}
void RareGachaScene::SetOnReturnBtnClick(std::function<void()> callback) {
    m_OnReturn = callback;
}

void RareGachaScene::Update() {
    if (m_ReturnBtn) {
        m_ReturnBtn->Update();
    }
    if (m_StorageBtn) m_StorageBtn->Update();
    if (m_span) m_span->Update();
    if (m_muti_span) m_muti_span->Update();
}
void RareGachaScene::SetOnStorageBtnClick(std::function<void()> callback) { m_OnStorageBtnClick = callback; }
void RareGachaScene::SetOnspanClick(std::function<void()> callback) { m_Onspan = callback; }
void RareGachaScene::SetOn_muti_spanClick(std::function<void()> callback) { m_On_muti_span = callback; }
void RareGachaScene::Draw() {
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

    if (m_StorageBtn) {
        //底層

        //冰箱
        m_StorageBtn->m_Transform.scale = { ((float)Cut::STORAGE_W / sheetSize.x) * 0.8f, ((float)Cut::STORAGE_H / sheetSize.y) * 0.8f };
        m_StorageBtn->SetZIndex(1);
        m_StorageBtn->DrawRect(Cut::STORAGE_X, Cut::STORAGE_Y, Cut::STORAGE_W, Cut::STORAGE_H);
    }




    // ==========================================
    // 🔢 繪製所有數字系統
    // ==========================================
    if (m_XPNumber) m_XPNumber->Draw();
    if (m_CatFoodNumber)m_CatFoodNumber->Draw();
    if (m_TicketNumber)m_TicketNumber->Draw();
    // 畫單抽按鈕上面的數字 (永遠看得到，只是數字可能是 1 或是 150)
    if (m_SpanCostNumber) m_SpanCostNumber->Draw();

    // 🚀 關鍵判斷：只有當票券大於 0 的時候，才去畫多抽按鈕上面的數字！
    if (m_Tickets > 0) {
        if (m_MutiSpanCostNumber) m_MutiSpanCostNumber->Draw();
    }else {
        float baseScale = 1.1f;
        float catFoodScale = 0.8f; // 👈 罐頭圖示的縮放大小，可以自己微調
        // 畫單抽黃色底板
        if (m_span) {
            m_span->m_Transform.scale = {
                ((float)Cut::SPAN_BTN_W / sheetSize007.x) * baseScale,
                ((float)Cut::SPAN_BTN_H / sheetSize007.y) * baseScale
            };
            m_span->SetZIndex(21);
            m_span->DrawRect(Cut::SPAN_BTN_X, Cut::SPAN_BTN_Y, Cut::SPAN_BTN_W, Cut::SPAN_BTN_H);

            m_span->m_Transform.scale = {
                ((float)Cut::SPAN_BTN_W / sheetSize007.x) * baseScale,
                ((float)Cut::SPAN_BTN_H / sheetSize007.y) * baseScale
            };
        }

        // 畫多抽黃色底板
        if (m_muti_span) {
            m_muti_span->m_Transform.scale = {
                ((float)Cut::SPAN_BTN_W / sheetSize007.x) * baseScale,
                ((float)Cut::SPAN_BTN_H / sheetSize007.y) * baseScale
            };
            m_muti_span->SetZIndex(21);
            m_muti_span->DrawRect(Cut::SPAN_BTN_X, Cut::SPAN_BTN_Y, Cut::SPAN_BTN_W, Cut::SPAN_BTN_H);

            m_muti_span->m_Transform.scale = {
                ((float)Cut::SPAN_BTN_W / sheetSize007.x) * baseScale,
                ((float)Cut::SPAN_BTN_H / sheetSize007.y) * baseScale
            };
        }
        // 🥫 畫單抽的罐頭圖示 (疊在黃色底板上)
        if (m_SpanCatFoodIcon) {
            m_SpanCatFoodIcon->m_Transform.scale = {
                ((float)Cut::CATFOOD_ICON_W / sheetSize.x) * catFoodScale,
                ((float)Cut::CATFOOD_ICON_H / sheetSize.y) * catFoodScale
            };
            m_SpanCatFoodIcon->SetZIndex(65);
            m_SpanCatFoodIcon->DrawRect(Cut::CATFOOD_ICON_X, Cut::CATFOOD_ICON_Y, Cut::CATFOOD_ICON_W, Cut::CATFOOD_ICON_H);
        }

        // 🥫 畫多抽的罐頭圖示 (疊在黃色底板上)
        if (m_MutiSpanCatFoodIcon) {
            m_MutiSpanCatFoodIcon->m_Transform.scale = {
                ((float)Cut::CATFOOD_ICON_W / sheetSize.x) * catFoodScale,
                ((float)Cut::CATFOOD_ICON_H / sheetSize.y) * catFoodScale
            };
            m_MutiSpanCatFoodIcon->SetZIndex(65);
            m_MutiSpanCatFoodIcon->DrawRect(Cut::CATFOOD_ICON_X, Cut::CATFOOD_ICON_Y, Cut::CATFOOD_ICON_W, Cut::CATFOOD_ICON_H);
        }

    }


    // ==========================================
    // 🔢 最後再畫所有數字系統 (疊在最上面才不會被蓋住！)
    // ==========================================
    if (m_XPNumber) m_XPNumber->Draw();
    if (m_CatFoodNumber) m_CatFoodNumber->Draw();
    if (m_TicketNumber) m_TicketNumber->Draw();

    // 畫抽卡按鈕上的數字 (不管是票券還是罐頭，現在都一定會畫出來！)
    if (m_SpanCostNumber) m_SpanCostNumber->Draw();
    if (m_MutiSpanCostNumber) m_MutiSpanCostNumber->Draw();

}