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

    // 彈窗按鈕：底板 (假設跟 HomeScene 一樣用 img010 的黃色底框)
    const float POPUP_BTN_BASE_X = 255.0f, POPUP_BTN_BASE_Y = 207.0f, POPUP_BTN_BASE_W = 260.0f, POPUP_BTN_BASE_H = 50.0f;

    // 彈窗按鈕：上面的文字 (假設在 img007 裡面)
    const float POPUP_USE_TXT_X = 0.0f, POPUP_USE_TXT_Y = 0.0f, POPUP_USE_TXT_W = 100.0f, POPUP_USE_TXT_H = 40.0f; // 「使用貓咪」字樣
    const float POPUP_XP_TXT_X = 0.0f, POPUP_XP_TXT_Y = 0.0f, POPUP_XP_TXT_W = 100.0f, POPUP_XP_TXT_H = 40.0f;  // 「兌換 XP」字樣
    const float POPUP_CANCEL_TXT_X = 446.0f, POPUP_CANCEL_TXT_Y = 0.0f, POPUP_CANCEL_TXT_W = 512.0f-POPUP_CANCEL_TXT_X, POPUP_CANCEL_TXT_H = 63.0f; // 「取消」字樣
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
    auto blackImg = std::make_shared<Util::Image>(RESOURCE_DIR "/img/black.png");
    m_Background_black.SetDrawable(blackImg);
    m_Background_black.SetZIndex(49);
    m_Background_black.m_Transform.scale = glm::vec2(windowWidth / blackImg->GetSize().x, windowHeight / blackImg->GetSize().y);
    // 2. 建立返回按鈕 (假設放在左上角)
    // 這裡先借用你的 battleStart 圖，之後你可以換成專用的返回圖示或合圖切圖
    const std::string img006Path = RESOURCE_DIR"/img/img006_tw.png";
    const std::string img007Path = RESOURCE_DIR"/img/img007_tw.png";
    const std::string atlasPath = RESOURCE_DIR"/img/img010_tw.png";
    const std::string yesOrNoPath = RESOURCE_DIR"/img/yesOrNo.png";


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

    auto img007 = std::make_shared<Util::Image>(img007Path);
    glm::vec2 sheetSize007 = img007->GetSize();
    float b_size_w_1 = 185.0f;
    float b_size_h_1 = 40.0f;
    float b_size_w_2 = 225.0f;
    float b_y = -0.94;
    m_ToUse = std::make_shared<Button>(-0.05f, b_y, b_size_w_1, b_size_h_1, img007Path, " ", 30, Util::Color(255, 255, 255, 255));
    m_ToUse->SetZIndex(1); // 設為 1，藏在背景底下，讓背景圖案顯示出來
    m_ToUse->SetScale(b_size_w_1 / sheetSize007.x, b_size_h_1 / sheetSize007.y);
    // m_ToUse->SetOnClick([this]() {
    //     if (m_OnToUse) m_OnToUse();
    // });
    // ==========================================
    // 💰 「使用」按鈕邏輯
    // ==========================================
    m_ToUse->SetOnClick([this]() {
        // 防呆：如果冰箱已經空了，就什麼都不做
        if (m_StorageItems.empty()) return;
        for (auto it = m_StorageItems.begin(); it != m_StorageItems.end(); ++it) {
            if ((*it)) {
                UnitID selectedCatID = (*it)->GetCatID();
                std::cout << "[Storage] 使用了貓咪 ID: " << static_cast<int>(selectedCatID) << std::endl;

                // 從畫面與陣列中刪除這隻貓
                m_StorageItems.erase(it);

                // 呼叫 App.cpp 綁定的事件 (可以去那邊寫加入兵營的邏輯)
                if (m_OnToUse) m_OnToUse();
                break; // 一次只消耗一隻，處理完就跳出
            }
        }
    });

    // 💰 「換 XP」按鈕空殼 (設定在畫面更右下)
    m_ToXp = std::make_shared<Button>(0.2825f, b_y, b_size_w_2, b_size_h_1, img007Path, " ", 30, Util::Color(255, 255, 255, 255));
    m_ToXp->SetZIndex(1); // 藏在背景底下
    m_ToXp->SetScale(b_size_w_2 / sheetSize007.x, b_size_h_1 / sheetSize007.y);
    // m_ToXp->SetOnClick([this]() {
    //     if (m_OnToXp) m_OnToXp();
    // });


    // ==========================================
    // 💰 「換 XP」按鈕邏輯
    // ==========================================
    m_ToXp->SetOnClick([this]() { // ✅ 這裡正確使用 m_ToXp 了！
        // 防呆：如果冰箱已經空了，就什麼都不做
        if (m_StorageItems.empty()) return;

        int totalCats = 0;
        for (auto& cat : m_StorageItems) {
            if (cat) {
                totalCats++; // 計算總共有幾隻貓

                // 呼叫 App.cpp 綁定的事件
                if (m_OnToXp) m_OnToXp();
            }
        }

        std::cout << "[Storage] 將 " << totalCats << " 隻貓咪轉換為 XP!" << std::endl;

        // 🚀 一次性把所有的 XP 加上去 (數量 * 10000)
        m_XP += (10000 * totalCats);
        if (m_XPNumber) m_XPNumber->SetValue(m_XP);

        // 🚀 迴圈結束後，一次性清空所有貓咪！
        m_StorageItems.clear();

        // 更新顯示數量為 0
        m_cat_count->SetValue(0);
    });
    // ==========================================
    // 🚀 初始化彈窗介面的新按鈕 (置中偏下)
    // ==========================================

    float popup_btn_w = 225.0f;
    float popup_btn_h = 40.0f;

    // 🌟 直接吃你 P 好的 UseBtn.png
    m_PopupUseBtn = std::make_shared<Button>(
        -0.2f, -0.3f, popup_btn_w-40, popup_btn_h-40,
        RESOURCE_DIR"/img/UseBtn.png", " ", 30, Util::Color(255, 255, 255, 255)
    );
    m_PopupUseBtn->SetZIndex(50);
    m_PopupUseBtn->SetOnClick([this]() {
        if (m_SelectedPopupCat) {
            std::cout << "[Popup] 使用了貓咪 ID: " << static_cast<int>(m_SelectedPopupCat->GetCatID()) << std::endl;
            // ... (裡面的刪除邏輯維持你原本的) ...
            for (auto it = m_StorageItems.begin(); it != m_StorageItems.end(); ++it) {
                if (*it == m_SelectedPopupCat) {
                    m_StorageItems.erase(it);
                    break;
                }
            }
            if (m_OnToUse) m_OnToUse();
            m_cat_count->SetValue(static_cast<int>(m_StorageItems.size()));
        }
        m_IsPopupOpen = false;
    });

    // 🌟 直接吃你 P 好的 ToXpBtnpng.png
    m_PopupXpBtn = std::make_shared<Button>(
        0.2f, -0.3f, popup_btn_w, popup_btn_h,
        RESOURCE_DIR"/img/ToXpBtnpng.png", " ", 30, Util::Color(255, 255, 255, 255)
    );
    m_PopupXpBtn->SetZIndex(50);
    m_PopupXpBtn->SetOnClick([this]() {
        if (m_SelectedPopupCat) {
            std::cout << "[Popup] 貓咪轉換為 XP!" << std::endl;
            m_XP += 10000;
            if (m_XPNumber) m_XPNumber->SetValue(m_XP);
            // ... (裡面的刪除邏輯維持你原本的) ...
            for (auto it = m_StorageItems.begin(); it != m_StorageItems.end(); ++it) {
                if (*it == m_SelectedPopupCat) {
                    m_StorageItems.erase(it);
                    break;
                }
            }
            if (m_OnToXp) m_OnToXp();
            m_cat_count->SetValue(static_cast<int>(m_StorageItems.size()));
        }
        m_IsPopupOpen = false;
    });
    m_PopupCancelBtn = std::make_shared<Button>(
        0.3f, -0.1f, 150.0f, 40.0f,
        yesOrNoPath, " ", 30, Util::Color(255, 255, 255, 255)
    );
    m_PopupCancelBtn->SetZIndex(50);
    m_PopupCancelBtn->SetOnClick([this]() {
        m_IsPopupOpen = false; // 單純關閉彈窗
    });

    // ==========================================
    // 🐈 放入貓咪並綁定「點擊開啟彈窗」事件
    // ==========================================
    auto addCatToStorage = [this](UnitID id) {
        auto cat = std::make_shared<StorageItem>(0.0f, 0.0f, id);

        cat->SetOnSelectCallback([this](StorageItem* clickedCat) {
            if (!m_IsPopupOpen) { // 只有在沒開彈窗時才能點擊貓咪
                m_IsPopupOpen = true;
                m_LockPopupClick = true; // 🚀 關鍵：彈窗打開的瞬間，立刻上鎖！
                // 找出被點擊的 shared_ptr 並記錄起來
                for(auto& c : m_StorageItems) {
                    if (c.get() == clickedCat) {
                        m_SelectedPopupCat = c;
                        break;
                    }
                }
            }
        });
        m_StorageItems.push_back(cat);
    };
    addCatToStorage(UnitID::LONG_LEG_CAT);
    addCatToStorage(UnitID::CAT);
    addCatToStorage(UnitID::AXE_CAT);
    addCatToStorage(UnitID::CowCat);
    addCatToStorage(UnitID::FishCat);
    addCatToStorage(UnitID::bighead);

    addCatToStorage(UnitID::DogDoin);
    addCatToStorage(UnitID::DogDoin);

    addCatToStorage(UnitID::Peashooter);
    addCatToStorage(UnitID::Queen);


    cat_count = m_StorageItems.size();
    m_cat_count->SetValue(cat_count); // 假資料：n隻貓咪

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
    if (m_TestCat) m_TestCat->Update();
    // 🚀 讓冰箱裡所有的貓咪都更新 (旋轉、漂浮、感應點擊)
    for (auto& item : m_StorageItems) {
        if (item) item->Update();
    }
    if (m_IsPopupOpen) {
        // 🚀 防點擊穿透機制
        if (m_LockPopupClick) {
            // 如果玩家「沒有」按下左鍵 (代表手指已經鬆開了)，就解鎖
            if (!Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB)) {
                m_LockPopupClick = false;
            }
        }
        else {
            // 🚀 只有解鎖後，才允許彈窗內的 3 個按鈕感應點擊
            if (m_PopupUseBtn) m_PopupUseBtn->Update();
            if (m_PopupXpBtn) m_PopupXpBtn->Update();
            if (m_PopupCancelBtn) m_PopupCancelBtn->Update();
        }
    }
    else {
        // 彈窗沒開時，更新全域按鈕與背景貓咪
        if (m_ReturnBtn) m_ReturnBtn->Update();
        if (m_ToUse) m_ToUse->Update();
        if (m_ToXp) m_ToXp->Update();
        if (m_TestCat) m_TestCat->Update();

        for (auto& item : m_StorageItems) {
            if (item) item->Update();
        }
    }
}

