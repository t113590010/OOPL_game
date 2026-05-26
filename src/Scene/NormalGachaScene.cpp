#include "NormalGachaScene.hpp"
#include "Core/Context.hpp"

namespace Cut {
    const float RETURN_ICON_X = 423.0f, RETURN_ICON_Y = 170.0f, RETURN_ICON_W = 511.0f - RETURN_ICON_X, RETURN_ICON_H = 232.0f-RETURN_ICON_Y;
    const float RETURN_BASE_X = 256.0f, RETURN_BASE_Y = 0.0f, RETURN_BASE_W = 99.0f, RETURN_BASE_H = 97.0f;
    const float STORAGE_X = 0.0f, STORAGE_Y = 0.0f, STORAGE_W = 72.0f, STORAGE_H = 82.0f;
    const float SPAN_BTN_X = 297.0f, SPAN_BTN_Y = 426.0f, SPAN_BTN_W = 511.0f-SPAN_BTN_X, SPAN_BTN_H = 508.0f-SPAN_BTN_Y;
    const float CATFOOD_ICON_X = 161.0f, CATFOOD_ICON_Y = 158.0f, CATFOOD_ICON_W = 219.0f - CATFOOD_ICON_X, CATFOOD_ICON_H = 203.0f-CATFOOD_ICON_Y;
    const float POPUP_CANCEL_TXT_X = 446.0f, POPUP_CANCEL_TXT_Y = 0.0f, POPUP_CANCEL_TXT_W = 512.0f-POPUP_CANCEL_TXT_X, POPUP_CANCEL_TXT_H = 63.0f;
}

UnitID PullOneCat() {
    std::vector<UnitID> pool = {
        UnitID::CAT,
        UnitID::AXE_CAT,
        UnitID::CowCat,
        UnitID::FishCat,
        UnitID::LONG_LEG_CAT,
        UnitID::GaintCat,
        UnitID::FlyCat,
        UnitID::LongCat,
        UnitID::DinoCat,
        UnitID::bighead
    };
    return pool[rand() % pool.size()];
}

