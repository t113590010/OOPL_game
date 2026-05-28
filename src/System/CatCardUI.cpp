#include "CatCardUI.hpp"
#include "Core/Context.hpp"
#include "PlayerData.hpp"

CatCardUI::CatCardUI(UnitID id, float pos_ratio_x, float pos_ratio_y)
    : m_UnitID(id) {
    const UnitStats& stats = UnitData::Get(id);
    // 🚀 1. 拋棄舊的推算法，直接跟 PlayerData 拿最新的雙軌等級！
    PlayerData::CatLevel catLevel = PlayerData::GetInstance()->GetCatLevel(id);
    // 只要基礎等級滿 20，就亮起 MAX 圖示
    m_IsMaxLevel = (catLevel.base >= 20);
    m_BackgroundBtn = std::make_shared<Button>(
        pos_ratio_x, pos_ratio_y, m_Width, m_Height,
        RESOURCE_DIR"/img/blackCard.png", " ", 20, Util::Color(0,0,0,0)
    );
    m_BackgroundBtn->SetZIndex(30);
    m_BaseCenter = m_BackgroundBtn->m_Transform.translation;

    m_CatIcon = std::make_shared<Util::GameObject>(std::make_unique<Util::Image>(stats.ediPath), 31);
    m_CatIcon->m_Transform.scale = { CatIconScaleX, CatIconScaleY };
    m_CatIcon->m_Transform.translation = m_BaseCenter + OFFSET_ICON;
    // 🚀 新增：判斷如果是自製角色，額外疊加 imgPath 作為本體
    if (id == UnitID::DogDoin || id == UnitID::Peashooter || id == UnitID::Queen) {
        float physW = 100.0f;
        float physH = 100.0f;
        float customScale = 1.0f;
        glm::vec2 specificOffset = { 0.0f, 0.0f };
        if (id == UnitID::DogDoin) {
            physW = 351.0f;
            physH = 288.0f;
            customScale = 0.065f;
            specificOffset = { -45.0f, -3.0f}; // 💡 如果你想讓牠往上移一點，就把 Y 調大
        }
        else if (id == UnitID::Peashooter) {
            physW = 90.0f;
            physH = 56.0f;
            customScale = 0.15f;
            specificOffset = { -45.0f, -15.0f}; // 💡 如果你想讓牠往左移一點，就把 X 變負數
        }
        else if (id == UnitID::Queen) {
            physW = 55.0f;
            // physH = 348-252;
            physH = 348-232;

            customScale = 0.2f;
            specificOffset = { -37.0f, -5.0f };
        }

        m_CustomCatImg = std::make_shared<Button>(
            pos_ratio_x, pos_ratio_y, physW, physH,
            stats.imgPath, " ", 32, Util::Color(0,0,0,0)
        );

        // 🚀 關鍵魔法：讓 X 變成負數，圖片就會完美水平翻轉！
        m_CustomBaseScale = { -customScale, customScale };

        // 記下專屬偏移量
        m_CustomOffset = specificOffset;

        m_CustomCatImg->m_Transform.scale = m_CustomBaseScale;
        // 初始化座標時，記得把專屬偏移量也加上去
        m_CustomCatImg->m_Transform.translation = m_BaseCenter + OFFSET_ICON + m_CustomOffset;
        m_CustomCatImg->SetZIndex(40);

    } else {
        m_CustomCatImg = nullptr;
    }
    // m_NameImage = std::make_shared<Util::GameObject>(std::make_unique<Util::Image>(RESOURCE_DIR"/img/black.png"), 32);
    // m_NameImage->m_Transform.scale = { 0.0f, 0.0f };
    // m_NameImage->m_Transform.translation = m_BaseCenter + OFFSET_NAME;
    // 🚀 1. 初始化額外等級 (例如 +90)
    m_ExtraLevelNumber = std::make_shared<NumberSystem>(0.0f, 0.0f, 15.0f, 20.0f, RESOURCE_DIR"/img/moneyInfo.png");
    // TODO: 之後這裡可以改成從 PlayerData 讀取這隻貓的「加值」
    m_ExtraLevelNumber->SetValue("+90");

    // 🚀 2. 初始化「等級」圖片
    lvl_Icon = std::make_shared<Button>(
         pos_ratio_x, pos_ratio_y, 10.0f, 5.0f,
         RESOURCE_DIR"/img/moneyInfo.png", " ", 20, Util::Color(0,0,0,0)
     );
    lvl_Icon->SetZIndex(32);

    // 🚀 3. 初始化「MAX」圖示 (照你的定義先用 NumberSystem，如果之後換 GameObject 記得改這裡)
    m_lvlMaxIcon = std::make_shared<NumberSystem>(0.0f, 0.0f, 30.0f, 15.0f, RESOURCE_DIR"/img/moneyInfo.png");
    m_lvlMaxIcon->SetValue("MAX");


    m_LevelNumber = std::make_shared<NumberSystem>(0.0f, 0.0f, 15.0f, 20.0f, RESOURCE_DIR"/img/moneyInfo.png");
    m_LevelNumber->SetValue(catLevel.base);
    if (catLevel.plus > 0) {
        m_ExtraLevelNumber = std::make_shared<NumberSystem>(0.0f, 0.0f, 15.0f, 20.0f, RESOURCE_DIR"/img/moneyInfo.png");
        // 加上 "+" 號字串
        m_ExtraLevelNumber->SetValue("+" + std::to_string(catLevel.plus));
    } else {
        m_ExtraLevelNumber = nullptr; // 沒有吃重複貓，就不顯示加值
    }
    m_CostNumber = std::make_shared<NumberSystem>(0.0f, 0.0f, 15.0f, 20.0f, RESOURCE_DIR"/img/moneyInfo.png");
    m_CostNumber->SetValue(stats.cost);

    m_SwapBtn = std::make_shared<Button>(
        pos_ratio_x, pos_ratio_y, 40.0f, 40.0f,
        RESOURCE_DIR"/img/swap_icon.png", " ", 20, Util::Color(0,0,0,0)
    );
    m_SwapBtn->SetZIndex(35);

    // 👑 建構子結束時，先做一次初始絕對座標計算 (NAME/NUMBER)
    m_SwapBtn->m_Transform.translation.x += OFFSET_SWAP.x;
    m_SwapBtn->m_Transform.translation.y += OFFSET_SWAP.y;

    // 紀錄原始縮放
    m_BgBaseScale = m_BackgroundBtn->m_Transform.scale;
    m_SwapBaseScale = m_SwapBtn->m_Transform.scale;
}