void StorageScene::Draw() {
    m_Background.Draw();

    static auto uiAtlas = std::make_shared<Util::Image>(RESOURCE_DIR"/img/img006_tw.png");
    glm::vec2 sheetSize = uiAtlas->GetSize();

    static auto img007Atlas = std::make_shared<Util::Image>(RESOURCE_DIR"/img/img007_tw.png");
    glm::vec2 sheetSize007 = img007Atlas->GetSize();

    static auto yesOrNoAtlas = std::make_shared<Util::Image>(RESOURCE_DIR"/img/yesOrNo.png"); // 👈 彈窗底框通常在這
    glm::vec2 sheetSizeyesOrNo = yesOrNoAtlas->GetSize();
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
    if (m_TestCat) m_TestCat->Draw();
    // 🚀 畫出冰箱裡所有的貓咪
    for (auto& item : m_StorageItems) {
        if (item) item->Draw();
    }
    if (m_IsPopupOpen && m_SelectedPopupCat) {
        // 呼叫我們寫的專用函數，把貓咪放大畫在中間

        m_Background_black.Draw();;

        m_SelectedPopupCat->DrawForPopup();

        // 🌟 因為圖片已經做好了，直接無腦 Draw，不用再切圖！
        if (m_PopupUseBtn) m_PopupUseBtn->Draw();
        if (m_PopupXpBtn) m_PopupXpBtn->Draw();
        if (m_PopupCancelBtn) {
            float popupBaseScale = 1.2f;

            // 這裡假設你用 img010 當底板，所以切圖算式如下：
            m_PopupCancelBtn->m_Transform.scale = {
                ((float)Cut::POPUP_CANCEL_TXT_W / sheetSizeyesOrNo.x) * popupBaseScale,
                ((float)Cut::POPUP_CANCEL_TXT_H / sheetSizeyesOrNo.y) * popupBaseScale
            };
            m_PopupCancelBtn->SetZIndex(50);
            m_PopupCancelBtn->DrawRect(Cut::POPUP_CANCEL_TXT_X, Cut::POPUP_CANCEL_TXT_Y, Cut::POPUP_CANCEL_TXT_W, Cut::POPUP_CANCEL_TXT_H);

            // 歸位
            m_PopupCancelBtn->m_Transform.scale = {
                ((float)Cut::POPUP_CANCEL_TXT_W / sheetSizeyesOrNo.x) * popupBaseScale,
                ((float)Cut::POPUP_CANCEL_TXT_H / sheetSizeyesOrNo.y) * popupBaseScale
            };

            // 💡 註：因為你沒有「取消」兩個字的圖案，所以按鈕只會顯示出一個空底板。
            // 建議你也像另外兩個按鈕一樣，自己 P 一張 CancelBtn.png 餵給它，這樣最省事！
        }
    }
}