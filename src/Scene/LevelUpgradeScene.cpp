#include "LevelUpgradeScene.hpp"
#include "Core/Context.hpp"
#include "PlayerData.hpp"
#include <glm/trigonometric.hpp>
#include "Util/SFX.hpp"

// 🚀 完美借用 DeckScene 的切圖資料庫，加個 Upg 前綴避免衝突
namespace UpgCut {
    const float RETURN_ICON_X = 423.0f;
    const float RETURN_ICON_Y = 170.0f;
    const float RETURN_ICON_W = 511.0f - RETURN_ICON_X;
    const float RETURN_ICON_H = 232.0f - RETURN_ICON_Y;

    const float RETURN_BASE_X = 256.0f;
    const float RETURN_BASE_Y = 0.0f;
    const float RETURN_BASE_W = 99.0f;
    const float RETURN_BASE_H = 97.0f;
}

LevelUpgradeScene::LevelUpgradeScene() {
    auto playerData = PlayerData::GetInstance();
    m_XP = playerData->GetXP();
    m_CatFood = playerData->GetCatFood();

    // 1. 設定背景
    auto bgImage = std::make_shared<Util::Image>(RESOURCE_DIR"/img/UpgScene.png");
    m_Background.SetDrawable(bgImage);
    m_Background.SetZIndex(10);

    auto context = Core::Context::GetInstance();
    float windowWidth = (float)context->GetWindowWidth();
    float windowHeight = (float)context->GetWindowHeight();
    glm::vec2 imgSize = bgImage->GetSize();
    m_Background.m_Transform.scale = glm::vec2(windowWidth / imgSize.x, windowHeight / imgSize.y);

    // 2. 設定左右邊框 (完美照抄 DeckScene)
    float borderWidth = 30.0f;
    m_LeftBorder = std::make_shared<Button>(
        -1.035f, 0.0f, windowHeight, borderWidth,
        RESOURCE_DIR"/img/img008_tw.png", " ", 20, Util::Color(0,0,0,0)
    );
    m_LeftBorder->SetZIndex(80);
    m_LeftBorder->m_Transform.rotation = glm::radians(-90.0f);

    m_RightBorder = std::make_shared<Button>(
        1.035f, 0.0f, windowHeight, borderWidth,
        RESOURCE_DIR"/img/img008_tw.png", " ", 20, Util::Color(0,0,0,0)
    );
    m_RightBorder->SetZIndex(80);
    m_RightBorder->m_Transform.rotation = glm::radians(90.0f);

    // 3. 設定返回按鈕
    float btnSize = 100.0f;
    m_ReturnBtn = std::make_shared<Button>(
        -0.805f, -0.87f, btnSize, btnSize,
        RESOURCE_DIR"/img/img006_tw.png", " ", 30, Util::Color(255, 255, 255, 255)
    );
    m_ReturnBtn->SetZIndex(20);
    m_ReturnBtn->SetOnClick([this]() {
        Util::SFX(RESOURCE_DIR "/music/clickbtn.mp3").Play();
        if (m_OnReturn) m_OnReturn();
    });
    m_UpgradeBtn = std::make_shared<Button>(
            -0.65f, -0.43f, 100.0f, 63.0f,
            RESOURCE_DIR"/img/upgrade.png", // 💡 你可以先拿這個圖墊著，之後換成真的升級按鈕圖
            " ", 35, Util::Color(0,0,0,0)
        );
    m_UpgradeBtn->SetZIndex(0);

    // 🚀 核心升級邏輯
    m_UpgradeBtn->SetOnClick([this]() {
        if (static_cast<int>(m_SelectedCatID) == 0) return; // 沒選貓就不做事

        auto pData = PlayerData::GetInstance();
        PlayerData::CatLevel catLvl = pData->GetCatLevel(m_SelectedCatID);

        // 1. 檢查是否滿級 (假設上限是 20 級)
        if (catLvl.base >= 20) {
            LOG_DEBUG("這隻貓咪已經達到最大等級！");
            Util::SFX(RESOURCE_DIR "/music/fail_summon_cat.mp3").Play();

            return;
        }

        // 2. 計算升級所需 XP (⚠️ 要跟 CatCardUI 裡的公式一模一樣！)
        int costXP = catLvl.base * 200;

        // 3. 判斷 XP 夠不夠並扣款
        if (pData->SpendXP(costXP)) {
            Util::SFX(RESOURCE_DIR "/music/clickbtn.mp3").Play();
            LOG_DEBUG("升級成功！花費 XP: " + std::to_string(costXP));

            // 4. 提升基礎等級並存檔！
            pData->UpgradeCatBaseLevel(m_SelectedCatID);
            pData->SaveToFile();

            // 5. 更新 XP / 貓罐頭顯示
            m_XP =
                pData->GetXP();

            if (m_XPNumber)
            {
                m_XPNumber->SetValue(
                    m_XP
                );
            }

            m_CatFood =
                pData->GetCatFood();

            if (m_CatFoodNumber)
            {
                m_CatFoodNumber->SetValue(
                    m_CatFood
                );
            }

            // 6. 輕量更新下方卡片資料
            // 不要 LoadCats，不要 Refresh，不要重建卡片
            if (m_DeckUI)
            {
                m_DeckUI->RefreshCards();
            }

        } else {
            LOG_DEBUG("XP 不足，無法升級！");
            Util::SFX(RESOURCE_DIR "/music/fail_summon_cat.mp3").Play();
        }
    });
    // 4. 資源數字
    m_XPNumber = std::make_shared<NumberSystem>(0.84f, 0.935f, 29.0f, 38.0f, RESOURCE_DIR"/img/moneyInfo.png");
    m_XPNumber->SetValue(m_XP);

    m_CatFoodNumber = std::make_shared<NumberSystem>(0.84f, -0.95f, 29.0f, 38.0f, RESOURCE_DIR"/img/moneyInfo.png");
    m_CatFoodNumber->SetValue(m_CatFood);

    // 5. 初始化下方滑動冰箱 DeckUI
    m_DeckUI = std::make_shared<DeckUI>();
    m_DeckUI->SetBaseYRatio(0.2f);
    m_DeckUI->SetUpgradeMode(true);
    // 載入玩家真實擁有的貓咪
    std::vector<UnitID> realCatList = PlayerData::GetInstance()->GetUnlockedCatsList();
    m_DeckUI->LoadCats(realCatList);
    if (!realCatList.empty()) {
        m_SelectedCatID = realCatList[0];
        // RefreshSelectedCatInfo(); // 呼叫刷新，大圖跟按鈕就出來了！
    }
    // 🚀 升級專屬邏輯：當玩家在列表裡「點擊」某隻貓時
    m_DeckUI->SetOnCatSelected([this](UnitID selectedId) {
         LOG_DEBUG("在升級畫面，玩家選中了貓咪 ID: " + std::to_string(static_cast<int>(selectedId)));

         m_SelectedCatID = selectedId;

     });
}

