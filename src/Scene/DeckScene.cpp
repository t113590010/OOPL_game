#include "DeckScene.hpp"
#include "Core/Context.hpp"

// 如果有專屬的切圖座標，可以建一個獨立的 namespace
#include "PlayerData.hpp"
#include <glm/trigonometric.hpp>
#include "Util/SFX.hpp"

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
    // 🚀 設定邊框寬度 (你可以去小畫家量一下木頭邊框實際多寬，這裡先抓 120)
    float borderWidth = 30.0f;

    // 初始化左邊框
    m_LeftBorder = std::make_shared<Button>(
        -1.035f, 0.0f,
        windowHeight,  // 🚀 原本的高 -> 現在變成寬
        borderWidth,   // 🚀 原本的寬 -> 現在變成高
        RESOURCE_DIR"/img/img008_tw.png",
        " ", 20, Util::Color(0,0,0,0)
    );
    m_LeftBorder->SetZIndex(80);
    // 轉 90 度後，原本橫向的 windowHeight 就會完美變成垂直高度！
    m_LeftBorder->m_Transform.rotation = glm::radians(-90.0f);

    // 初始化右邊框
    m_RightBorder = std::make_shared<Button>(
        1.035f, 0.0f,
        windowHeight,  // 🚀 同樣寬高對調
        borderWidth,
        RESOURCE_DIR"/img/img008_tw.png",
        " ", 20, Util::Color(0,0,0,0)
    );
    m_RightBorder->SetZIndex(80);
    m_RightBorder->m_Transform.rotation = glm::radians(90.0f);


    m_Background.m_Transform.scale = glm::vec2(windowWidth / imgSize.x, windowHeight / imgSize.y);
    auto blackImg = std::make_unique<Util::Image>(RESOURCE_DIR "/img/black.png");
    glm::vec2 blackImgSize = blackImg->GetSize();

    m_PopupBg = std::make_shared<Util::GameObject>(std::move(blackImg), 95);
    m_PopupBg->m_Transform.scale = glm::vec2(windowWidth / blackImgSize.x, windowHeight / blackImgSize.y);

    const std::string img006Path = RESOURCE_DIR"/img/img006_tw.png";
    const std::string img007Path = RESOURCE_DIR"/img/img007_tw.png";
    const std::string yesOrNoPath = RESOURCE_DIR"/img/yesOrNo.png";

    float btnSize = 100.0f;
    m_EmptyDeckPopup = std::make_shared<Util::GameObject>(std::make_unique<Util::Image>(RESOURCE_DIR"/img/failReturn.png"), 100); // Z-Index 設 100 保證在最上層
    m_EmptyDeckPopup->m_Transform.scale = { 0.5f, 0.5f }; // 依照你的圖片大小調整比例
    m_ReturnBtn = std::make_shared<Button>(
        -0.8f, -0.87f, btnSize, btnSize,
        img006Path, " ", 30, Util::Color(255, 255, 255, 255)
    );
    m_ReturnBtn->SetZIndex(20);
    m_ReturnBtn->SetOnClick([this]() {
        Util::SFX(RESOURCE_DIR "/music/clickbtn.mp3").Play();
         // 🚀 1. 檢查目前上陣的貓咪數量
         int equippedCount = 0;
         for (int i = 0; i < 10; ++i) {
             if (static_cast<int>(m_Slots[i]->GetUnitID()) != 0) {
                 equippedCount++;
             }
         }

         // 🚀 2. 判斷是否放行
         if (equippedCount == 0) {
             // ❌ 隊伍空的：發出錯誤音效，並打開彈出視窗
             Util::SFX(RESOURCE_DIR "/music/fail_summon_cat.mp3").Play();
             m_IsPopupActive = true;
         } else {
             // ✅ 隊伍有貓：正常返回主畫面
             if (m_OnReturn) m_OnReturn();
         }
     });

    m_Upg = std::make_shared<Button>(
        -0.078f, -0.93f, 15, 25,
        RESOURCE_DIR"/img/upgrade.png", " ", 30, Util::Color(255, 255, 255, 255)
    );
    m_Upg->SetZIndex(0);
    m_Upg->SetOnClick([this]() {
        if (m_OnUpg) m_OnUpg();
    });


    m_XPNumber = std::make_shared<NumberSystem>(0.84f, 0.93f, 29.0f, 38.0f, RESOURCE_DIR"/img/moneyInfo.png");
    m_XPNumber->SetValue(m_XP);

    m_CatFoodNumber = std::make_shared<NumberSystem>(0.84f, -0.93f, 29.0f, 38.0f, RESOURCE_DIR"/img/moneyInfo.png");
    m_CatFoodNumber->SetValue(m_CatFood);

    // 🚀 初始化滑動 UI
    m_DeckUI = std::make_shared<DeckUI>();

    // 假設你要把圖鑑裡所有已解鎖的貓都丟進去滑動列表
    // 你可以自己寫一個函數把 PlayerData::m_UnlockedCats 的 Key 抽出來，或是先寫死幾隻測試
    // std::vector<UnitID> testCatList = {
    //     UnitID::CAT, UnitID::AXE_CAT, UnitID::LongCat,
    //     UnitID::LONG_LEG_CAT, UnitID::CowCat, UnitID::FishCat, // 多塞幾隻測滑動長度
    //     UnitID::DinoCat, UnitID::FlyCat, UnitID::GaintCat
    // };
    //
    // m_DeckUI->LoadCats(testCatList);
    std::vector<UnitID> realCatList = PlayerData::GetInstance()->GetUnlockedCatsList();
    m_DeckUI->LoadCats(realCatList);
    // 註冊當列表內的貓咪被點擊時，你要做什麼事？
    m_DeckUI->SetOnCatSelected([this](UnitID selectedId) {
        LOG_DEBUG("在隊伍編成畫面，玩家選中了貓咪 ID: " + std::to_string(static_cast<int>(selectedId)));
        // TODO: 之後在這裡處理點擊下方角色，替換掉上方陣容的邏輯
    });

    m_DeckUI->SetOnCatDraggedOut([this](UnitID draggedId) {
        if (!m_IsDraggingGhost && static_cast<int>(draggedId) != 0) {
            LOG_DEBUG("開始拖拉貓咪: " + std::to_string(static_cast<int>(draggedId)));
            m_IsDraggingGhost = true;
            m_DraggedUnitID = draggedId;
            m_DraggedFromSlot = -1;
            // 🚀 建立懸浮頭像 (Ghost)
            const UnitStats& stats = UnitData::Get(draggedId);
            m_DragGhostIcon = std::make_shared<Util::GameObject>(std::make_unique<Util::Image>(stats.iconPath), 90); // Z-Index 設超高
            m_DragGhostIcon->m_Transform.scale = { 1.5f, 1.5f };
        }
    });
    // 設定第一格(左上角)的基準點
    float startX = -0.35f;
    float startY =  0.49f;

    // 設定格子與格子之間的距離
    float spacingX = 0.205f;
    float spacingY = 0.25f;

    m_Slots.clear();
    // 🚀 取得真實存檔裡的 10 格陣容
    const auto& currentDeck = PlayerData::GetInstance()->GetDeck();
    for (int i = 0; i < 10; ++i) {
        // 核心排版算式：
        // i % 5 算出行(Column)：0,1,2,3,4, 0,1,2,3,4
        // i / 5 算出列(Row)   ：0,0,0,0,0, 1,1,1,1,1
        float posX = startX + (i % 5) * spacingX;
        float posY = startY - (i / 5) * spacingY;

        auto slot = std::make_shared<SlotUI>(i, posX, posY);
        slot->SetUnit(currentDeck[i]);
        // 設定點擊事件：當玩家點了這個格子，把它設為「選中狀態」
        slot->SetOnClick([this](int index) {
            LOG_DEBUG("點擊了出陣列表第 " + std::to_string(index + 1) + " 格！");

            m_CurrentSelectedSlot = index;

            // 把所有格子都取消選中，只亮起被點的那個
            for (auto& s : m_Slots) {
                s->SetSelected(false);
            }
            m_Slots[index]->SetSelected(true);
        });
        m_Slots.push_back(slot);
    }
}
int DeckScene::GetHoveredSlotIndex(glm::vec2 mousePos) {
    // Slot 的物理寬高 (跟你 SlotUI.hpp 裡設定的一致)
    // 💡 記得去確認一下你 SlotUI.hpp 裡面的 m_Width 和 m_Height 實體大小
    float slotWidth = 113.0f;
    float slotHeight = 87.0f;

    for (int i = 0; i < m_Slots.size(); ++i) {
        // 🚀 修正：直接拿絕對像素座標，不要再乘以 halfW / halfH
        float cx = m_Slots[i]->GetBaseX();
        float cy = m_Slots[i]->GetBaseY();

        // 計算 AABB 邊界
        float left   = cx - (slotWidth / 2.0f);
        float right  = cx + (slotWidth / 2.0f);
        float bottom = cy - (slotHeight / 2.0f);
        float top    = cy + (slotHeight / 2.0f);

        // 如果滑鼠在這個矩形範圍內，就回傳這個格子的 Index
        if (mousePos.x >= left && mousePos.x <= right &&
            mousePos.y >= bottom && mousePos.y <= top) {
            return i;
            }
    }
    return -1; // 沒碰到任何格子
}
void DeckScene::SetOnReturnBtnClick(std::function<void()> callback) {
    m_OnReturn = callback;
}
void DeckScene::SetOnUpgClick(std::function<void()> callback) {
    m_OnUpg = callback;
}
void DeckScene::EquipCatToSlot(UnitID draggedCat, int targetSlotIndex) {
    if (targetSlotIndex < 0 || targetSlotIndex >= m_Slots.size() || static_cast<int>(draggedCat) == 0) {
        return;
    }

    int existingSlotIndex = -1;
    for (int i = 0; i < m_Slots.size(); ++i) {
        if (m_Slots[i]->GetUnitID() == draggedCat) {
            existingSlotIndex = i;
            break;
        }
    }

    UnitID displacedCat = m_Slots[targetSlotIndex]->GetUnitID();
    auto pData = PlayerData::GetInstance(); // 🚀 取得單例

    if (existingSlotIndex != -1) {
        // 互換位置
        m_Slots[existingSlotIndex]->SetUnit(displacedCat);
        pData->SetDeckUnit(existingSlotIndex, displacedCat); // 🚀 同步資料

        m_Slots[targetSlotIndex]->SetUnit(draggedCat);
        pData->SetDeckUnit(targetSlotIndex, draggedCat);     // 🚀 同步資料
    }
    else {
        // 覆蓋新位置
        m_Slots[targetSlotIndex]->SetUnit(draggedCat);
        pData->SetDeckUnit(targetSlotIndex, draggedCat);     // 🚀 同步資料
    }

    // 🚀 動作完成，立刻存檔！
    pData->SaveToFile();
}
void DeckScene::Refresh() {
    auto pData = PlayerData::GetInstance();

    // 1. 重新讀取最新陣容，並更新上方的 10 個格子
    // (因為你的 SlotUI::SetUnit 裡面會自動去抓最新等級，所以只要重新 Set 一次就搞定了！)
    const auto& currentDeck = pData->GetDeck();
    for (int i = 0; i < 10; ++i) {
        m_Slots[i]->SetUnit(currentDeck[i]);
    }

    // 2. 重新讀取最新的解鎖名單，並更新下方冰箱
    // (DeckUI::LoadCats 會清空舊卡片並重新生成，自然就會抓到剛抽到的新貓跟最新等級)
    std::vector<UnitID> realCatList = pData->GetUnlockedCatsList();
    m_DeckUI->LoadCats(realCatList);

    LOG_DEBUG("DeckScene 資料已重新整理！");
}
void DeckScene::Update() {
    if (m_IsPopupActive) {
        // 當玩家點擊滑鼠左鍵(放開的那一瞬間)時，關閉視窗
        if (Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB)) {
            m_IsPopupActive = false;
        }
        return; // ⚠️ 關鍵：直接 return 結束 Update，底下的按鈕就點不到了！
    }
    if (m_ReturnBtn) {
        m_ReturnBtn->Update();
    }
    if (m_Upg) {
        m_Upg->Update();
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

    if (m_LeftBorder) m_LeftBorder->Update();
    if (m_RightBorder) m_RightBorder->Update();
    for (auto& slot : m_Slots) slot->Update();

    glm::vec2 mousePos = Util::Input::GetCursorPosition();
    bool isMouseDown = Util::Input::IsKeyPressed(Util::Keycode::MOUSE_LB);

    // ==========================================
    // 🚀 1. 新增：從上方 10 個格子「拔出」貓咪
    // ==========================================
    // 如果滑鼠按著，且「沒有」在拖拉任何東西，就檢查是不是按在上方格子上
    if (isMouseDown && !m_IsDraggingGhost) {
        int hoveredIndex = GetHoveredSlotIndex(mousePos);

        if (hoveredIndex != -1) {
            UnitID slotCat = m_Slots[hoveredIndex]->GetUnitID();

            if (static_cast<int>(slotCat) != 0) {
                m_IsDraggingGhost = true;
                m_DraggedUnitID = slotCat;
                m_DraggedFromSlot = hoveredIndex;

                // 拔起來 (UI 變空位)
                m_Slots[hoveredIndex]->SetUnit(static_cast<UnitID>(0));

                // 🚀 同步資料：這格現在空了，並存檔！
                PlayerData::GetInstance()->SetDeckUnit(hoveredIndex, static_cast<UnitID>(0));
                PlayerData::GetInstance()->SaveToFile();

                const UnitStats& stats = UnitData::Get(slotCat);
                m_DragGhostIcon = std::make_shared<Util::GameObject>(std::make_unique<Util::Image>(stats.iconPath), 90);
                m_DragGhostIcon->m_Transform.scale = { 1.5f, 1.5f };
            }
        }
    }

    // ==========================================
    // 2. 處理拖拉懸浮頭像與放開落點
    // ==========================================
    if (m_IsDraggingGhost) {
        if (m_DragGhostIcon) {
            m_DragGhostIcon->m_Transform.translation = mousePos;
        }

        if (!isMouseDown) { // 玩家放開滑鼠
            int targetSlotIndex = GetHoveredSlotIndex(mousePos);

            if (targetSlotIndex != -1) { // 丟進了某個格子

                if (m_DraggedFromSlot == -1) {
                    // 🌟 情況 A：從冰箱來的 -> 交給大總管去重複並裝備
                    EquipCatToSlot(m_DraggedUnitID, targetSlotIndex);
                }
                else {
                    // 🌟 情況 B：從上方別的格子來的 -> 執行精準的 A-B 互換！
                    UnitID displacedCat = m_Slots[targetSlotIndex]->GetUnitID(); // 記下目標格原本是誰(可能是B貓，也可能是空位0)

                    m_Slots[targetSlotIndex]->SetUnit(m_DraggedUnitID); // 把手上的 A 貓放進目標格

                    // 把原本的 B 貓，放回 A 貓被挖空的那個舊位置裡！
                    if (m_DraggedFromSlot != targetSlotIndex) {
                        m_Slots[m_DraggedFromSlot]->SetUnit(displacedCat);
                    }
                }
            } else {
                // 丟到了場外
                if (m_DraggedFromSlot != -1) {
                    // 因為點擊時已經先把它清空了，這裡不用做任何事，它就自然被卸除了！
                    LOG_DEBUG("丟到場外，貓咪解除裝備！");
                }
            }

            // 清理狀態
            m_IsDraggingGhost = false;
            m_DraggedUnitID = static_cast<UnitID>(0);
            m_DraggedFromSlot = -1; // 🚀 重置來源
            m_DragGhostIcon = nullptr;
        }
    }
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

    if (m_Upg) {
        m_Upg->Draw();
    }
    // 之後這裡放其他冰箱內按鈕的 Draw (如果要用 DrawRect 疊圖一樣寫在這裡)
    if (m_XPNumber) m_XPNumber->Draw();
    if (m_CatFoodNumber) m_CatFoodNumber->Draw();
    if (m_DeckUI) m_DeckUI->Draw();
    if (m_LeftBorder) {
        // 例如木頭柱子在圖片的 x=10, y=20, w=100, h=500
        m_LeftBorder->DrawRect(481, 0, 863-481, 77);
    }

    // 🚀 畫出右遮罩
    if (m_RightBorder) {
        // 如果左右柱子圖案一樣，這裡可以直接代入一樣的 xywh
        m_RightBorder->DrawRect(481, 0, 863-481, 77);
    }
    for (auto& slot : m_Slots) slot->Draw();
    if (m_DragGhostIcon && m_IsDraggingGhost) {
        m_DragGhostIcon->Draw();
    }
    if (m_IsPopupActive ) {
        if ( m_EmptyDeckPopup) {
        m_EmptyDeckPopup->Draw();
        }
        if (m_PopupBg) m_PopupBg->Draw();
    }
}