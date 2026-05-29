#include "StorageScene.hpp"
#include "Core/Context.hpp"
#include <iostream>
#include <algorithm>
#include <filesystem>
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

StorageScene::StorageScene() {
    auto playerData = PlayerData::GetInstance();
    m_XP = playerData->GetXP();
    m_CatFood = playerData->GetCatFood();

    auto bgImage = std::make_shared<Util::Image>(RESOURCE_DIR"/img/StorageScene.png");
    m_Background.SetDrawable(bgImage);
    m_Background.SetZIndex(10);

    auto context = Core::Context::GetInstance();
    float windowWidth = (float)context->GetWindowWidth();
    float windowHeight = (float)context->GetWindowHeight();
    glm::vec2 imgSize = bgImage->GetSize();

    m_Background.m_Transform.scale = glm::vec2(windowWidth / imgSize.x, windowHeight / imgSize.y);
    auto blackImg = std::make_shared<Util::Image>(RESOURCE_DIR "/img/black.png");
    m_Background_black.SetDrawable(blackImg);
    m_Background_black.SetZIndex(49);
    m_Background_black.m_Transform.scale = glm::vec2(windowWidth / blackImg->GetSize().x, windowHeight / blackImg->GetSize().y);

    const std::string img006Path = RESOURCE_DIR"/img/img006_tw.png";
    const std::string img007Path = RESOURCE_DIR"/img/img007_tw.png";
    const std::string yesOrNoPath = RESOURCE_DIR"/img/yesOrNo.png";

    float btnSize = 100.0f;

    m_ReturnBtn = std::make_shared<Button>(
        -0.82f, -0.87f, btnSize, btnSize,
        img006Path, " ", 30, Util::Color(255, 255, 255, 255)
    );
    m_ReturnBtn->SetZIndex(20);
    m_ReturnBtn->SetOnClick([this]() {
        if (m_OnReturn) m_OnReturn();
    });

    m_XPNumber = std::make_shared<NumberSystem>(0.86f, 0.94f, 29.0f, 38.0f, RESOURCE_DIR"/img/moneyInfo.png");
    m_XPNumber->SetValue(m_XP);

    m_CatFoodNumber = std::make_shared<NumberSystem>(0.86f, -0.94f, 29.0f, 38.0f, RESOURCE_DIR"/img/moneyInfo.png");
    m_CatFoodNumber->SetValue(m_CatFood);

    m_cat_count = std::make_shared<NumberSystem>(
        0.78f, -0.82f, 15.0f, 19.0f,
        RESOURCE_DIR"/img/moneyInfo.png", NumberSystem::FontType::WHITE_SMALL
    );

    auto img007 = std::make_shared<Util::Image>(img007Path);
    glm::vec2 sheetSize007 = img007->GetSize();
    float b_size_w_1 = 185.0f, b_size_h_1 = 40.0f, b_size_w_2 = 225.0f, b_y = -0.94f;

    // ==========================================
    // 👑 修正一：「全部使用」按鈕，改為迴圈一次全部解鎖
    // ==========================================
    m_ToUse = std::make_shared<Button>(-0.05f, b_y, b_size_w_1, b_size_h_1, img007Path, " ", 30, Util::Color(255, 255, 255, 255));
    m_ToUse->SetZIndex(1);
    m_ToUse->SetScale(b_size_w_1 / sheetSize007.x, b_size_h_1 / sheetSize007.y);
    m_ToUse->SetOnClick([this]() {
        if (m_StorageItems.empty()) return;
        auto playerData = PlayerData::GetInstance();

        // 1. 迴圈全部解鎖
        for (auto& item : m_StorageItems) {
            if (item) playerData->UnlockOrUpgradeCat(item->GetCatID());
        }

        // 2. 由後往前清空真實冰箱
        for (int i = static_cast<int>(m_StorageItems.size()) - 1; i >= 0; --i) {
            playerData->RemoveFromFridge(i);
        }

        playerData->SaveToFile();

        m_StorageItems.clear(); // 畫面清空
        if (m_cat_count) m_cat_count->SetValue(0);
        if (m_OnToUse) m_OnToUse();
    });

    // ==========================================
    // 💰 修正：「全部換 XP」按鈕邏輯
    // ==========================================
    m_ToXp = std::make_shared<Button>(0.2825f, b_y, b_size_w_2, b_size_h_1, img007Path, " ", 30, Util::Color(255, 255, 255, 255));
    m_ToXp->SetZIndex(1);
    m_ToXp->SetScale(b_size_w_2 / sheetSize007.x, b_size_h_1 / sheetSize007.y);
    m_ToXp->SetOnClick([this]() {
        if (m_StorageItems.empty()) return;
        auto playerData = PlayerData::GetInstance();
        int totalXpEarned = 0;

        for (auto& item : m_StorageItems) {
            if (item) totalXpEarned += UnitData::Get(item->GetCatID()).rank * 10000;
        }

        playerData->AddXP(totalXpEarned);
        for (int i = static_cast<int>(m_StorageItems.size()) - 1; i >= 0; --i) {
            playerData->RemoveFromFridge(i);
        }
        playerData->SaveToFile();

        m_XP = playerData->GetXP();
        if (m_XPNumber) m_XPNumber->SetValue(m_XP);
        m_StorageItems.clear();
        if (m_cat_count) m_cat_count->SetValue(0);
        if (m_OnToXp) m_OnToXp();
    });

    float popup_btn_w = 225.0f, popup_btn_h = 40.0f;

    m_PopupUseBtn = std::make_shared<Button>(-0.2f, -0.3f, popup_btn_w - 40.0f, popup_btn_h - 40.0f, RESOURCE_DIR"/img/UseBtn.png", " ", 30, Util::Color(255, 255, 255, 255));
    m_PopupUseBtn->SetZIndex(50);

    auto removeSelectedCat = [this]() -> bool {
        if (!m_SelectedPopupCat) return false;
        auto playerData = PlayerData::GetInstance();
        for (size_t i = 0; i < m_StorageItems.size(); ++i) {
            if (m_StorageItems[i] == m_SelectedPopupCat) {
                playerData->RemoveFromFridge(static_cast<int>(i));
                m_StorageItems.erase(m_StorageItems.begin() + i);
                return true;
            }
        }
        return false;
    };

    m_PopupUseBtn->SetOnClick([this, removeSelectedCat]() {
        auto playerData = PlayerData::GetInstance();
        if (m_SelectedPopupCat) {
            UnitID id = m_SelectedPopupCat->GetCatID();
            if (removeSelectedCat()) {
                playerData->UnlockOrUpgradeCat(id);
                playerData->SaveToFile();
                if (m_OnToUse) m_OnToUse();
                if (m_cat_count) m_cat_count->SetValue(static_cast<int>(m_StorageItems.size()));
            }
        }
        m_IsPopupOpen = false;
        m_SelectedPopupCat = nullptr;
        m_PopupDisplayBody = nullptr;
        m_PopupDisplayLegs = nullptr;
    });

    m_PopupXpBtn = std::make_shared<Button>(0.2f, -0.3f, popup_btn_w, popup_btn_h, RESOURCE_DIR "/img/ToXpBtnpng.png", " ", 30, Util::Color(255, 255, 255, 255));
    m_PopupXpBtn->SetZIndex(50);
    m_PopupXpBtn->SetOnClick([this, removeSelectedCat]() {
        auto playerData = PlayerData::GetInstance();
        if (m_SelectedPopupCat) {
            UnitID id = m_SelectedPopupCat->GetCatID();
            if (removeSelectedCat()) {
                int earnXp = UnitData::Get(id).rank * 10000;
                playerData->AddXP(earnXp);
                playerData->SaveToFile();
                m_XP = playerData->GetXP();
                if (m_XPNumber) m_XPNumber->SetValue(m_XP);
                if (m_OnToXp) m_OnToXp();
                if (m_cat_count) m_cat_count->SetValue(static_cast<int>(m_StorageItems.size()));
            }
        }
        m_IsPopupOpen = false;
        m_SelectedPopupCat = nullptr;
        m_PopupDisplayBody = nullptr;
        m_PopupDisplayLegs = nullptr;
    });

    m_PopupCancelBtn = std::make_shared<Button>(0.3f, -0.1f, 150.0f, 40.0f, yesOrNoPath, " ", 30, Util::Color(255, 255, 255, 255));
    m_PopupCancelBtn->SetZIndex(50);
    m_PopupCancelBtn->SetOnClick([this]() {
        m_IsPopupOpen = false;
        m_SelectedPopupCat = nullptr;
        m_PopupDisplayBody = nullptr;
        m_PopupDisplayLegs = nullptr;
    });

    // 備註：建構子中的貓咪載入我們移到 Update 裡自動處理了！
}