void LevelUpgradeScene::Refresh()
{
    auto pData =
        PlayerData::GetInstance();

    m_XP =
        pData->GetXP();

    m_CatFood =
        pData->GetCatFood();

    if (m_XPNumber)
    {
        m_XPNumber->SetValue(m_XP);
    }

    if (m_CatFoodNumber)
    {
        m_CatFoodNumber->SetValue(m_CatFood);
    }

    std::vector<UnitID> realCatList =
        pData->GetUnlockedCatsList();

    if (m_DeckUI)
    {
        // 每次進入場景才重建一次，不是每幀重建
        // 可以同步新解鎖角色與最新加值
        m_DeckUI->LoadCats(realCatList);
    }

    if (!realCatList.empty())
    {
        bool selectedCatStillExists =
            std::find(
                realCatList.begin(),
                realCatList.end(),
                m_SelectedCatID
            ) != realCatList.end();

        if (!selectedCatStillExists)
        {
            m_SelectedCatID =
                realCatList.front();
        }
    }
    else
    {
        m_SelectedCatID =
            static_cast<UnitID>(0);
    }

    LOG_DEBUG(
        "LevelUpgradeScene Refresh: cat list and card data updated."
    );
}
void LevelUpgradeScene::SetOnReturnBtnClick(std::function<void()> callback) {
    m_OnReturn = callback;
}

