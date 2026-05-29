
#include "System/UISystem.hpp"
#include "Entity/UnitFactory.hpp"
#include "Entity/UnitData.hpp"
#include "Core/Context.hpp"

UISystem::UISystem() {
    m_SlotBg = std::make_shared<Util::Image>(RESOURCE_DIR"/img/slot_bg.png");
    m_Rank1Bg = std::make_shared<Util::Image>(RESOURCE_DIR"/img/rank1.png");
    m_Rank2Bg = std::make_shared<Util::Image>(RESOURCE_DIR"/img/rank2.png");
    m_Rank3Bg = std::make_shared<Util::Image>(RESOURCE_DIR"/img/rank3.png");

    // ZIndex 照舊設定
    m_BgRenderer.SetZIndex(10);
    m_MaskRenderer.SetZIndex(15);
    m_RankRenderer.SetZIndex(20);
    m_IconRenderer.SetZIndex(30);
    auto context = Core::Context::GetInstance();
    float halfW = context->GetWindowWidth() / 2.0f;
    float halfH = context->GetWindowHeight() / 2.0f;

    const float SLOT_Y_SPACING = SLOT_SIZE - 15.0f;
    float topY = UI_Y + SLOT_Y_SPACING;
    float bottomY = UI_Y;
    // ==========================================
    // 🚀 新增：建立「升級錢包」按鈕 (放在選兵格子的左邊)
    // ==========================================
    float walletAbsX = SLOT_X_START - 250.0f; // 往左推 150
    float walletAbsY = bottomY;               // 跟下排貓咪對齊

    float wRatioX = walletAbsX / halfW;
    float wRatioY = walletAbsY / halfH;
    float wTextRatioX = (walletAbsX + 0) / halfW;
    float wTextRatioY = (walletAbsY - 10) / halfH; // 文字稍微往下偏

    m_WalletBtn = std::make_shared<Button>(
        wRatioX, wRatioY,
        160.0f, 130.0f,
        RESOURCE_DIR"/img/moneybag_noupg.png", // ⚠️ 記得準備一張錢包的圖片！
        " ",
        20,
        Util::Color(0, 0, 0, 255), // 黃色字體
        wTextRatioX, wTextRatioY
    );
    m_WalletNumber = std::make_shared<NumberSystem>(wTextRatioX+0.045, wTextRatioY, 15.0f, 25.0f, RESOURCE_DIR"/img/moneyInfo.png");
    // 綁定錢包升級事件
    m_WalletBtn->SetOnClick([this]() {
        if (m_OnWalletUpgrade) m_OnWalletUpgrade();
    });
    // ==========================================
    float cannonAbsX = SLOT_X_START + (4 * SLOT_SPACING) + 200.0f;
    float cannonAbsY = bottomY;

    float cRatioX = cannonAbsX / halfW;
    float cRatioY = cannonAbsY / halfH;
    float cTextRatioX = (cannonAbsX + 0) / halfW;
    float cTextRatioY = (cannonAbsY - 10) / halfH;

    m_CannonBtn = std::make_shared<Button>(
        cRatioX, cRatioY,
        60.0f, 30.0f, // 貓咪砲按鈕可以稍微寬一點
        RESOURCE_DIR"/img/img002_tw.png", // ⚠️ 記得準備充能中的圖 片
        " ",
        25, // 字體大一點
        Util::Color(255, 0, 0, 255), // 紅色字體
        cTextRatioX, cTextRatioY
    );
    m_CannonNumber = std::make_shared<NumberSystem>(cTextRatioX, cTextRatioY, 20.0f, 35.0f, RESOURCE_DIR"/img/moneyInfo.png");
    m_CannonBtn->SetClipRect(680, 230, 340, 230);
    // 綁定開砲事件
    m_CannonBtn->SetOnClick([this]() {
        if (m_OnFireCannon) m_OnFireCannon();
    });
}

