#include "System/UISystem.hpp"
#include "Entity/UnitFactory.hpp"
#include "Entity/UnitData.hpp"
#include "Util/Text.hpp"
#include "Util/Color.hpp"
#include "Util/Image.hpp"
#include <string>

UISystem::UISystem() {
    m_SlotBg = std::make_shared<Util::Image>(RESOURCE_DIR"/img/slot_bg.png");
    m_SlotFrame = std::make_shared<Util::Image>(RESOURCE_DIR"/img/slot_frame.png");
    m_Rank1Bg = std::make_shared<Util::Image>(RESOURCE_DIR"/img/rank1.png");
    m_Rank2Bg = std::make_shared<Util::Image>(RESOURCE_DIR"/img/rank2.png");
    m_Rank3Bg = std::make_shared<Util::Image>(RESOURCE_DIR"/img/rank3.png");

    // 💡 ZIndex 層級設定
    m_BgRenderer.SetZIndex(10);    // 最底層
    m_MaskRenderer.SetZIndex(15);  // 灰色遮罩層
    m_RankRenderer.SetZIndex(20);  // 階級框
    m_IconRenderer.SetZIndex(30);  // 貓咪頭像
    m_FrameRenderer.SetZIndex(40); // 黑邊框
    m_TextRenderer.SetZIndex(50);  // 文字
}

int UISystem::GetClickedSlot(const glm::vec2& mousePos) const {
    float halfSize = SLOT_SIZE / 2.0f;
    if (mousePos.y < UI_Y - halfSize || mousePos.y > UI_Y + halfSize) return -1;

    for (int i = 0; i < 5; ++i) {
        float centerX = SLOT_X_START + (static_cast<float>(i) * SLOT_SPACING);
        if (mousePos.x >= centerX - halfSize && mousePos.x <= centerX + halfSize) {
            return i;
        }
    }
    return -1;
}

void UISystem::Draw(const std::vector<UnitID>& deck, const float* cooldowns, float money) {
    const float ICON_SIZE = 70.0f;
    const float TEXT_Y_OFFSET = -25.0f;
    const float TEXT_X_OFFSET = 5.0f;

    for (int i = 0; i < 5; ++i) {
        float x = SLOT_X_START + (static_cast<float>(i) * SLOT_SPACING);

        // 1. 永遠繪製底座 (Z: 10)
        m_BgRenderer.SetDrawable(m_SlotBg);
        m_BgRenderer.m_Transform.translation = glm::vec2(x, UI_Y);
        m_BgRenderer.m_Transform.scale = glm::vec2(SLOT_SIZE / m_SlotBg->GetSize().x, SLOT_SIZE / m_SlotBg->GetSize().y);
        m_BgRenderer.Draw();

        if (i < (int)deck.size() && deck[i] != UnitID::NONE) {
            UnitID id = deck[i];
            const auto& stats = UnitData::Get(id);
            bool canAfford = (money >= static_cast<float>(stats.cost));
            bool isReady = (cooldowns[i] <= 0.0f);

            // 2. 只有在「可用」時才畫彩色階級框 (Z: 20)
            if (canAfford && isReady) {
                std::shared_ptr<Util::Image> rankImg = (stats.rank == 3) ? m_Rank3Bg : (stats.rank == 2 ? m_Rank2Bg : m_Rank1Bg);
                m_RankRenderer.SetDrawable(rankImg);
                m_RankRenderer.m_Transform.translation = glm::vec2(x, UI_Y);
                m_RankRenderer.m_Transform.scale = glm::vec2(SLOT_SIZE / rankImg->GetSize().x, SLOT_SIZE / rankImg->GetSize().y);
                m_RankRenderer.Draw();
            }

            // 3. 繪製貓咪頭像 (Z: 30)
            if (m_IconCache.find(id) == m_IconCache.end()) {
                m_IconCache[id] = std::make_shared<Util::Image>(UnitFactory::GetUnitIconPath(id));
            }
            auto icon = m_IconCache[id];
            if (icon && icon->GetSize().x > 0) {
                m_IconRenderer.SetDrawable(icon);
                m_IconRenderer.m_Transform.translation = glm::vec2(x, UI_Y);
                float finalScale = (canAfford && isReady) ? ICON_SIZE : ICON_SIZE * 0.7f;
                m_IconRenderer.m_Transform.scale = glm::vec2(finalScale / icon->GetSize().x, finalScale / icon->GetSize().y);
                m_IconRenderer.Draw();
            }

            // 4. 文字處理 (Z: 50)
            if (!isReady) {
                // 💡 冷卻中：只顯示秒數，不顯示價格
                auto cdText = std::make_shared<::Util::Text>(
                    RESOURCE_DIR"/Font/arial.ttf", 30, std::to_string((int)cooldowns[i] + 1), Util::Color(255, 255, 255, 255)
                );
                m_TextRenderer.SetDrawable(cdText);
                m_TextRenderer.m_Transform.translation = glm::vec2(x, UI_Y);
                m_TextRenderer.Draw();
            } else {
                // 💡 冷卻結束（Ready）：才顯示價格文字
                auto costColor = canAfford ? Util::Color(0, 0, 0, 255) : Util::Color(255, 0, 0, 255);
                auto costText = std::make_shared<::Util::Text>(
                    RESOURCE_DIR"/Font/arial.ttf", 18, std::to_string(stats.cost), costColor
                );
                m_TextRenderer.SetDrawable(costText);
                m_TextRenderer.m_Transform.translation = glm::vec2(x + TEXT_X_OFFSET, UI_Y + TEXT_Y_OFFSET);
                m_TextRenderer.Draw();
            }
        }

        // 5. 最外層黑邊框 (Z: 40)
        m_FrameRenderer.SetDrawable(m_SlotFrame);
        m_FrameRenderer.m_Transform.translation = glm::vec2(x, UI_Y);
        m_FrameRenderer.m_Transform.scale = glm::vec2(SLOT_SIZE / m_SlotFrame->GetSize().x, SLOT_SIZE / m_SlotFrame->GetSize().y);
        m_FrameRenderer.Draw();
    }
}