void StorageScene::SetOnReturnBtnClick(std::function<void()> callback) { m_OnReturn = callback; }
void StorageScene::SetOnToUse(std::function<void()> callback) { m_OnToUse = callback; }
void StorageScene::SetOnToXp(std::function<void()> callback) { m_OnToXp = callback; }

void StorageScene::Update() {
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

    // ==========================================
    // 👑 修正二：絕對防禦同步系統！(解決幽靈貓Bug)
    // 只要發現真假冰箱大小不同，或是裡面的貓咪對不上，瞬間重組畫面！
    // ==========================================
    bool needsRebuild = false;
    const auto& realFridge = pData->GetFridge();

    if (m_StorageItems.size() != realFridge.size()) {
        needsRebuild = true;
    } else {
        for (size_t i = 0; i < realFridge.size(); ++i) {
            if (m_StorageItems[i]->GetCatID() != realFridge[i]) {
                needsRebuild = true;
                break;
            }
        }
    }

    // 當發現不同步，且目前沒有打開彈窗干擾時，立刻重組
    if (needsRebuild && !m_IsPopupOpen) {
        m_StorageItems.clear();
        int maxCatsToLoad = std::min(static_cast<int>(realFridge.size()), 200);

        for (int i = 0; i < maxCatsToLoad; ++i) {
            auto cat = std::make_shared<StorageItem>(0.0f, 0.0f, realFridge[i]);

            cat->SetOnSelectCallback([this](StorageItem* clickedCat) {
                if (!m_IsPopupOpen) {
                    m_IsPopupOpen = true;
                    m_LockPopupClick = true;
                    m_PopupDisplayBody = nullptr;
                    m_PopupDisplayLegs = nullptr;

                    for(auto& c : m_StorageItems) {
                        if (c.get() == clickedCat) {
                            m_SelectedPopupCat = c;
                            break;
                        }
                    }

                    // 🌟 巨神貓特判：生成彈窗身體與腳部
                    if (m_SelectedPopupCat && m_SelectedPopupCat->GetCatID() == UnitID::GaintCat) {
                        UnitID id = m_SelectedPopupCat->GetCatID();
                        auto& stats = UnitData::Get(id);

                        std::string path = stats.imgPath;
                        size_t lastSlash = path.find_last_of("/\\");
                        std::string filename = path.substr(lastSlash + 1);
                        size_t dotPos = filename.find_last_of(".");
                        if (dotPos != std::string::npos) filename = filename.substr(0, dotPos);

                        std::string baseDir = path.substr(0, path.find_last_of("/\\"));
                        baseDir = baseDir.substr(0, baseDir.find_last_of("/\\"));
                        std::string imgcutPath = baseDir + "/imgcut/" + filename + ".imgcut";

                        if (std::filesystem::exists(imgcutPath)) {
                            std::vector<SpriteFrame> allFrames = ParseImgCut(imgcutPath);
                            if (allFrames.size() >= 2) {
                                float sheetWidth = std::make_shared<Util::Image>(stats.imgPath)->GetSize().x;
                                float sheetHeight = std::make_shared<Util::Image>(stats.imgPath)->GetSize().y;

                                float showCat = 100.0f;
                                float baseScale = showCat / std::max((float)allFrames[0].w, (float)allFrames[0].h);
                                float finalScale = baseScale * 2.5f;

                                m_PopupDisplayBody = std::make_shared<Button>(0.0f, 0.4f, 100.0f, 100.0f, stats.imgPath, " ", 30, Util::Color(255, 255, 255, 255));
                                m_PopupDisplayBody->SetClipRect(allFrames[0].x, allFrames[0].y, allFrames[0].w, allFrames[0].h - 3);
                                m_PopupDisplayBody->SetScale((allFrames[0].w * finalScale) / sheetWidth, (allFrames[0].h * finalScale) / sheetHeight);

                                m_PopupDisplayLegs = std::make_shared<Button>(0.0f, 0.40f, 100.0f, 100.0f, stats.imgPath, " ", 30, Util::Color(255, 255, 255, 255));
                                m_PopupDisplayLegs->SetClipRect(allFrames[1].x, allFrames[1].y, allFrames[1].w, allFrames[1].h);
                                m_PopupDisplayLegs->SetScale((allFrames[1].w * finalScale) / sheetWidth, (allFrames[1].h * finalScale) / sheetHeight);
                                m_PopupDisplayLegs->SetZIndex(80);

                                float baseY = m_PopupDisplayLegs->m_Transform.translation.y - 140.0f;
                                m_PopupDisplayLegs->m_Transform.translation.y = baseY + ((allFrames[1].h / 2.0f) * finalScale);

                                m_PopupDisplayBody->m_Transform.translation.x = m_PopupDisplayLegs->m_Transform.translation.x + (-3.0f * finalScale);
                                m_PopupDisplayBody->m_Transform.translation.y = baseY + ((allFrames[0].h / 2.0f) * finalScale) + (15.0f * finalScale);
                                m_PopupDisplayBody->SetZIndex(81);
                            }
                        }
                    }
                }
            });
            m_StorageItems.push_back(cat);
        }
        if (m_cat_count) m_cat_count->SetValue(static_cast<int>(m_StorageItems.size()));
    }


    if (m_IsPopupOpen) {
        if (m_LockPopupClick) {
            if (!Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB)) {
                m_LockPopupClick = false;
            }
        }
        else {
            if (m_PopupUseBtn) m_PopupUseBtn->Update();
            if (m_PopupXpBtn) m_PopupXpBtn->Update();
            if (m_PopupCancelBtn) m_PopupCancelBtn->Update();
        }
    }
    else {
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

    static auto yesOrNoAtlas = std::make_shared<Util::Image>(RESOURCE_DIR"/img/yesOrNo.png");
    glm::vec2 sheetSizeyesOrNo = yesOrNoAtlas->GetSize();

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

    if (m_XPNumber) m_XPNumber->Draw();
    if (m_CatFoodNumber) m_CatFoodNumber->Draw();
    if (m_cat_count) m_cat_count->Draw();

    if (m_ToUse) m_ToUse->DrawRect(Cut::SPAN_BTN_X, Cut::SPAN_BTN_Y, Cut::SPAN_BTN_W, Cut::SPAN_BTN_H);
    if (m_ToXp) m_ToXp->DrawRect(Cut::SPAN_BTN_X, Cut::SPAN_BTN_Y, Cut::SPAN_BTN_W, Cut::SPAN_BTN_H);
    if (m_TestCat) m_TestCat->Draw();

    for (auto& item : m_StorageItems) {
        if (item) item->Draw();
    }

    if (m_IsPopupOpen && m_SelectedPopupCat) {
        m_Background_black.Draw();

        // 🚀 核心繪製：套用扭蛋機的巨神貓判斷！
        if (m_PopupDisplayBody) {
            if (m_PopupDisplayLegs) m_PopupDisplayLegs->Draw();
            m_PopupDisplayBody->Draw();
        } else {
            m_SelectedPopupCat->DrawForPopup();
        }

        if (m_PopupUseBtn) m_PopupUseBtn->Draw();
        if (m_PopupXpBtn) m_PopupXpBtn->Draw();
        if (m_PopupCancelBtn) {
            float popupBaseScale = 1.2f;
            m_PopupCancelBtn->m_Transform.scale = {
                ((float)Cut::POPUP_CANCEL_TXT_W / sheetSizeyesOrNo.x) * popupBaseScale,
                ((float)Cut::POPUP_CANCEL_TXT_H / sheetSizeyesOrNo.y) * popupBaseScale
            };
            m_PopupCancelBtn->SetZIndex(50);
            m_PopupCancelBtn->DrawRect(Cut::POPUP_CANCEL_TXT_X, Cut::POPUP_CANCEL_TXT_Y, Cut::POPUP_CANCEL_TXT_W, Cut::POPUP_CANCEL_TXT_H);
        }
    }
}