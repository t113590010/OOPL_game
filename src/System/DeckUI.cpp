#include "DeckUI.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Core/Context.hpp"

DeckUI::DeckUI() {
}

void DeckUI::LoadCats(const std::vector<UnitID>& catList) {
    m_Cards.clear();
    m_ScrollX = 0.0f;

    // 🚀 決定整排卡片的高度 (調大會往上)
    float baseYRatio = -0.4f;

    for (size_t i = 0; i < catList.size(); ++i) {
        UnitID id = catList[i];

        // 初始 X 直接給 0，Update 會自動把他們排好
        auto card = std::make_shared<CatCardUI>(id, 0.0f, baseYRatio);

        card->SetOnClick([this](UnitID selectedId) {
            if (!m_IsDragging && m_OnCatSelected) m_OnCatSelected(selectedId);
        });
        card->SetOnSwapClick([this](UnitID selectedId) {
            if (!m_IsDragging && m_OnSwapClick) m_OnSwapClick(selectedId);
        });

        m_Cards.push_back(card);
    }
}
void DeckUI::Update() {
    if (m_Cards.empty()) return;

    auto context = Core::Context::GetInstance();
    float halfW = context->GetWindowWidth() / 2.0f;
    float halfH = context->GetWindowHeight() / 2.0f;
    float absoluteY = halfH * -0.4f;

    // 固定的基準間距
    float SPACING = 190.0f;

    // --- 1. 滑動與吸附 (Snap) ---
    // --- 1. 滑動與吸附 (Snap) ---
    glm::vec2 mousePos = Util::Input::GetCursorPosition();
    bool isMouseDown = Util::Input::IsKeyPressed(Util::Keycode::MOUSE_LB);

    // Y 軸判定範圍
    bool isInScrollArea = (mousePos.y > absoluteY - 180.0f && mousePos.y < absoluteY + 180.0f);

    // 🚀 Debug: 印出按下的瞬間，滑鼠座標和目標座標差多少
    if (isMouseDown) {
        LOG_DEBUG("MouseDown -> MouseY: " + std::to_string(mousePos.y) +
                  " | Target AbsY: " + std::to_string(absoluteY) +
                  " | InArea: " + std::to_string(isInScrollArea));
    }

    if (isMouseDown) {
        // 狀態 A：剛按下的第一幀
        if (!m_IsDragging && isInScrollArea) {
            LOG_DEBUG("Drag Started!");
            m_IsDragging = true;
            m_LastMouseX = mousePos.x;
        }
        // 狀態 B：已經在拖曳中
        else if (m_IsDragging) {
            m_ScrollX += (mousePos.x - m_LastMouseX);
            m_LastMouseX = mousePos.x;
            LOG_DEBUG("Dragging... ScrollX: " + std::to_string(m_ScrollX));
        }
    } else {
        if (m_IsDragging) {
            LOG_DEBUG("Drag Released! Snapping to nearest card.");
        }

        m_IsDragging = false;

        // 吸附功能
        int nearestIndex = std::round(-m_ScrollX / SPACING);
        m_ScrollX = -nearestIndex * SPACING;
    }

    // --- 2. 邊界限制 ---
    float maxScroll = 0.0f;
    float minScroll = -((m_Cards.size() - 1) * SPACING);
    if (minScroll > 0) minScroll = 0.0f;

    if (m_ScrollX > maxScroll) m_ScrollX = maxScroll;
    if (m_ScrollX < minScroll) m_ScrollX = minScroll;

    // --- 3. 絕對座標更新與剔除 ---
    for (size_t i = 0; i < m_Cards.size(); ++i) {
        float rawX = (i * SPACING) + m_ScrollX;

        // 畫面剔除 (Culling)
        if (rawX < -halfW - 150.0f || rawX > halfW + 150.0f) {
            m_Cards[i]->SetVisible(false);
            continue;
        }
        m_Cards[i]->SetVisible(true);

        // 計算距離中心的權重 (0.0 ~ 1.0)
        float dist = std::abs(rawX);
        float weight = std::max(0.0f, 1.0f - (dist / SPACING));

        // 比例：中間 1.0，旁邊 0.6
        float targetScale = 0.6f + (0.4f * weight);

        // 🚀 核心數學修正：補償被吃掉的 52 像素
        // 判斷這張卡片在中間的左邊(-)還是右邊(+)
        float pushDirection = (rawX > 0) ? 1.0f : ((rawX < 0) ? -1.0f : 0.0f);

        // 嚴格推開半徑差 (130 - 78 = 52)
        float pushAmount = 52.0f;

        // 權重為 1(正中間)時不推，權重為 0(旁邊)時推滿 52 像素
        float pushOffset = pushDirection * pushAmount * (1.0f - weight);

        // 最終絕對不重疊的精準坐標
        float finalX = rawX + pushOffset;

        m_Cards[i]->ApplyTransform(finalX, targetScale);
        m_Cards[i]->Update();
    }
}
void DeckUI::Draw() {
    for (auto& card : m_Cards) {
        card->Draw();
    }
}

void DeckUI::SetOnCatSelected(std::function<void(UnitID)> callback) {
    m_OnCatSelected = callback;
}

void DeckUI::SetOnSwapClick(std::function<void(UnitID)> callback) {
    m_OnSwapClick = callback;
}