void CatCardUI::ApplyTransform(float finalX, float scale) {
    float cx = finalX;
    m_IsSelected = (scale > 0.99f);
    // 🚀 完美底部對齊：只下壓「實際高度的一半」
    float halfHeight = m_Height / 2.0f; // 180 / 2 = 90
    float cy = m_BaseCenter.y - halfHeight * (1.0f - scale);

    if (m_BackgroundBtn) {
        m_BackgroundBtn->m_Transform.translation = { cx, cy };
        m_BackgroundBtn->m_Transform.scale = m_BgBaseScale * scale;
    }
    if (m_CatIcon) {
        m_CatIcon->m_Transform.translation = { cx + (OFFSET_ICON.x * scale), cy + (OFFSET_ICON.y * scale) };
        m_CatIcon->m_Transform.scale = glm::vec2(CatIconScaleX, CatIconScaleY) * scale;
    }
    // if (m_NameImage) {
    //     m_NameImage->m_Transform.translation = { cx + (OFFSET_NAME.x * scale), cy + (OFFSET_NAME.y * scale) };
    // }
    if (m_LevelNumber) {
        m_LevelNumber->SetPosition(cx + (OFFSET_LEVEL.x * scale), cy + (OFFSET_LEVEL.y * scale));
        m_LevelNumber->SetScale(scale);
    }
    if (m_CostNumber) {
        m_CostNumber->SetPosition(cx + (OFFSET_COST.x * scale), cy + (OFFSET_COST.y * scale));
        m_CostNumber->SetScale(scale);
    }
    if (m_SwapBtn) {
        m_SwapBtn->m_Transform.translation = { cx + (OFFSET_SWAP.x * scale), cy + (OFFSET_SWAP.y * scale) };
        m_SwapBtn->m_Transform.scale = m_SwapBaseScale * scale;
    }

    if (m_ExtraLevelNumber) {
        m_ExtraLevelNumber->SetPosition(cx + (OFFSET_EXTRA_LEVEL.x * scale), cy + (OFFSET_EXTRA_LEVEL.y * scale));
        m_ExtraLevelNumber->SetScale(scale);
    }
    if (lvl_Icon) {
        lvl_Icon->m_Transform.translation = { cx + (OFFSET_LVL_ICON.x * scale*LvlIconScaleX), cy + (OFFSET_LVL_ICON.y * scale*LvlIconScaleX) };
        // 這裡的 1.0f 之後要換成你切圖後的實際縮放比例
        lvl_Icon->m_Transform.scale = glm::vec2(1.0f, 1.0f) * scale*LvlIconScaleX;
    }
    if (m_lvlMaxIcon) {
        m_lvlMaxIcon->SetPosition(cx + (OFFSET_MAX_ICON.x * scale), cy + (OFFSET_MAX_ICON.y * scale));
        m_lvlMaxIcon->SetScale(scale);
    }
    if (m_CustomCatImg) {
        // 🚀 將原本的 OFFSET_ICON 加上 m_CustomOffset，然後再一起乘上 scale！
        m_CustomCatImg->m_Transform.translation = {
            cx + ((OFFSET_ICON.x + m_CustomOffset.x) * scale),
            cy + ((OFFSET_ICON.y + m_CustomOffset.y) * scale)
        };

        // 這裡套用我們剛剛設好的 scale (X 已經是負數了，所以翻轉效果會一直保留)
        m_CustomCatImg->m_Transform.scale = m_CustomBaseScale * scale;
    }
}
void PlayerData::UnlockOrUpgradeCat(UnitID id) {
    if (HasCat(id)) {
        // 如果已經有了 -> 視為抽到重複貓，增加額外等級 (Plus Level)
        AddCatPlusLevel(id);
    } else {
        // 如果還沒有 -> 首次獲得，解鎖基礎貓
        UnlockCat(id);
    }
}
void CatCardUI::Update() {
    // 🚀 解決問題一（圖片出界）：強迫症級別的畫面剔除 (Culling)
    // 既然引擎沒有提供視窗裁切，我們就在 Update 裡硬幹。
    auto context = Core::Context::GetInstance();
    float halfW = context->GetWindowWidth() / 2.0f;

    // 如果卡片中心點在螢幕左右邊緣外（多抓150緩衝），直接不更新、不顯示。
    // 這能保證你看不到「圖片超出去」到旁邊的 UI 區域。
    if (m_BackgroundBtn->m_Transform.translation.x < -halfW - 150.0f ||
        m_BackgroundBtn->m_Transform.translation.x > halfW + 150.0f) {
        m_IsVisible = false;
        return; // 中斷，不執行按鈕檢測
    }
    m_IsVisible = true;

    if (m_BackgroundBtn) m_BackgroundBtn->Update();
    if (m_SwapBtn && m_IsSelected) m_SwapBtn->Update();
}