NormalGachaScene::NormalGachaScene() {
    auto context = Core::Context::GetInstance();
    float windowWidth = (float)context->GetWindowWidth();
    float windowHeight = (float)context->GetWindowHeight();

    auto bgImage = std::make_shared<Util::Image>(RESOURCE_DIR"/img/NormalGachaScene.png");
    m_Background.SetDrawable(bgImage);
    m_Background.SetZIndex(10);
    m_Background.m_Transform.scale = glm::vec2(windowWidth / bgImage->GetSize().x, windowHeight / bgImage->GetSize().y);

    auto blackImg = std::make_shared<Util::Image>(RESOURCE_DIR "/img/black.png");
    m_Background_black.SetDrawable(blackImg);
    m_Background_black.SetZIndex(75); // 確保在背景之上，彈窗元件之下
    m_Background_black.m_Transform.scale = glm::vec2(windowWidth / blackImg->GetSize().x, windowHeight / blackImg->GetSize().y);

    const std::string img006Path = RESOURCE_DIR"/img/img006_tw.png";
    const std::string img007Path = RESOURCE_DIR"/img/img007_tw.png";
    const std::string atlasPath = RESOURCE_DIR"/img/img010_tw.png";

    m_ReturnBtn = std::make_shared<Button>(-0.82f, -0.87f, 100.0f, 100.0f, img006Path, " ", 30, Util::Color(255, 255, 255, 255));
    m_ReturnBtn->SetZIndex(20);
    m_ReturnBtn->SetOnClick([this]() { if (m_OnReturn) m_OnReturn(); });

    m_StorageBtn = std::make_shared<Button>(-0.66, -0.72 , 85, 65, atlasPath, " ", 30, Util::Color(255, 255, 255, 255));
    m_StorageBtn->SetZIndex(20);
    m_StorageBtn->SetOnClick([this]() { if (m_OnStorageBtnClick) m_OnStorageBtnClick(); });

    float spanOffset_X = 0.11f, spanOffset_Y = -0.73f, spanOffset2_Y = -0.77f;

    m_span = std::make_shared<Button>(0.33f, spanOffset_Y, 300.0f, 86.0f, img007Path, " ", 30, Util::Color(255, 255, 255, 255));
//     m_span = std::make_shared<Button>(
//     0.33f, spanOffset_Y,
//     Cut::SPAN_BTN_W, Cut::SPAN_BTN_H, // 👈 這裡一定要用對切圖大小
//     img007Path, " ", 30, Util::Color(255, 255, 255, 255)
// );


    m_span->SetZIndex(1);
    m_span->SetOnClick([this]() {


        if (m_State != GachaState::IDLE) return;

        if (m_Tickets > 0) m_Tickets--; else if (m_CatFood >= 150) m_CatFood -= 150;
        else {
            Util::SFX(RESOURCE_DIR "/music/fail_summon_cat.mp3").Play();

            return;
        };
        Util::SFX(RESOURCE_DIR "/music/clickbtn.mp3").Play();

        Util::SFX(RESOURCE_DIR "/music/spin.mp3").Play();

        UpdateGachaButtons();
        if (m_CatFoodNumber) m_CatFoodNumber->SetValue(m_CatFood);

        m_PulledCats.clear();
        m_PulledCats.push_back(PullOneCat());
        m_State = GachaState::ANIMATING;
        m_AnimTimer = 0;
        if (m_Onspan) m_Onspan();
    });
    // m_muti_span = std::make_shared<Button>(
    //       (m_span->GetTransform().scale.x) + spanOffset_X, spanOffset_Y,
    //       Cut::SPAN_BTN_W, Cut::SPAN_BTN_H, // 👈 這裡也要對應
    //       img007Path, " ", 30, Util::Color(255, 255, 255, 255)
    //   );
    m_muti_span = std::make_shared<Button>(m_span->GetTransform().scale.x+spanOffset_X, spanOffset_Y, 300.0f, 86.0f, img007Path, " ", 30, Util::Color(255, 255, 255, 255));
    m_muti_span->SetZIndex(1);
    m_muti_span->SetOnClick([this]() {
        if (m_State != GachaState::IDLE) return;

        int pullCount = 0;
        if (m_Tickets > 0) { pullCount = (m_Tickets > 10) ? 10 : m_Tickets; m_Tickets -= pullCount; }
        else if (m_CatFood >= 1500) { pullCount = 10; m_CatFood -= 1500; }   else {
            Util::SFX(RESOURCE_DIR "/music/fail_summon_cat.mp3").Play();

            return;
        };
        Util::SFX(RESOURCE_DIR "/music/clickbtn.mp3").Play();

        Util::SFX(RESOURCE_DIR "/music/spin.mp3").Play();

        UpdateGachaButtons();
        if (m_CatFoodNumber) m_CatFoodNumber->SetValue(m_CatFood);

        m_PulledCats.clear();
        for (int i = 0; i < pullCount; ++i) m_PulledCats.push_back(PullOneCat());
        m_State = GachaState::ANIMATING;
        m_AnimTimer = 0;
        if (m_On_muti_span) m_On_muti_span();
    });

    m_XPNumber = std::make_shared<NumberSystem>(0.86f, 0.94f, 29.0f, 38.0f, RESOURCE_DIR"/img/moneyInfo.png"); m_XPNumber->SetValue(m_XP);
    m_CatFoodNumber = std::make_shared<NumberSystem>(0.86f, -0.94f, 29.0f, 38.0f, RESOURCE_DIR"/img/moneyInfo.png"); m_CatFoodNumber->SetValue(m_CatFood);
    m_TicketNumber = std::make_shared<NumberSystem>(0.4f, -0.94f, 29.0f, 36.0f, RESOURCE_DIR"/img/moneyInfo.png"); m_TicketNumber->SetValue(m_Tickets);
    m_SpanCostNumber = std::make_shared<NumberSystem>(0.475f, spanOffset2_Y, 29.0f, 38.0f, RESOURCE_DIR"/img/moneyInfo.png");
    m_MutiSpanCostNumber = std::make_shared<NumberSystem>(0.835f, spanOffset2_Y, 29.0f, 38.0f, RESOURCE_DIR"/img/moneyInfo.png");

    m_SpanCatFoodIcon = std::make_shared<Button>(0.32f, spanOffset2_Y, 40.0f, 40.0f, img006Path, " ", 30, Util::Color(255, 255, 255, 255));
    m_MutiSpanCatFoodIcon = std::make_shared<Button>(0.65f, spanOffset2_Y, 40.0f, 40.0f, img006Path, " ", 30, Util::Color(255, 255, 255, 255));

    for (int i = 0; i < 69; ++i) {
        std::string path = RESOURCE_DIR "/img/RareSpan_Fixed/RareSpan-" + std::to_string(i) + ".png";
        if (std::filesystem::exists(path)) m_AnimFrames.push_back(std::make_shared<Util::Image>(path));
    }
    if (!m_AnimFrames.empty()) {
        m_AnimObject.SetDrawable(m_AnimFrames[0]);
        glm::vec2 animImgSize = m_AnimFrames[0]->GetSize();
        m_AnimObject.m_Transform.scale = { windowWidth / animImgSize.x, windowHeight / animImgSize.y };
    }
    m_AnimObject.SetZIndex(90);
    m_AnimObject.m_Transform.translation = { 0.0f, 0.0f };

    // 初始化功能按鈕
    float popup_btn_w = 225.0f, popup_btn_h = 40.0f;
    const std::string yesOrNoPath = RESOURCE_DIR"/img/yesOrNo.png";

    m_PopupUseBtn = std::make_shared<Button>(-0.2f, -0.3f, popup_btn_w-40, popup_btn_h-40, RESOURCE_DIR"/img/UseBtn.png", " ", 30, Util::Color(255, 255, 255, 255));
    m_PopupUseBtn->SetZIndex(80);
    m_PopupUseBtn->SetOnClick([this]() {
        m_CurrentResultIndex++; // 下一隻
        ShowNextResultCat();
    });

    m_PopupXpBtn = std::make_shared<Button>(0.2f, -0.3f, popup_btn_w, popup_btn_h, RESOURCE_DIR"/img/ToXpBtnpng.png", " ", 30, Util::Color(255, 255, 255, 255));
    m_PopupXpBtn->SetZIndex(80);
    m_PopupXpBtn->SetOnClick([this]() {
        m_XP += 10000;
        if (m_XPNumber) m_XPNumber->SetValue(m_XP);
        m_CurrentResultIndex++; // 下一隻
        ShowNextResultCat();
    });

    m_PopupCancelBtn = std::make_shared<Button>(0.3f, -0.1f, 150.0f, 40.0f, yesOrNoPath, " ", 30, Util::Color(255, 255, 255, 255));
    m_PopupCancelBtn->SetZIndex(80);
    m_PopupCancelBtn->SetOnClick([this]() {
        m_CurrentResultIndex++; // 下一隻
        ShowNextResultCat();
    });

    UpdateGachaButtons();
}

