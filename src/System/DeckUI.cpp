#include "DeckUI.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/SFX.hpp"

#include "Core/Context.hpp"

DeckUI::DeckUI() {
}

void DeckUI::LoadCats(const std::vector<UnitID>& catList) {
    m_Cards.clear();
    m_ScrollX = 0.0f;
    m_TargetScrollX = 0.0f;
    // 🚀 決定整排卡片的高度 (調大會往上)
    // float baseYRatio = -0.4f;
    m_CurrentCenterIndex = 0;
    float baseYRatio = m_BaseYRatio;
    for (size_t i = 0; i < catList.size(); ++i) {
        UnitID id = catList[i];

        // 初始 X 直接給 0，Update 會自動把他們排好
        auto card = std::make_shared<CatCardUI>(id, 0.0f, baseYRatio, m_IsUpgradeMode);

        card->SetOnClick([this](UnitID selectedId) {
            if (!m_IsDragging && m_OnCatSelected) m_OnCatSelected(selectedId);
        });
        card->SetOnSwapClick([this](UnitID selectedId) {
            if (!m_IsDragging && m_OnSwapClick) m_OnSwapClick(selectedId);
        });

        m_Cards.push_back(card);
    }
}
void DeckUI::SetBaseYRatio(float yRatio) {
    m_BaseYRatio = yRatio;
}
UnitID DeckUI::GetCenterCatID() const {
    if (m_Cards.empty()) return static_cast<UnitID>(0);

    // 找距離中心點 (X=0) 最近的那張卡
    float SPACING = 190.0f;
    int nearestIndex = std::round(-m_ScrollX / SPACING);

    // 邊界防呆
    if (nearestIndex < 0) nearestIndex = 0;
    if (nearestIndex >= m_Cards.size()) nearestIndex = m_Cards.size() - 1;

    return m_Cards[nearestIndex]->GetUnitID();
}

void DeckUI::SetOnCatDraggedOut(std::function<void(UnitID)> callback) {
    m_OnCatDraggedOut = callback;
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
   glm::vec2 mousePos = Util::Input::GetCursorPosition();
    bool isMouseDown = Util::Input::IsKeyPressed(Util::Keycode::MOUSE_LB);

    // Y 軸判定範圍與拖拉閾值
    bool isInScrollArea = (mousePos.y > absoluteY - 180.0f && mousePos.y < absoluteY + 180.0f);
    float dragOutThreshold = absoluteY + 180.0f;

    if (isMouseDown) {
        if (!m_IsDragging && isInScrollArea && !m_IsDraggingCatOut) {
            m_IsDragging = true;
            m_LastMouseX = mousePos.x;
            m_TargetScrollX = m_ScrollX; // 剛點下時，目標等於當前位置
        }
        else if (m_IsDragging) {
            // 🚀 檢查滑鼠是不是「往上」拉破了閾值
            if (mousePos.y > dragOutThreshold) {
                // 進入拖出模式
                m_IsDragging = false;
                m_IsDraggingCatOut = true;

                if (m_OnCatDraggedOut) {
                    m_OnCatDraggedOut(GetCenterCatID());
                }

                // 🌟 關鍵：當貓被拖出去時，讓底下的卡片列立刻尋找最近的卡片當作目標！
                int nearestIndex = std::round(-m_ScrollX / SPACING);
                m_TargetScrollX = -nearestIndex * SPACING;
            }
            else {
                // 正常的左右滑動 (跟著滑鼠走)
                m_ScrollX += (mousePos.x - m_LastMouseX);
                m_TargetScrollX = m_ScrollX; // 目標死死咬住滑鼠
                m_LastMouseX = mousePos.x;
            }
        }
    } else {
        // 放開滑鼠
        m_IsDraggingCatOut = false;
        if (m_IsDragging) {
            m_IsDragging = false;
            // 🌟 關鍵：放開滑鼠時，不直接改 m_ScrollX，而是設定「目標位置」
            int nearestIndex = std::round(-m_ScrollX / SPACING);
            m_TargetScrollX = -nearestIndex * SPACING;
        }
    }

    // --- 2. 邊界限制 (套用在目標上) ---
    float maxScroll = 0.0f;
    float minScroll = -((m_Cards.size() - 1) * SPACING);
    if (minScroll > 0) minScroll = 0.0f;

    if (m_TargetScrollX > maxScroll) m_TargetScrollX = maxScroll;
    if (m_TargetScrollX < minScroll) m_TargetScrollX = minScroll;

    // --- 🚀 3. 平滑移動動畫 (Lerp) ---
    // 如果現在沒有在拖曳，且還沒到達目標位置，就讓它順順地滑過去！
    if (!m_IsDragging) {
        // 0.15f 是滑動的速度，數字越大滑越快，越小越滑順 (建議 0.1f ~ 0.2f)
        m_ScrollX += (m_TargetScrollX - m_ScrollX) * 0.15f;
    }
    if (!m_Cards.empty()) {
        int currentIndex = std::round(-m_ScrollX / SPACING);

        if (currentIndex < 0) currentIndex = 0;
        if (currentIndex >= static_cast<int>(m_Cards.size())) currentIndex = m_Cards.size() - 1;

        // 如果算出來的 Index 跟上一幀不一樣，代表滑動到了新的一格！
        if (currentIndex != m_CurrentCenterIndex) {
            m_CurrentCenterIndex = currentIndex;

            // 播放滑動切換音效
            Util::SFX(RESOURCE_DIR "/music/choose.mp3").Play();
        }
    }
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
void DeckUI::SetUpgradeMode(bool isUpgrade) {
    m_IsUpgradeMode = isUpgrade;
}
void DeckUI::SetOnCatSelected(std::function<void(UnitID)> callback) {
    m_OnCatSelected = callback;
}

void DeckUI::SetOnSwapClick(std::function<void(UnitID)> callback) {
    m_OnSwapClick = callback;
}