// #include "System/UISystem.hpp"
// #include "Entity/UnitFactory.hpp"
// #include "Entity/UnitData.hpp"
// #include "Util/Text.hpp"
// #include "Util/Color.hpp"
// #include "Util/Image.hpp"
// #include <string>
//
// UISystem::UISystem() {
//     m_SlotBg = std::make_shared<Util::Image>(RESOURCE_DIR"/img/slot_bg.png");
//     m_SlotFrame = std::make_shared<Util::Image>(RESOURCE_DIR"/img/slot_frame.png");
//     m_Rank1Bg = std::make_shared<Util::Image>(RESOURCE_DIR"/img/rank1.png");
//     m_Rank2Bg = std::make_shared<Util::Image>(RESOURCE_DIR"/img/rank2.png");
//     m_Rank3Bg = std::make_shared<Util::Image>(RESOURCE_DIR"/img/rank3.png");
//
//     // 💡 ZIndex 層級設定
//     m_BgRenderer.SetZIndex(10);    // 最底層
//     m_MaskRenderer.SetZIndex(15);  // 灰色遮罩層
//     m_RankRenderer.SetZIndex(20);  // 階級框
//     m_IconRenderer.SetZIndex(30);  // 貓咪頭像
//     m_FrameRenderer.SetZIndex(40); // 黑邊框
//     m_TextRenderer.SetZIndex(50);  // 文字
// }
//
// int UISystem::GetClickedSlot(const glm::vec2& mousePos) const {
//     float halfSize = SLOT_SIZE / 2.0f;
//
//     // 💡 修正：Y 軸往上為正，所以第一排 (topY) 是加上間距
//     const float SLOT_Y_SPACING = SLOT_SIZE-15.0f;
//     float topY = UI_Y + SLOT_Y_SPACING;
//     float bottomY = UI_Y;
//
//     // 擴充為 10 格
//     for (int i = 0; i < 10; ++i) {
//         int col = i % 5; // 求餘數 (0,1,2,3,4)：決定 X 座標
//         int row = i / 5; // 求商數 (0 或 1)：決定 Y 座標 (0是上排，1是下排)
//
//         float currentX = SLOT_X_START + (static_cast<float>(col) * SLOT_SPACING);
//         float currentY = (row == 0) ? topY : bottomY;
//
//         // 檢查游標是否落在這個格子的範圍內
//         if (mousePos.x >= currentX - halfSize && mousePos.x <= currentX + halfSize &&
//             mousePos.y >= currentY - halfSize && mousePos.y <= currentY + halfSize) {
//             return i;
//         }
//     }
//     return -1;
// }
//
// void UISystem::Draw(const std::vector<UnitID>& deck, const float* cooldowns, float money) {
//     const float ICON_SIZE = 70.0f;
//     const float TEXT_Y_OFFSET = -25.0f;
//     const float TEXT_X_OFFSET = 5.0f;
//
//     // 💡 修正：Y 軸往上為正，所以第一排 (topY) 是加上間距
//     const float SLOT_Y_SPACING = SLOT_SIZE-15.0f;
//     float topY = UI_Y + SLOT_Y_SPACING;
//     float bottomY = UI_Y;
//
//     // 擴充為 10 格
//     for (int i = 0; i < 10; ++i) {
//         int col = i % 5;
//         int row = i / 5;
//
//         float x = SLOT_X_START + (static_cast<float>(col) * SLOT_SPACING);
//         float y = (row == 0) ? topY : bottomY;
//
//         // 1. 永遠繪製底座 (Z: 10)
//         m_BgRenderer.SetDrawable(m_SlotBg);
//         m_BgRenderer.m_Transform.translation = glm::vec2(x, y);
//         m_BgRenderer.m_Transform.scale = glm::vec2(SLOT_SIZE / m_SlotBg->GetSize().x, SLOT_SIZE / m_SlotBg->GetSize().y);
//         m_BgRenderer.Draw();
//
//         // 💡 防呆：如果 deck 裡面沒放滿 10 隻貓，就跳過後面的繪製
//         if (i < (int)deck.size() && deck[i] != UnitID::NONE) {
//             UnitID id = deck[i];
//             const auto& stats = UnitData::Get(id);
//             bool canAfford = (money >= static_cast<float>(stats.cost));
//             bool isReady = (cooldowns[i] <= 0.0f);
//
//             // 2. 只有在「可用」時才畫彩色階級框 (Z: 20)
//             if (canAfford && isReady) {
//                 std::shared_ptr<Util::Image> rankImg = (stats.rank == 3) ? m_Rank3Bg : (stats.rank == 2 ? m_Rank2Bg : m_Rank1Bg);
//                 m_RankRenderer.SetDrawable(rankImg);
//                 m_RankRenderer.m_Transform.translation = glm::vec2(x, y);
//                 m_RankRenderer.m_Transform.scale = glm::vec2(SLOT_SIZE / rankImg->GetSize().x, SLOT_SIZE / rankImg->GetSize().y);
//                 m_RankRenderer.Draw();
//             }
//
//             // 3. 繪製貓咪頭像 (Z: 30)
//             if (m_IconCache.find(id) == m_IconCache.end()) {
//                 m_IconCache[id] = std::make_shared<Util::Image>(UnitFactory::GetUnitIconPath(id));
//             }
//             auto icon = m_IconCache[id];
//             if (icon && icon->GetSize().x > 0) {
//                 m_IconRenderer.SetDrawable(icon);
//                 m_IconRenderer.m_Transform.translation = glm::vec2(x, y);
//                 float finalScale = (canAfford && isReady) ? ICON_SIZE : ICON_SIZE * 0.7f;
//                 m_IconRenderer.m_Transform.scale = glm::vec2(finalScale / icon->GetSize().x, finalScale / icon->GetSize().y);
//                 m_IconRenderer.Draw();
//             }
//
//             // 4. 文字處理 (Z: 50)
//             if (!isReady) {
//                 auto cdText = std::make_shared<::Util::Text>(
//                     RESOURCE_DIR"/Font/arial.ttf", 30, std::to_string((int)cooldowns[i] + 1), Util::Color(255, 255, 255, 255)
//                 );
//                 m_TextRenderer.SetDrawable(cdText);
//                 m_TextRenderer.m_Transform.translation = glm::vec2(x, y);
//                 m_TextRenderer.Draw();
//             } else {
//                 auto costColor = canAfford ? Util::Color(50, 50, 50, 255) : Util::Color(255, 0, 0, 255);
//                 auto costText = std::make_shared<::Util::Text>(
//                     RESOURCE_DIR"/Font/arial.ttf", 18, std::to_string(stats.cost), costColor
//                 );
//                 m_TextRenderer.SetDrawable(costText);
//                 m_TextRenderer.m_Transform.translation = glm::vec2(x + TEXT_X_OFFSET, y + TEXT_Y_OFFSET);
//                 m_TextRenderer.Draw();
//             }
//         }
//
//         // 5. 最外層黑邊框 (Z: 40)
//         m_FrameRenderer.SetDrawable(m_SlotFrame);
//         m_FrameRenderer.m_Transform.translation = glm::vec2(x, y);
//         m_FrameRenderer.m_Transform.scale = glm::vec2(SLOT_SIZE / m_SlotFrame->GetSize().x, SLOT_SIZE / m_SlotFrame->GetSize().y);
//         m_FrameRenderer.Draw();
//     }
// }


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
}