// ==========================================
// 🚀 彈窗序列生成核心邏輯（包含巨神貓特判下半身）
// ==========================================
void NormalGachaScene::ShowNextResultCat() {
    if (m_CurrentResultIndex < m_PulledCats.size()) {
        Util::SFX(RESOURCE_DIR "/music/get_unit_item.mp3").Play();
        
        UnitID id = m_PulledCats[m_CurrentResultIndex];
        auto& stats = UnitData::Get(id);

        // 1. 生成預設的 StorageItem（維持原有結構）
        m_SelectedPopupCat = std::make_shared<StorageItem>(0.0f, 0.0f, id);
        m_IsPopupOpen = true;

        m_PopupDisplayLegs = nullptr;
        m_PopupDisplayBody = nullptr;

        // 2. 🌟 巨神貓特判：精準對齊你的骨架系統，手動幫彈窗刻出完整的下半身！
        std::string path = stats.imgPath;
        size_t lastSlash = path.find_last_of("/\\");
        std::string filename = path.substr(lastSlash + 1);
        size_t dotPos = filename.find_last_of(".");
        if (dotPos != std::string::npos) filename = filename.substr(0, dotPos);
        std::string baseDir = path.substr(0, path.find_last_of("/\\"));
        baseDir = baseDir.substr(0, baseDir.find_last_of("/\\"));
        std::string imgcutPath = baseDir + "/imgcut/" + filename + ".imgcut";

        // 🌟 巨神貓特判：手動幫它縫合下半身
        if (id == UnitID::GaintCat) {
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

                    // 🚨 這裡就是之前害你看不見巨神貓的元凶！
                    // 我現在完全 100% 照抄你 StorageItem 建構子裡的放大比例算法！
                    float showCat = 100.0f;
                    float baseScale = showCat / std::max((float)allFrames[0].w, (float)allFrames[0].h);
                    float finalScale = baseScale * 2.5f; // 真真實實的放大 2.5 倍！

                    // 1. 建立身體
                    m_PopupDisplayBody = std::make_shared<Button>(0.0f, 0.4f, 100.0f, 100.0f, stats.imgPath, " ", 30, Util::Color(255, 255, 255, 255));
                    m_PopupDisplayBody->SetClipRect(allFrames[0].x, allFrames[0].y, allFrames[0].w, allFrames[0].h-3);
                    m_PopupDisplayBody->SetScale((allFrames[0].w * finalScale) / sheetWidth, (allFrames[0].h * finalScale) / sheetHeight);

                    // 2 建立腳部 (0.0f, 0.4f 完全對齊冰箱彈窗的高度)
                    m_PopupDisplayLegs = std::make_shared<Button>(0.0f, 0.40f, 100.0f, 100.0f, stats.imgPath, " ", 30, Util::Color(255, 255, 255, 255));
                    m_PopupDisplayLegs->SetClipRect(allFrames[1].x, allFrames[1].y, allFrames[1].w, allFrames[1].h);
                    m_PopupDisplayLegs->SetScale((allFrames[1].w * finalScale) / sheetWidth, (allFrames[1].h * finalScale) / sheetHeight);
                    m_PopupDisplayLegs->SetZIndex(80);

                    // 用你測過最完美的 -100.0f 往下沉
                    float baseY = m_PopupDisplayLegs->m_Transform.translation.y - 140.0f;
                    m_PopupDisplayLegs->m_Transform.translation.y = baseY + ((allFrames[1].h / 2.0f) * finalScale);


                    // 完美套用你的 Offset: bodyX = -3.0f, bodyY = 15.0f
                    m_PopupDisplayBody->m_Transform.translation.x = m_PopupDisplayLegs->m_Transform.translation.x + (-3.0f * finalScale);
                    m_PopupDisplayBody->m_Transform.translation.y = baseY + ((allFrames[0].h / 2.0f) * finalScale) + (15.0f * finalScale);
                    m_PopupDisplayBody->SetZIndex(81);
                }
            }

        }
    } else {
        // 全部點完，功德圓滿，回到主抽卡畫面
        m_State = GachaState::IDLE;
        m_IsPopupOpen = false;
        m_SelectedPopupCat = nullptr;
        m_PopupDisplayLegs = nullptr;
        m_PopupDisplayBody = nullptr;
        m_PulledCats.clear();
    }
}

