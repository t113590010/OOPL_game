#include "StageCardUI.hpp"

#include "Util/Color.hpp"

StageCardUI::StageCardUI(
    int stageID,
    const std::string& stageName,
    float posRatioX,
    float posRatioY
)
    :
    m_StageID(stageID),
    m_StageName(stageName)
{
    m_BackgroundBtn =
        std::make_shared<Button>(
            posRatioX,
            posRatioY,
            m_Width,
            m_Height,
            RESOURCE_DIR "/img/levelBTN.png",
            stageName,
            32,
            Util::Color(0,0,0,255)
        );

    m_BaseCenter =
        m_BackgroundBtn
            ->m_Transform.translation;

    m_BgBaseScale =
        m_BackgroundBtn
            ->m_Transform.scale;
}

void StageCardUI::Update()
{
    if (!m_IsVisible)
    {
        return;
    }

    if (m_BackgroundBtn)
    {
        m_BackgroundBtn->Update();
    }
}

void StageCardUI::Draw()
{
    if (!m_IsVisible)
    {
        return;
    }

    if (m_BackgroundBtn)
    {
        m_BackgroundBtn->Draw();
    }
}

void StageCardUI::ApplyTransform(
    float finalX,
    float scale
)
{
    if (!m_BackgroundBtn)
    {
        return;
    }

    m_BackgroundBtn
        ->m_Transform.translation.x =
        finalX;

    m_BackgroundBtn
        ->m_Transform.translation.y =
        m_BaseCenter.y;

    m_BackgroundBtn
        ->m_Transform.scale =
        m_BgBaseScale * scale;
    m_BackgroundBtn->SetTextPosition(
    finalX,
    m_BaseCenter.y
    );
}

void StageCardUI::SetOnClick(
    std::function<void(int)> callback
)
{
    if (!m_BackgroundBtn)
    {
        return;
    }

    m_BackgroundBtn->SetOnClick(
        [this, callback]()
        {
            if (callback)
            {
                callback(
                    m_StageID
                );
            }
        }
    );
}