void UISystem::Init(const std::vector<UnitID>& deck) {
    auto context = Core::Context::GetInstance();
    float halfW = context->GetWindowWidth() / 2.0f;
    float halfH = context->GetWindowHeight() / 2.0f;

    const float SLOT_Y_SPACING = SLOT_SIZE - 15.0f;
    float topY = UI_Y + SLOT_Y_SPACING;
    float bottomY = UI_Y;
    m_SlotButtons.clear();
    m_SlotNumbers.clear(); // 🚀 清空數字陣列
    for (int i = 0; i < 10; ++i) {
        if (i >= deck.size() || deck[i] == UnitID::NONE) {
            m_SlotButtons.push_back(nullptr);
            m_SlotNumbers.push_back(nullptr);
            continue;
        }

        int col = i % 5;
        int row = i / 5;
        float absoluteX = SLOT_X_START + (static_cast<float>(col) * SLOT_SPACING);
        float absoluteY = (row == 0) ? topY : bottomY;
        float textRatioX = (absoluteX + 3) / halfW;
        float textRatioY = (absoluteY - 20) / halfH;
        float ratioX = absoluteX / halfW;
        float ratioY = absoluteY / halfH;

        UnitID id = deck[i];
        auto& stats = UnitData::Get(id);

        // 🚀 關鍵：拿 slot_frame 當作按鈕的底圖！
        auto btn = std::make_shared<Button>(
            ratioX, ratioY,
            SLOT_SIZE, SLOT_SIZE,
            RESOURCE_DIR"/img/slot_frame.png",
            " ",
            18,
            Util::Color(0, 0, 0, 255), // 黃色字體

            textRatioX, textRatioY
        );

        btn->SetOnClick([this, i]() {
             m_ClickedSlot = i;  // 記錄被點擊的是第幾個格子
         });

        m_SlotButtons.push_back(btn);
        auto num = std::make_shared<NumberSystem>(textRatioX, textRatioY, 12.0f, 18.0f, RESOURCE_DIR"/img/moneyInfo.png");
        m_SlotNumbers.push_back(num);
    }
}

void UISystem::Update(const std::vector<UnitID>& deck, const float* cooldowns, float money, int walletUpgradeCost, float cannonProgress, bool isCannonReady) {
    // 🚀 更新錢包按鈕狀態
    if (m_WalletBtn) {
        m_WalletBtn->Update();

        if (walletUpgradeCost == -1) {
            m_WalletNumber->SetValue("M"); // 🚀 更新圖片數字
            m_WalletBtn->SetImage(RESOURCE_DIR"/img/moneybag_max.png");
            m_WalletNumber->SetScale({2.0f,1.0});

        } else {
            m_WalletNumber->SetValue(std::to_string(walletUpgradeCost)+'$'); // 🚀 更新圖片數字
            m_WalletNumber->SetScale(1.0);

            if (money >= static_cast<float>(walletUpgradeCost)) {
                m_WalletBtn->SetImage(RESOURCE_DIR"/img/moneybag_upg.png");
            } else {
                m_WalletBtn->SetImage(RESOURCE_DIR"/img/moneybag_noupg.png");
            }
        }
    }

    // 🚀 更新貓咪砲按鈕狀態
    if (m_CannonBtn) {
        m_CannonBtn->Update();

        if (isCannonReady) {
            m_CannonNumber->SetValue("M"); // 🚀 原本寫 FIRE，現在換成 MAX 圖片字
            m_CannonBtn->SetClipRect(196, 123, 195, 255-123);
            m_CannonNumber->SetScale({2.0f,1.0});
        } else {
            int percent = static_cast<int>(cannonProgress * 100.0f);
            m_CannonNumber->SetValue(percent); // 🚀 更新百分比圖片數字
            m_CannonBtn->SetClipRect(392, 123, 195, 255-123);
            m_CannonNumber->SetScale(1.0f);
        }
    }

    for (size_t i = 0; i < m_SlotButtons.size(); ++i) {
        auto& btn = m_SlotButtons[i];
        if (!btn) continue;

        btn->Update(); // 讓按鈕自己偵測滑鼠

        UnitID id = deck[i];
        float cd = cooldowns[i];
        auto& stats = UnitData::Get(id);

        if (cd > 0.0f) {
            m_SlotNumbers[i]->SetValue(static_cast<int>(cd) + 1);
        } else {
            m_SlotNumbers[i]->SetValue(stats.cost);
        }
    }
}

