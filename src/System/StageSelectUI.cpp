#include "StageSelectUI.hpp"
#include <StageConfig.hpp>
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/SFX.hpp"

#include "Core/Context.hpp"

StageSelectUI::StageSelectUI() {
}

void StageSelectUI::LoadStages(
    const std::vector<int>& stageList
) {
    m_Cards.clear();

    m_ScrollX = 0.0f;
    m_TargetScrollX = 0.0f;
    m_CurrentCenterIndex = 0;

    float baseYRatio = m_BaseYRatio;

    for (size_t i = 0; i < stageList.size(); ++i) {

        int stageID = stageList[i];

        auto displayIt =
            STAGE_DISPLAY_DATA.find(stageID);

        if (displayIt == STAGE_DISPLAY_DATA.end()) {
            continue;
        }

        const auto& display =
            displayIt->second;

        auto card =
            std::make_shared<StageCardUI>(
                stageID,
                display.stageName,
                0.0f,
                baseYRatio
            );

        card->SetOnClick(
            [this](int selectedStageID) {

                if (!m_IsDragging && m_OnStageSelected) {
                    m_OnStageSelected(
                        selectedStageID - 1
                    );
                }
            }
        );

        m_Cards.push_back(card);
    }
}

int StageSelectUI::GetCenterStageID() const
{
    if (m_Cards.empty())
        return -1;

    float SPACING = 320.0f;

    int nearestIndex =
        std::round(-m_ScrollX / SPACING);

    if (nearestIndex < 0)
        nearestIndex = 0;

    if (nearestIndex >=
        static_cast<int>(m_Cards.size()))
    {
        nearestIndex =
            m_Cards.size() - 1;
    }

    return m_Cards[nearestIndex]
        ->GetStageID();
}


void StageSelectUI::Update()
{
    if (m_Cards.empty())
        return;

    auto context =
        Core::Context::GetInstance();

    float halfW =
        context->GetWindowWidth() / 2.0f;

    float halfH =
        context->GetWindowHeight() / 2.0f;

    float absoluteY =
        halfH * m_BaseYRatio;

    float SPACING = 320.0f;

    // =========================
    // 滑鼠拖曳
    // =========================

    glm::vec2 mousePos =
        Util::Input::GetCursorPosition();

    bool isMouseDown =
        Util::Input::IsKeyPressed(
            Util::Keycode::MOUSE_LB
        );

    bool isInScrollArea =
        (
            mousePos.y > absoluteY - 180.0f
            &&
            mousePos.y < absoluteY + 180.0f
        );

    if (isMouseDown)
    {
        if (!m_IsDragging && isInScrollArea)
        {
            m_IsDragging = true;

            m_LastMouseX =
                mousePos.x;

            m_TargetScrollX =
                m_ScrollX;
        }
        else if (m_IsDragging)
        {
            m_ScrollX +=
                (
                    mousePos.x
                    -
                    m_LastMouseX
                );

            m_TargetScrollX =
                m_ScrollX;

            m_LastMouseX =
                mousePos.x;
        }
    }
    else
    {
        if (m_IsDragging)
        {
            m_IsDragging = false;

            int nearestIndex =
                std::round(
                    -m_ScrollX
                    /
                    SPACING
                );

            m_TargetScrollX =
                -nearestIndex
                *
                SPACING;
        }
    }

    // =========================
    // 邊界限制
    // =========================

    float maxScroll = 0.0f;

    float minScroll =
        -(
            (m_Cards.size() - 1)
            *
            SPACING
        );

    if (minScroll > 0)
    {
        minScroll = 0.0f;
    }

    if (m_TargetScrollX > maxScroll)
    {
        m_TargetScrollX =
            maxScroll;
    }

    if (m_TargetScrollX < minScroll)
    {
        m_TargetScrollX =
            minScroll;
    }

    // =========================
    // 平滑吸附
    // =========================

    if (!m_IsDragging)
    {
        m_ScrollX +=
            (
                m_TargetScrollX
                -
                m_ScrollX
            )
            *
            0.15f;
    }

    // =========================
    // 中央卡片切換音效
    // =========================

    int currentIndex =
        std::round(
            -m_ScrollX
            /
            SPACING
        );

    if (currentIndex < 0)
    {
        currentIndex = 0;
    }

    if (
        currentIndex >=
        static_cast<int>(
            m_Cards.size()
        )
    )
    {
        currentIndex =
            static_cast<int>(
                m_Cards.size()
            ) - 1;
    }

    if (
        currentIndex
        !=
        m_CurrentCenterIndex
    )
    {
        m_CurrentCenterIndex =
            currentIndex;

        Util::SFX(
            RESOURCE_DIR
            "/music/choose.mp3"
        ).Play();
    }
    int stageID =
            m_Cards[
                m_CurrentCenterIndex
            ]->GetStageID();

    auto it =
        STAGE_DISPLAY_DATA.find(
            stageID
        );

    if (
        it
        !=
        STAGE_DISPLAY_DATA.end()
    )
    {
        LOG_DEBUG(
            "Selected Stage: {} ({})",
            stageID,
            it->second.stageName
        );
    }
    // =========================
    // 更新卡片
    // =========================

    for (
        size_t i = 0;
        i < m_Cards.size();
        ++i
    )
    {
        float rawX =
            (i * SPACING)
            +
            m_ScrollX;

        if (
            rawX < -halfW - 300.0f
            ||
            rawX > halfW + 300.0f
        )
        {
            m_Cards[i]
                ->SetVisible(false);

            continue;
        }

        m_Cards[i]
            ->SetVisible(true);

        float dist =
            std::abs(rawX);

        float weight =
            std::max(
                0.0f,
                1.0f
                -
                (
                    dist
                    /
                    SPACING
                )
            );

        float targetScale =
            0.6f
            +
            (
                0.4f
                *
                weight
            );

        float pushDirection =
            (
                rawX > 0
            )
            ?
            1.0f
            :
            (
                (
                    rawX < 0
                )
                ?
                -1.0f
                :
                0.0f
            );

        float pushAmount =
            70.0f;

        float pushOffset =
            pushDirection
            *
            pushAmount
            *
            (
                1.0f
                -
                weight
            );

        float finalX =
            rawX
            +
            pushOffset;

        m_Cards[i]
            ->ApplyTransform(
                finalX,
                targetScale
            );

        m_Cards[i]
            ->Update();
    }
}

void StageSelectUI::Draw()
{
    for (auto& card : m_Cards)
    {
        card->Draw();
    }
}
void StageSelectUI::SetOnStageSelected(
    std::function<void(int)> callback
)
{
    m_OnStageSelected = callback;

}
