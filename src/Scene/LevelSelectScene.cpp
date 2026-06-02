#include "LevelSelectScene.hpp"
#include "StageConfig.hpp"
#include "Core/Context.hpp"
#include "Util/Color.hpp"
#include "Util/Image.hpp"
#include <PlayerData.hpp>
namespace LevelSelectCut {
    const float RETURN_ICON_X = 423.0f;
    const float RETURN_ICON_Y = 170.0f;
    const float RETURN_ICON_W = 511.0f - RETURN_ICON_X;
    const float RETURN_ICON_H = 232.0f - RETURN_ICON_Y;

    const float RETURN_BASE_X = 256.0f;
    const float RETURN_BASE_Y = 0.0f;
    const float RETURN_BASE_W = 99.0f;
    const float RETURN_BASE_H = 97.0f;
}
LevelSelectScene::LevelSelectScene() {
    LOG_DEBUG("LevelSelectScene Created");
    // ============================
    // 背景
    // ============================

    auto bgImage =
        std::make_shared<Util::Image>(
            RESOURCE_DIR "/img/homeBackground.png"
        );

    m_Background.SetDrawable(bgImage);
    m_Background.SetZIndex(10);

    auto context = Core::Context::GetInstance();

    float windowWidth =
        (float)context->GetWindowWidth();

    float windowHeight =
        (float)context->GetWindowHeight();

    glm::vec2 imgSize =
        bgImage->GetSize();

    m_Background.m_Transform.scale =
        glm::vec2(
            windowWidth / imgSize.x,
            windowHeight / imgSize.y
        );
    // ============================
    // 返回按鈕
    // ============================

    float btnSize = 100.0f;

    m_BackBtn = std::make_shared<Button>(
        -0.805f,
        -0.87f,
        btnSize,
        btnSize,
        RESOURCE_DIR"/img/img006_tw.png",
        " ",
        30,
        Util::Color(255,255,255,255)
    );

    m_BackBtn->SetOnClick([this]() {

        if (m_OnReturn) {
            m_OnReturn();
        }
    });

    m_StageSelectUI =
        std::make_shared<StageSelectUI>();

    std::vector<int> stageList;

    int maxUnlocked =
        PlayerData::GetInstance()
            ->GetMaxUnlockedStage();

    for (int i = 1; i <= maxUnlocked; i++)
    {
        stageList.push_back(i);
    }

    m_StageSelectUI->LoadStages(
        stageList
    );
    m_StageSelectUI->SetOnStageSelected(
    [this](int stageID)
    {
        if (m_OnStageSelected)
        {
            m_OnStageSelected(
                stageID - 1
            );
            LOG_DEBUG("LevelSelectScene Created");
        }
    }
);

        m_StartBattleBtn =
            std::make_shared<Button>(
                0.45f,     // X
                -0.82f,    // Y
                260.0f,
                110.0f,
                RESOURCE_DIR "/img/battleStart.png",
                " ",
                30,
                Util::Color(255,255,255,255)
            );
    m_StartBattleBtn->SetOnClick(
    [this]()
    {
        if (m_OnStageSelected)
        {
            m_OnStageSelected(
                m_SelectedStageID - 1
            );
        }
    }
);

}

void LevelSelectScene::Update() {

    if (m_BackBtn) {
        m_BackBtn->Update();
    }
    if (m_StageSelectUI)
    {
        m_StageSelectUI->Update();
    }
    if (m_StartBattleBtn)
    {
        m_StartBattleBtn->Update();
    }
}

void LevelSelectScene::Draw() {

    m_Background.Draw();

    // 返回按鈕
    static auto uiAtlas =
        std::make_shared<Util::Image>(
            RESOURCE_DIR"/img/img006_tw.png"
        );

    glm::vec2 sheetSize =
        uiAtlas->GetSize();

    float iconBoost = 0.8f;
    float baseScale = 1.0f;

    if (m_BackBtn)
    {
        m_BackBtn->m_Transform.scale =
        {
            ((float)LevelSelectCut::RETURN_BASE_W / sheetSize.x) * baseScale,
            ((float)LevelSelectCut::RETURN_BASE_H / sheetSize.y) * baseScale
        };

        m_BackBtn->SetZIndex(21);

        m_BackBtn->DrawRect(
            LevelSelectCut::RETURN_BASE_X,
            LevelSelectCut::RETURN_BASE_Y,
            LevelSelectCut::RETURN_BASE_W,
            LevelSelectCut::RETURN_BASE_H
        );

        glm::vec2 originalPos =
            m_BackBtn->m_Transform.translation;

        m_BackBtn->m_Transform.translation +=
            glm::vec2(-0.2f, 0.0f);

        m_BackBtn->m_Transform.scale =
        {
            ((float)LevelSelectCut::RETURN_ICON_W / sheetSize.x) * baseScale * iconBoost,
            ((float)LevelSelectCut::RETURN_ICON_H / sheetSize.y) * baseScale * iconBoost
        };

        m_BackBtn->SetZIndex(22);

        m_BackBtn->DrawRect(
            LevelSelectCut::RETURN_ICON_X,
            LevelSelectCut::RETURN_ICON_Y,
            LevelSelectCut::RETURN_ICON_W,
            LevelSelectCut::RETURN_ICON_H
        );

        m_BackBtn->m_Transform.translation =
            originalPos;
    }
    if (m_StageSelectUI)
    {
        m_StageSelectUI->Draw();
    }
    if (m_StartBattleBtn)
    {
        m_StartBattleBtn->Draw();
    }
}

void LevelSelectScene::SetOnStageSelected(
    std::function<void(int)> callback
) {
    m_OnStageSelected = callback;
}

void LevelSelectScene::SetOnReturnBtnClick(
    std::function<void()> callback
) {
    m_OnReturn = callback;
}