void NormalGachaScene::UpdateGachaButtons() {
    if (m_TicketNumber) m_TicketNumber->SetValue(m_Tickets);
    if (m_Tickets > 0) {
        m_SpanCostNumber->SetValue(1);
        m_MutiSpanCostNumber->SetValue((m_Tickets > 10) ? 10 : m_Tickets);
    } else {
        m_SpanCostNumber->SetValue(150);
        m_MutiSpanCostNumber->SetValue(1500);
    }
}

void NormalGachaScene::SetOnReturnBtnClick(std::function<void()> callback) { m_OnReturn = callback; }
void NormalGachaScene::SetOnStorageBtnClick(std::function<void()> callback) { m_OnStorageBtnClick = callback; }
void NormalGachaScene::SetOnspanClick(std::function<void()> callback) { m_Onspan = callback; }
void NormalGachaScene::SetOn_muti_spanClick(std::function<void()> callback) { m_On_muti_span = callback; }

void NormalGachaScene::Update() {
    if (m_State == GachaState::IDLE) {
        if (m_ReturnBtn) m_ReturnBtn->Update();
        if (m_StorageBtn) m_StorageBtn->Update();
        if (m_span) m_span->Update();
        if (m_muti_span) m_muti_span->Update();
    }
    else if (m_State == GachaState::ANIMATING) {
        m_AnimTimer++;
        int currentFrame = m_AnimTimer / animSpeed;

        if (currentFrame >= m_AnimFrames.size()) {
            m_State = GachaState::RESULT;
            m_CurrentResultIndex = 0;
            ShowNextResultCat(); // 動畫一停，立刻砸出第一隻貓的個別介面
        }
    }
    else if (m_State == GachaState::RESULT) {

        if (m_IsPopupOpen) {
            if (m_PopupUseBtn) m_PopupUseBtn->Update();
            if (m_PopupXpBtn) m_PopupXpBtn->Update();
            if (m_PopupCancelBtn) m_PopupCancelBtn->Update();
        }
    }
}