void UISystem::Init(const std::vector<UnitID>& deck) {
    auto context = Core::Context::GetInstance();
    float halfW = context->GetWindowWidth() / 2.0f;
    float halfH = context->GetWindowHeight() / 2.0f;

    const float SLOT_Y_SPACING = SLOT_SIZE - 15.0f;
    float topY = UI_Y + SLOT_Y_SPACING;
    float bottomY = UI_Y;

    for (int i = 0; i < 10; ++i) {
        if (i >= deck.size() || deck[i] == UnitID::NONE) {
            m_SlotButtons.push_back(nullptr);
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
            std::to_string(stats.cost),
            18,
            Util::Color(255, 255, 0, 255),// 先用白色字，未來有需要可在 Button 擴充改色功能
            textRatioX, textRatioY
        );

        btn->SetOnClick([this, i]() {
             m_ClickedSlot = i;  // 記錄被點擊的是第幾個格子
         });

        m_SlotButtons.push_back(btn);
    }
}

void UISystem::Update(const std::vector<UnitID>& deck, const float* cooldowns, float money) {
    for (size_t i = 0; i < m_SlotButtons.size(); ++i) {
        auto& btn = m_SlotButtons[i];
        if (!btn) continue;

        btn->Update(); // 讓按鈕自己偵測滑鼠

        UnitID id = deck[i];
        float cd = cooldowns[i];
        auto& stats = UnitData::Get(id);

        if (cd > 0.0f) {
            btn->UpdateText(std::to_string((int)cd + 1));
        } else {
            btn->UpdateText(std::to_string(stats.cost));
        }
    }
}

void UISystem::Draw(const std::vector<UnitID>& deck, const float* cooldowns, float money) {
    const float ICON_SIZE = 70.0f;
    const float SLOT_Y_SPACING = SLOT_SIZE - 15.0f;
    float topY = UI_Y + SLOT_Y_SPACING;
    float bottomY = UI_Y;

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
            }
        }
    }
}