// Draw 函式也請記得同步加上這個 m_IsVisible 判斷
void CatCardUI::Draw() {
    if (!m_IsVisible) return; // 解決圖片出界
    if (m_BackgroundBtn) m_BackgroundBtn->Draw();
    if (m_CatIcon) m_CatIcon->Draw();
    // if (m_NameImage) m_NameImage->Draw();
    if (m_LevelNumber) m_LevelNumber->Draw();
    if (m_CostNumber) m_CostNumber->Draw();
    if (m_SwapBtn && m_IsSelected) m_SwapBtn->Draw();

    if (m_ExtraLevelNumber) m_ExtraLevelNumber->Draw();
    if (lvl_Icon) {
        lvl_Icon->DrawRect(0, 88, 69, 116-88);
    }
    if (m_lvlMaxIcon && m_IsMaxLevel) m_lvlMaxIcon->Draw();
    if (m_CustomCatImg) {
        if (m_UnitID == UnitID::DogDoin) {
            // TODO: 填入 DogDoin 在圖片上的真實座標
            m_CustomCatImg->DrawRect(56, 88, 407-56, 376-88);
        }
        else if (m_UnitID == UnitID::Peashooter) {
            // TODO: 填入 Peashooter 在圖片上的真實座標
            m_CustomCatImg->DrawRect(181, 308, 271-181, 384-308);
        }
        else if (m_UnitID == UnitID::Queen) {
            // TODO: 填入 Queen 在圖片上的真實座標
            m_CustomCatImg->DrawRect(557, 272, 55, 348-252);
        }
    }
}
void CatCardUI::SetOnClick(std::function<void(UnitID)> callback) {
    if (m_BackgroundBtn) {
        m_BackgroundBtn->SetOnClick([this, callback]() {
            callback(m_UnitID);
        });
    }
}

void CatCardUI::SetOnSwapClick(std::function<void(UnitID)> callback) {
    if (m_SwapBtn) {
        m_SwapBtn->SetOnClick([this, callback]() {
            callback(m_UnitID);
        });
    }
}