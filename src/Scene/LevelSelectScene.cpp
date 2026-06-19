#include "LevelSelectScene.hpp"
#include "StageConfig.hpp"
#include "Core/Context.hpp"
#include "Util/Color.hpp"
#include "Util/Image.hpp"
#include <PlayerData.hpp>
#include "Util/SFX.hpp"
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
    auto battleStartImage =
        std::make_shared<Util::Image>(
            RESOURCE_DIR "/img/battleStartFont.png"
        );

    m_BattleStartFont.SetDrawable(
        battleStartImage
    );

    m_BattleStartFont.SetZIndex(50);

    // 稍微偏右
    m_BattleStartFont
        .m_Transform.translation =
        glm::vec2(200.0f, 0.0f);

    // 圖片原始比例
    m_BattleStartFontBaseScale =
        glm::vec2(1.0f, 1.0f);

    // 建構時先設定成動畫結束後的大小
    m_BattleStartFont
        .m_Transform.scale =
        m_BattleStartFontBaseScale *
        BATTLE_START_MIN_SCALE;
    LOG_DEBUG("LevelSelectScene Created");
    // ============================
    // 背景
    // ============================
    m_SelectedStageID = 1;
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
    m_WindowWidth = windowWidth;
    m_WindowHeight = windowHeight;

    glm::vec2 imgSize =
        bgImage->GetSize();

    m_Background.m_Transform.scale =
        glm::vec2(
            windowWidth / imgSize.x,
            windowHeight / imgSize.y
        );


    auto blackImage =
    std::make_shared<Util::Image>(
        RESOURCE_DIR "/img/blackscreen.png"
    );

    m_BlackOverlay.SetDrawable(
        blackImage
    );

    m_BlackOverlay.SetZIndex(100);

    glm::vec2 blackImageSize =
        blackImage->GetSize();

    const float overlayWidth =
        m_WindowWidth * 1.35f;

    const float overlayHeight =
        m_WindowHeight * 1.15f;

    m_BlackOverlay.m_Transform.scale =
        glm::vec2(
            overlayWidth / blackImageSize.x,
            overlayHeight / blackImageSize.y
        );

    m_BlackOverlayStartX =
        m_WindowWidth * 0.5f +
        overlayWidth * 0.5f;

    // 最後稍微偏右，將右側裂縫推出螢幕
    m_BlackOverlayEndX = 180.0f;

    m_BlackOverlay.m_Transform.translation =
        glm::vec2(
            m_BlackOverlayStartX,
            0.0f
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
        Util::SFX(RESOURCE_DIR "/music/clickbtn.mp3").Play();

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
            m_SelectedStageID = stageID;
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
            LOG_DEBUG(
                "Start Battle Clicked, Selected Stage = {}",
                m_SelectedStageID
            );

            StartBattleTransition(
                m_SelectedStageID - 1
            );
        }
);

}

void LevelSelectScene::Update()
{
    if (m_IsStartingBattle)
    {
        UpdateBattleTransition();
        return;
    }

    if (m_BackBtn)
    {
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
    if (
        m_IsStartingBattle &&
        m_StartBattleTimer <
            BATTLE_START_FONT_DURATION
    )
    {
        m_BattleStartFont.Draw();
    }

    if (
        m_IsStartingBattle &&
        m_StartBattleTimer >=
            BATTLE_START_FONT_DURATION
    )
    {
        m_BlackOverlay.Draw();
    }
}

void LevelSelectScene::StartBattleTransition(
    int stageID
)
{
    if (m_IsStartingBattle)
    {
        return;
    }

    m_IsStartingBattle = true;
    m_StartBattleTimer = 0.0f;
    m_PendingStageID = stageID;

    m_BattleStartFont
        .m_Transform.scale =
        m_BattleStartFontBaseScale *
        BATTLE_START_MAX_SCALE;

    // 重設黑幕位置
    m_BlackOverlay.m_Transform.translation =
        glm::vec2(
            m_BlackOverlayStartX,
            0.0f
        );

    Util::SFX(
        RESOURCE_DIR
        "/music/StartBattle.mp3"
    ).Play();

    LOG_DEBUG(
        "Start battle transition, stageID = {}",
        stageID
    );
}

void LevelSelectScene::UpdateBattleTransition()
{
    constexpr float dt = 0.016f;

    m_StartBattleTimer += dt;

    // ============================
    // 前 0.6 秒：字幕由大縮小
    // ============================
    if (
        m_StartBattleTimer <=
        BATTLE_START_SCALE_DURATION
    )
    {
        float progress =
            m_StartBattleTimer /
            BATTLE_START_SCALE_DURATION;

        progress = std::clamp(
            progress,
            0.0f,
            1.0f
        );

        float smoothProgress =
            progress *
            progress *
            (
                3.0f -
                2.0f * progress
            );

        float scaleMultiplier =
            BATTLE_START_MAX_SCALE +
            (
                BATTLE_START_MIN_SCALE -
                BATTLE_START_MAX_SCALE
            ) *
            smoothProgress;

        m_BattleStartFont
            .m_Transform.scale =
            m_BattleStartFontBaseScale *
            scaleMultiplier;
    }
    else
    {
        m_BattleStartFont
            .m_Transform.scale =
            m_BattleStartFontBaseScale *
            BATTLE_START_MIN_SCALE;
    }

    // ============================
    // 最後 1 秒：黑幕從右往左滑入
    // ============================
    if (
    m_StartBattleTimer >=
    BATTLE_START_FONT_DURATION
)
    {
        float blackProgress =
            (
                m_StartBattleTimer -
                BATTLE_START_FONT_DURATION
            ) /
            BLACK_FADE_DURATION;

        blackProgress =
            std::clamp(
                blackProgress,
                0.0f,
                1.0f
            );

        float smoothBlackProgress =
            blackProgress *
            blackProgress *
            (
                3.0f -
                2.0f * blackProgress
            );

        float currentX =
            m_BlackOverlayStartX *
            (
                1.0f -
                smoothBlackProgress
            );

        m_BlackOverlay.m_Transform.translation =
            glm::vec2(
                currentX,
                0.0f
            );
    }

    // ============================
    // 過場結束，進入戰鬥
    // ============================
    if (
        m_StartBattleTimer >=
        BATTLE_TRANSITION_DURATION
    )
    {
        m_IsStartingBattle = false;
        m_StartBattleTimer = 0.0f;

        const int stageID =
            m_PendingStageID;

        m_PendingStageID = -1;

        if (m_OnStageSelected)
        {
            m_OnStageSelected(stageID);
        }
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