void LevelUpgradeScene::Update() {
    if (m_ReturnBtn) m_ReturnBtn->Update();
    if (m_UpgradeBtn && static_cast<int>(m_SelectedCatID) != 0) {
        m_UpgradeBtn->Update();
    }
    // 🚀 即時同步資源數值
    auto pData = PlayerData::GetInstance();
    if (m_XP != pData->GetXP()) {
        m_XP = pData->GetXP();
        if (m_XPNumber) m_XPNumber->SetValue(m_XP);
    }
    if (m_CatFood != pData->GetCatFood()) {
        m_CatFood = pData->GetCatFood();
        if (m_CatFoodNumber) m_CatFoodNumber->SetValue(m_CatFood);
    }

    if (m_DeckUI) {
        m_DeckUI->Update();

        // ==========================================
        // 🚀 核心修復：讓選取的貓咪「永遠等於畫面正中央的貓」！
        // ==========================================
        UnitID centerCat = m_DeckUI->GetCenterCatID();
        if (static_cast<int>(centerCat) != 0 && m_SelectedCatID != centerCat) {
            m_SelectedCatID = centerCat;

            // 💡 你之前把 RefreshSelectedCatInfo() 註解掉了
            // 如果你把那段大圖加回來，只要把下面這行解除註解，滑動時上面大圖就會跟著切換！
            // RefreshSelectedCatInfo();
        }
    }
    if (m_LeftBorder) m_LeftBorder->Update();
    if (m_RightBorder) m_RightBorder->Update();

}

void LevelUpgradeScene::Draw() {
    m_Background.Draw();

    // 🚀 完美照抄 DeckScene 的雙層返回按鈕畫法
    static auto uiAtlas = std::make_shared<Util::Image>(RESOURCE_DIR"/img/img006_tw.png");
    glm::vec2 sheetSize = uiAtlas->GetSize();
    float iconBoost = 0.8f;
    float baseScale = 1.0f;

    if (m_ReturnBtn) {
        m_ReturnBtn->m_Transform.scale = {
            ((float)UpgCut::RETURN_BASE_W / sheetSize.x) * baseScale,
            ((float)UpgCut::RETURN_BASE_H / sheetSize.y) * baseScale
        };
        m_ReturnBtn->SetZIndex(21);
        m_ReturnBtn->DrawRect(UpgCut::RETURN_BASE_X, UpgCut::RETURN_BASE_Y, UpgCut::RETURN_BASE_W, UpgCut::RETURN_BASE_H);

        glm::vec2 originalPos = m_ReturnBtn->m_Transform.translation;
        m_ReturnBtn->m_Transform.translation += glm::vec2(-0.2f, 0.0f);

        m_ReturnBtn->m_Transform.scale = {
            ((float)UpgCut::RETURN_ICON_W / sheetSize.x) * baseScale * iconBoost,
            ((float)UpgCut::RETURN_ICON_H / sheetSize.y) * baseScale * iconBoost
        };
        m_ReturnBtn->SetZIndex(22);
        m_ReturnBtn->DrawRect(UpgCut::RETURN_ICON_X, UpgCut::RETURN_ICON_Y, UpgCut::RETURN_ICON_W, UpgCut::RETURN_ICON_H);

        m_ReturnBtn->m_Transform.translation = originalPos;
        m_ReturnBtn->m_Transform.scale = {
            ((float)UpgCut::RETURN_BASE_W / sheetSize.x) * baseScale,
            ((float)UpgCut::RETURN_BASE_H / sheetSize.y) * baseScale
        };
    }

    if (m_XPNumber) m_XPNumber->Draw();
    if (m_CatFoodNumber) m_CatFoodNumber->Draw();
    if (m_DeckUI) m_DeckUI->Draw();

    // if (m_SelectedCatIcon) m_SelectedCatIcon->Draw();
    // 🚀 畫出升級按鈕 (如果有選貓的話)
    if (m_UpgradeBtn && static_cast<int>(m_SelectedCatID) != 0) {
        m_UpgradeBtn->Draw();

        // TODO: 如果你之後想用 DrawRect 切特定的按鈕圖，可以寫在這裡
    }

    // 畫出左右邊框遮罩
    if (m_LeftBorder) m_LeftBorder->DrawRect(481, 0, 863-481, 77);
    if (m_RightBorder) m_RightBorder->DrawRect(481, 0, 863-481, 77);

}