void NormalGachaScene::Draw() {
    m_Background.Draw();
    static auto uiAtlas = std::make_shared<Util::Image>(RESOURCE_DIR"/img/img006_tw.png");
    glm::vec2 sheetSize = uiAtlas->GetSize();
    static auto img007Atlas = std::make_shared<Util::Image>(RESOURCE_DIR"/img/img007_tw.png");
    glm::vec2 sheetSize007 = img007Atlas->GetSize();
    float iconBoost = 0.8f, baseScale = 1.0f;


    if (m_ReturnBtn) {
        m_ReturnBtn->m_Transform.scale = { ((float)Cut::RETURN_BASE_W / sheetSize.x) * baseScale, ((float)Cut::RETURN_BASE_H / sheetSize.y) * baseScale };
        m_ReturnBtn->SetZIndex(21);
        m_ReturnBtn->DrawRect(Cut::RETURN_BASE_X, Cut::RETURN_BASE_Y, Cut::RETURN_BASE_W, Cut::RETURN_BASE_H);

        glm::vec2 originalPos = m_ReturnBtn->m_Transform.translation;
        m_ReturnBtn->m_Transform.translation += glm::vec2(-0.2f, 0.0f);
        m_ReturnBtn->m_Transform.scale = { ((float)Cut::RETURN_ICON_W / sheetSize.x) * baseScale * iconBoost, ((float)Cut::RETURN_ICON_H / sheetSize.y) * baseScale * iconBoost };
        m_ReturnBtn->SetZIndex(22);
        m_ReturnBtn->DrawRect(Cut::RETURN_ICON_X, Cut::RETURN_ICON_Y, Cut::RETURN_ICON_W, Cut::RETURN_ICON_H);

        m_ReturnBtn->m_Transform.translation = originalPos;
        m_ReturnBtn->m_Transform.scale = { ((float)Cut::RETURN_BASE_W / sheetSize.x) * baseScale, ((float)Cut::RETURN_BASE_H / sheetSize.y) * baseScale };
    }

    if (m_StorageBtn) {
        m_StorageBtn->m_Transform.scale = { ((float)Cut::STORAGE_W / sheetSize.x) * 0.8f, ((float)Cut::STORAGE_H / sheetSize.y) * 0.8f };
        m_StorageBtn->SetZIndex(1);
        m_StorageBtn->DrawRect(Cut::STORAGE_X, Cut::STORAGE_Y, Cut::STORAGE_W, Cut::STORAGE_H);
    }

    if (m_XPNumber) m_XPNumber->Draw();
    if (m_CatFoodNumber) m_CatFoodNumber->Draw();
    if (m_TicketNumber) m_TicketNumber->Draw();
    if (m_SpanCostNumber) m_SpanCostNumber->Draw();

    if (m_Tickets > 0) {
        if (m_MutiSpanCostNumber) m_MutiSpanCostNumber->Draw();
        if (m_span) {
            m_span->m_Transform.scale = { ((float)Cut::SPAN_BTN_W / sheetSize007.x) * 1.0f, ((float)Cut::SPAN_BTN_H / sheetSize007.y) * 1.0f };
            m_span->SetZIndex(1);
            // m_span->SetZIndex(21);
            m_span->DrawRect(Cut::SPAN_BTN_X, Cut::SPAN_BTN_Y, Cut::SPAN_BTN_W, Cut::SPAN_BTN_H);
        }
        if (m_muti_span) {
            m_muti_span->m_Transform.scale = { ((float)Cut::SPAN_BTN_W / sheetSize007.x) * 1.0f, ((float)Cut::SPAN_BTN_H / sheetSize007.y) * 1.0f };
            // m_muti_span->SetZIndex(21);
            m_muti_span->SetZIndex(1);

            m_muti_span->DrawRect(Cut::SPAN_BTN_X, Cut::SPAN_BTN_Y, Cut::SPAN_BTN_W, Cut::SPAN_BTN_H);
        }
    } else {
        float catFoodScale = 0.8f;
        if (m_span) {
            m_span->m_Transform.scale = { ((float)Cut::SPAN_BTN_W / sheetSize007.x) * 1.0f, ((float)Cut::SPAN_BTN_H / sheetSize007.y) * 1.0f };
            // m_span->SetZIndex(99);
            m_span->SetZIndex(21);
            m_span->DrawRect(Cut::SPAN_BTN_X, Cut::SPAN_BTN_Y, Cut::SPAN_BTN_W, Cut::SPAN_BTN_H);
        }
        if (m_muti_span) {
            m_muti_span->m_Transform.scale = { ((float)Cut::SPAN_BTN_W / sheetSize007.x) * 1.0f, ((float)Cut::SPAN_BTN_H / sheetSize007.y) * 1.0f };
            m_muti_span->SetZIndex(21);
            // m_muti_span->SetZIndex(99);

            m_muti_span->DrawRect(Cut::SPAN_BTN_X, Cut::SPAN_BTN_Y, Cut::SPAN_BTN_W, Cut::SPAN_BTN_H);
        }
        if (m_SpanCatFoodIcon) {
            m_SpanCatFoodIcon->m_Transform.scale = { ((float)Cut::CATFOOD_ICON_W / sheetSize.x) * catFoodScale, ((float)Cut::CATFOOD_ICON_H / sheetSize.y) * catFoodScale };
            m_SpanCatFoodIcon->SetZIndex(25);
            m_SpanCatFoodIcon->DrawRect(Cut::CATFOOD_ICON_X, Cut::CATFOOD_ICON_Y, Cut::CATFOOD_ICON_W, Cut::CATFOOD_ICON_H);
        }
        if (m_MutiSpanCatFoodIcon) {
            m_MutiSpanCatFoodIcon->m_Transform.scale = { ((float)Cut::CATFOOD_ICON_W / sheetSize.x) * catFoodScale, ((float)Cut::CATFOOD_ICON_H / sheetSize.y) * catFoodScale };
            m_MutiSpanCatFoodIcon->SetZIndex(25);
            m_MutiSpanCatFoodIcon->DrawRect(Cut::CATFOOD_ICON_X, Cut::CATFOOD_ICON_Y, Cut::CATFOOD_ICON_W, Cut::CATFOOD_ICON_H);
        }
    }

    if (m_MutiSpanCostNumber) m_MutiSpanCostNumber->Draw();

    if (m_State == GachaState::ANIMATING) {
        int currentFrame = m_AnimTimer / animSpeed;
        if (currentFrame >= 0 && currentFrame < m_AnimFrames.size()) {
            m_AnimObject.SetDrawable(m_AnimFrames[currentFrame]);
            m_AnimObject.Draw();
        }
    }
    else if (m_State == GachaState::RESULT) {
        if (m_IsPopupOpen && m_SelectedPopupCat) {
            m_Background_black.Draw(); // 刷暗背景

            // 🚀 核心繪製：如果是巨神貓，就畫出完美的身體與下半身長腿組合！一般貓就用預設的 DrawForPopup
            if (m_PopupDisplayBody) {
                if (m_PopupDisplayLegs) m_PopupDisplayLegs->Draw();
                m_PopupDisplayBody->Draw();
            } else {
                m_SelectedPopupCat->DrawForPopup();
            }

            // 畫出按鈕
            if (m_PopupUseBtn) m_PopupUseBtn->Draw();
            if (m_PopupXpBtn) m_PopupXpBtn->Draw();
            if (m_PopupCancelBtn) {
                float popupBaseScale = 1.2f;
                static auto yesOrNoAtlas = std::make_shared<Util::Image>(RESOURCE_DIR"/img/yesOrNo.png");
                glm::vec2 sheetSizeyesOrNo = yesOrNoAtlas->GetSize();
                m_PopupCancelBtn->m_Transform.scale = { ((float)Cut::POPUP_CANCEL_TXT_W / sheetSizeyesOrNo.x) * popupBaseScale, ((float)Cut::POPUP_CANCEL_TXT_H / sheetSizeyesOrNo.y) * popupBaseScale };
                m_PopupCancelBtn->SetZIndex(80);
                m_PopupCancelBtn->DrawRect(Cut::POPUP_CANCEL_TXT_X, Cut::POPUP_CANCEL_TXT_Y, Cut::POPUP_CANCEL_TXT_W, Cut::POPUP_CANCEL_TXT_H);
            }
        }
    }
}