#include "CatCardUI.hpp"
#include "Core/Context.hpp"
#include "PlayerData.hpp"

CatCardUI::CatCardUI(UnitID id, float pos_ratio_x, float pos_ratio_y)
    : m_UnitID(id) {
    const UnitStats& stats = UnitData::Get(id);
    int currentLevel = PlayerData::GetInstance()->GetCatLevel(id);
    if (currentLevel == 0) currentLevel = 1;

    m_BackgroundBtn = std::make_shared<Button>(
        pos_ratio_x, pos_ratio_y, m_Width, m_Height,
        RESOURCE_DIR"/img/blackCard.png", " ", 20, Util::Color(0,0,0,0)
    );
    m_BackgroundBtn->SetZIndex(30);
    m_BaseCenter = m_BackgroundBtn->m_Transform.translation;

    m_CatIcon = std::make_shared<Util::GameObject>(std::make_unique<Util::Image>(stats.ediPath), 31);
    m_CatIcon->m_Transform.scale = { CatIconScaleX, CatIconScaleY };
    m_CatIcon->m_Transform.translation = m_BaseCenter + OFFSET_ICON;

    // m_NameImage = std::make_shared<Util::GameObject>(std::make_unique<Util::Image>(RESOURCE_DIR"/img/black.png"), 32);
    // m_NameImage->m_Transform.scale = { 0.0f, 0.0f };
    // m_NameImage->m_Transform.translation = m_BaseCenter + OFFSET_NAME;

    m_LevelNumber = std::make_shared<NumberSystem>(0.0f, 0.0f, 15.0f, 20.0f, RESOURCE_DIR"/img/moneyInfo.png");
    m_LevelNumber->SetValue(currentLevel);

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
    if (m_SwapBtn) m_SwapBtn->Update();
}

// Draw 函式也請記得同步加上這個 m_IsVisible 判斷
void CatCardUI::Draw() {
    if (!m_IsVisible) return; // 解決圖片出界
    if (m_BackgroundBtn) m_BackgroundBtn->Draw();
    if (m_CatIcon) m_CatIcon->Draw();
    // if (m_NameImage) m_NameImage->Draw();
    if (m_LevelNumber) m_LevelNumber->Draw();
    if (m_CostNumber) m_CostNumber->Draw();
    if (m_SwapBtn) m_SwapBtn->Draw();
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