#include "System/StartDebugMenu.hpp"

#include "GameConfig.hpp"
#include "PlayerData.hpp"
#include "Util/Color.hpp"
#include "Util/SFX.hpp"
#include "Util/Logger.hpp"

namespace StartDebugCut
{
    const int FRAME_X = 255;
    const int FRAME_Y = 207;
    const int FRAME_W = 260;
    const int FRAME_H = 50;

    const float buttonWeight = 160.0f;
    const float buttonHeight = 12.0f;

    const float hitBoxWidth = 240.0f;
    const float hitBoxHeight = 38.0f;
}

StartDebugMenu::StartDebugMenu()
{
    // =========================
    // 背景面板
    // =========================

    m_PanelBg =
        std::make_shared<Button>(
            0.0f,
            0.0f,
            620.0f,
            420.0f,
            RESOURCE_DIR "/img/black.png",
            "",
            40,
            Util::Color(255, 255, 255, 255)
        );

    m_PanelBg->SetZIndex(60);

    // =========================
    // +10 銀券
    // =========================

    m_AddNormalTicketBtn =
        std::make_shared<Button>(
            0.0f,
            0.26f,
            StartDebugCut::buttonWeight,
            StartDebugCut::buttonHeight,
            RESOURCE_DIR "/img/img010_tw.png",
            "+10 SILVER",
            28,
            Util::Color(255, 255, 255, 255)
        );

    m_AddNormalTicketBtn->SetZIndex(65);

    m_AddNormalTicketBtn->SetHitBoxSize(
        StartDebugCut::hitBoxWidth,
        StartDebugCut::hitBoxHeight
    );

    m_AddNormalTicketBtn->SetClipRect(
        StartDebugCut::FRAME_X,
        StartDebugCut::FRAME_Y,
        StartDebugCut::FRAME_W,
        StartDebugCut::FRAME_H
    );

    m_AddNormalTicketBtn->SetOnClick(
        []()
        {
            Util::SFX(
                RESOURCE_DIR "/music/clickbtn.mp3"
            ).Play();

            auto playerData =
                PlayerData::GetInstance();

            playerData->AddNormalTickets(
                10
            );

            playerData->SaveToFile();

            LOG_DEBUG(
                "StartDebugMenu: Add 10 normal tickets"
            );
        }
    );

    // =========================
    // +10 金券
    // =========================

    m_AddRareTicketBtn =
        std::make_shared<Button>(
            0.0f,
            0.08f,
            StartDebugCut::buttonWeight,
            StartDebugCut::buttonHeight,
            RESOURCE_DIR "/img/img010_tw.png",
            "+10 GOLD",
            28,
            Util::Color(255, 255, 255, 255)
        );

    m_AddRareTicketBtn->SetZIndex(65);

    m_AddRareTicketBtn->SetHitBoxSize(
        StartDebugCut::hitBoxWidth,
        StartDebugCut::hitBoxHeight
    );

    m_AddRareTicketBtn->SetClipRect(
        StartDebugCut::FRAME_X,
        StartDebugCut::FRAME_Y,
        StartDebugCut::FRAME_W,
        StartDebugCut::FRAME_H
    );

    m_AddRareTicketBtn->SetOnClick(
        []()
        {
            Util::SFX(
                RESOURCE_DIR "/music/clickbtn.mp3"
            ).Play();

            auto playerData =
                PlayerData::GetInstance();

            playerData->AddRareTickets(
                10
            );

            playerData->SaveToFile();

            LOG_DEBUG(
                "StartDebugMenu: Add 10 rare tickets"
            );
        }
    );

    // =========================
    // +1000 貓罐頭
    // =========================

    m_AddCatFoodBtn =
        std::make_shared<Button>(
            0.0f,
            -0.10f,
            StartDebugCut::buttonWeight,
            StartDebugCut::buttonHeight,
            RESOURCE_DIR "/img/img010_tw.png",
            "+1000 CATFOOD",
            26,
            Util::Color(255, 255, 255, 255)
        );

    m_AddCatFoodBtn->SetZIndex(65);

    m_AddCatFoodBtn->SetHitBoxSize(
        StartDebugCut::hitBoxWidth,
        StartDebugCut::hitBoxHeight
    );

    m_AddCatFoodBtn->SetClipRect(
        StartDebugCut::FRAME_X,
        StartDebugCut::FRAME_Y,
        StartDebugCut::FRAME_W,
        StartDebugCut::FRAME_H
    );

    m_AddCatFoodBtn->SetOnClick(
        []()
        {
            Util::SFX(
                RESOURCE_DIR "/music/clickbtn.mp3"
            ).Play();

            auto playerData =
                PlayerData::GetInstance();

            playerData->AddCatFood(
                1000
            );

            playerData->SaveToFile();

            LOG_DEBUG(
                "StartDebugMenu: Add 1000 cat food"
            );
        }
    );

    // =========================
    // 返回
    // =========================

    m_BackBtn =
        std::make_shared<Button>(
            0.0f,
            -0.30f,
            StartDebugCut::buttonWeight,
            StartDebugCut::buttonHeight,
            RESOURCE_DIR "/img/img010_tw.png",
            "BACK",
            28,
            Util::Color(255, 255, 255, 255)
        );

    m_BackBtn->SetZIndex(65);

    m_BackBtn->SetHitBoxSize(
        StartDebugCut::hitBoxWidth,
        StartDebugCut::hitBoxHeight
    );

    m_BackBtn->SetClipRect(
        StartDebugCut::FRAME_X,
        StartDebugCut::FRAME_Y,
        StartDebugCut::FRAME_W,
        StartDebugCut::FRAME_H
    );

    m_BackBtn->SetOnClick(
        [this]()
        {
            Util::SFX(
                RESOURCE_DIR "/music/clickbtn.mp3"
            ).Play();

            if (m_OnBack)
            {
                m_OnBack();
            }
        }
    );
}

void StartDebugMenu::Update()
{
    if (m_AddNormalTicketBtn)
    {
        m_AddNormalTicketBtn->Update();
    }

    if (m_AddRareTicketBtn)
    {
        m_AddRareTicketBtn->Update();
    }

    if (m_AddCatFoodBtn)
    {
        m_AddCatFoodBtn->Update();
    }

    if (m_BackBtn)
    {
        m_BackBtn->Update();
    }
}

void StartDebugMenu::Draw()
{
    if (m_PanelBg)
    {
        m_PanelBg->Draw();
    }

    if (m_AddNormalTicketBtn)
    {
        m_AddNormalTicketBtn->Draw();
    }

    if (m_AddRareTicketBtn)
    {
        m_AddRareTicketBtn->Draw();
    }

    if (m_AddCatFoodBtn)
    {
        m_AddCatFoodBtn->Draw();
    }

    if (m_BackBtn)
    {
        m_BackBtn->Draw();
    }
}

void StartDebugMenu::SetOnBack(
    std::function<void()> callback
)
{
    m_OnBack =
        callback;
}