void UISystem::Draw(const std::vector<UnitID>& deck, const float* cooldowns, float money) {
    const float ICON_SIZE = 70.0f;
    const float SLOT_Y_SPACING = SLOT_SIZE - 15.0f;
    float topY = UI_Y + SLOT_Y_SPACING;
    float bottomY = UI_Y;
    if (m_WalletBtn) {
        m_WalletBtn->Draw();
        m_WalletNumber->Draw(); // 🚀 畫出錢包數字
    }


    // 🚀 畫出貓咪砲按鈕
    if (m_CannonBtn) {
        m_CannonBtn->Draw();
        m_CannonNumber->Draw(); // 🚀 畫出大砲進度數字
    }
    for (int i = 0; i < 10; ++i) {
        int col = i % 5;
        int row = i / 5;
        float x = SLOT_X_START + (static_cast<float>(col) * SLOT_SPACING);
        float y = (row == 0) ? topY : bottomY;

        // 1. 畫底座
        m_BgRenderer.SetDrawable(m_SlotBg);
        m_BgRenderer.m_Transform.translation = glm::vec2(x, y);
        m_BgRenderer.m_Transform.scale = glm::vec2(SLOT_SIZE / m_SlotBg->GetSize().x, SLOT_SIZE / m_SlotBg->GetSize().y);
        m_BgRenderer.Draw();

        if (i < (int)deck.size() && deck[i] != UnitID::NONE) {
            UnitID id = deck[i];
            const auto& stats = UnitData::Get(id);
            bool canAfford = (money >= static_cast<float>(stats.cost));
            bool isReady = (cooldowns[i] <= 0.0f);

            // 2. 畫階級框
            if (canAfford && isReady) {
                std::shared_ptr<Util::Image> rankImg = (stats.rank == 3) ? m_Rank3Bg : (stats.rank == 2 ? m_Rank2Bg : m_Rank1Bg);
                m_RankRenderer.SetDrawable(rankImg);
                m_RankRenderer.m_Transform.translation = glm::vec2(x, y);
                m_RankRenderer.m_Transform.scale = glm::vec2(SLOT_SIZE / rankImg->GetSize().x, SLOT_SIZE / rankImg->GetSize().y);
                m_RankRenderer.Draw();
            }

            // 3. 畫貓咪頭像
            if (m_IconCache.find(id) == m_IconCache.end()) {
                m_IconCache[id] = std::make_shared<Util::Image>(UnitFactory::GetUnitIconPath(id));
            }
            auto icon = m_IconCache[id];
            if (icon && icon->GetSize().x > 0) {
                m_IconRenderer.SetDrawable(icon);
                m_IconRenderer.m_Transform.translation = glm::vec2(x, y);
                float finalScale = (canAfford && isReady) ? ICON_SIZE : ICON_SIZE * 0.7f;
                m_IconRenderer.m_Transform.scale = glm::vec2(finalScale / icon->GetSize().x, finalScale / icon->GetSize().y);
                m_IconRenderer.Draw();
            }

            // 🚀 4. 最後將「邊框按鈕」蓋上去！它會畫出黑框、畫出文字、並吃下這格的點擊判定
            if (m_SlotButtons[i]) {
                m_SlotButtons[i]->Draw();
                m_SlotNumbers[i]->Draw(); // 🚀 畫出該格子的金錢/冷卻圖片數字
            }
        }
    }
}