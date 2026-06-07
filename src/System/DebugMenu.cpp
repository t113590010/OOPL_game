#include "System/DebugMenu.hpp"

#include "GameConfig.hpp"
#include "Util/Color.hpp"
#include "Util/SFX.hpp"
namespace DebugCut {
    const int FRAME_X = 255;
    const int FRAME_Y = 207;
    const int FRAME_W = 260;
    const int FRAME_H = 50;
    const float buttonWeight = 160.0f;
    const float buttonHeight = 12.0f;
    const float hitBoxWidth = 240.0f;
    const float hitBoxHeight = 38.0f;
}
DebugMenu::DebugMenu()
{
    // =========================
    // 背景面板
    // =========================

    m_PanelBg =
        std::make_shared<Button>(
            0.0f,
            0.0f,
            620.0f,
            520.0f,
            RESOURCE_DIR "/img/black.png",
            "",
            40,
            Util::Color(255, 255, 255, 255)
        );

    m_PanelBg->SetZIndex(60);

    // =========================
    // 按鈕
    // =========================

    m_AddXPBtn =
        std::make_shared<Button>(
            0.0f,
            0.4f,
            DebugCut::buttonWeight,
            DebugCut::buttonHeight,
            RESOURCE_DIR "/img/img010_tw.png",
            "+10000 XP",
            28,
            Util::Color(255, 255, 255, 255)
        );

    m_AddXPBtn->SetZIndex(65);
    m_AddXPBtn->SetHitBoxSize(
    DebugCut::hitBoxWidth,
    DebugCut::hitBoxHeight
);
    m_AddXPBtn->SetClipRect(
        DebugCut::FRAME_X,
        DebugCut::FRAME_Y,
        DebugCut::FRAME_W,
        DebugCut::FRAME_H
    );
    m_AddXPBtn->SetOnClick(
        [this]()
        {
            Util::SFX(
                RESOURCE_DIR "/music/clickbtn.mp3"
            ).Play();

            if (m_OnAddXP)
            {
                m_OnAddXP();
            }
        }
    );

    m_MaxMoneyBtn =
        std::make_shared<Button>(
            0.0f,
            0.22f,
            DebugCut::buttonWeight,
            DebugCut::buttonHeight,
            RESOURCE_DIR "/img/img010_tw.png",
            "MAX MONEY OFF",
            28,
            Util::Color(255, 255, 255, 255)
        );

    m_MaxMoneyBtn->SetZIndex(65);
    m_MaxMoneyBtn->SetHitBoxSize(
    DebugCut::hitBoxWidth,
    DebugCut::hitBoxHeight
);
    m_MaxMoneyBtn->SetClipRect(
        DebugCut::FRAME_X,
        DebugCut::FRAME_Y,
        DebugCut::FRAME_W,
        DebugCut::FRAME_H
    );
    m_MaxMoneyBtn->SetOnClick(
        [this]()
        {
            Util::SFX(
                RESOURCE_DIR "/music/clickbtn.mp3"
            ).Play();

            m_MaxMoneyEnabled =
                !m_MaxMoneyEnabled;

            if (m_MaxMoneyBtn)
            {
                m_MaxMoneyBtn->UpdateText(
                    m_MaxMoneyEnabled
                        ? "MAX MONEY ON"
                        : "MAX MONEY OFF"
                );
            }

            if (m_OnMaxMoney)
            {
                m_OnMaxMoney();
            }
        }
    );

    m_ToggleCatAttackBtn =
        std::make_shared<Button>(
            0.0f,
            0.04f,
            DebugCut::buttonWeight,
            DebugCut::buttonHeight,
            RESOURCE_DIR "/img/img010_tw.png",
            "CAT ATK x2 OFF",
            28,
            Util::Color(255, 255, 255, 255)
        );
    m_ToggleCatAttackBtn->SetHitBoxSize(
    DebugCut::hitBoxWidth,
    DebugCut::hitBoxHeight
);
    m_ToggleCatAttackBtn->SetZIndex(65);
    m_ToggleCatAttackBtn->SetClipRect(
        DebugCut::FRAME_X,
        DebugCut::FRAME_Y,
        DebugCut::FRAME_W,
        DebugCut::FRAME_H
    );
    m_ToggleCatAttackBtn->SetOnClick(
        [this]()
        {
            Util::SFX(
                RESOURCE_DIR "/music/clickbtn.mp3"
            ).Play();

            m_CatAttackBoostEnabled =
                !m_CatAttackBoostEnabled;

            if (m_ToggleCatAttackBtn)
            {
                m_ToggleCatAttackBtn->UpdateText(
                    m_CatAttackBoostEnabled
                        ? "CAT ATK x2 ON"
                        : "CAT ATK x2 OFF"
                );
            }

            if (m_OnToggleCatAttack)
            {
                m_OnToggleCatAttack();
            }
        }
    );

    m_ToggleCatSpeedBtn =
        std::make_shared<Button>(
            0.0f,
            -0.14f,
            DebugCut::buttonWeight,
            DebugCut::buttonHeight,
            RESOURCE_DIR "/img/img010_tw.png",
            "CAT SPEED x2 OFF",
            28,
            Util::Color(255, 255, 255, 255)
        );
    m_ToggleCatSpeedBtn->SetHitBoxSize(
    DebugCut::hitBoxWidth,
    DebugCut::hitBoxHeight
);
    m_ToggleCatSpeedBtn->SetZIndex(65);
    m_ToggleCatSpeedBtn->SetClipRect(
        DebugCut::FRAME_X,
        DebugCut::FRAME_Y,
        DebugCut::FRAME_W,
        DebugCut::FRAME_H
    );
    m_ToggleCatSpeedBtn->SetOnClick(
        [this]()
        {
            Util::SFX(
                RESOURCE_DIR "/music/clickbtn.mp3"
            ).Play();

            m_CatSpeedBoostEnabled =
                !m_CatSpeedBoostEnabled;

            if (m_ToggleCatSpeedBtn)
            {
                m_ToggleCatSpeedBtn->UpdateText(
                    m_CatSpeedBoostEnabled
                        ? "CAT SPEED x2 ON"
                        : "CAT SPEED x2 OFF"
                );
            }

            if (m_OnToggleCatSpeed)
            {
                m_OnToggleCatSpeed();
            }
        }
    );



    m_InstantWinBtn =
        std::make_shared<Button>(
            0.0f,
            -0.32f,
            DebugCut::buttonWeight,
            DebugCut::buttonHeight,
            RESOURCE_DIR "/img/img010_tw.png",
            "INSTANT WIN",
            28,
            Util::Color(255, 255, 255, 255)
        );
    m_InstantWinBtn->SetHitBoxSize(
    DebugCut::hitBoxWidth,
    DebugCut::hitBoxHeight
);
    m_InstantWinBtn->SetZIndex(65);
    m_InstantWinBtn->SetClipRect(
        DebugCut::FRAME_X,
        DebugCut::FRAME_Y,
        DebugCut::FRAME_W,
        DebugCut::FRAME_H
    );
    m_InstantWinBtn->SetOnClick(
        [this]()
        {
            Util::SFX(
                RESOURCE_DIR "/music/clickbtn.mp3"
            ).Play();

            if (m_OnInstantWin)
            {
                m_OnInstantWin();
            }
        }
    );

    m_BackBtn =
        std::make_shared<Button>(
            0.0f,
            -0.5f,
            DebugCut::buttonWeight,
            DebugCut::buttonHeight,
            RESOURCE_DIR "/img/img010_tw.png",
            "BACK",
            28,
            Util::Color(255, 255, 255, 255)
        );
    m_BackBtn->SetHitBoxSize(
    DebugCut::hitBoxWidth,
    DebugCut::hitBoxHeight
);
    m_BackBtn->SetZIndex(65);
    m_BackBtn->SetClipRect(
        DebugCut::FRAME_X,
        DebugCut::FRAME_Y,
        DebugCut::FRAME_W,
        DebugCut::FRAME_H
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

void DebugMenu::Update()
{
    if (m_AddXPBtn)
    {
        m_AddXPBtn->Update();
    }

    if (m_MaxMoneyBtn)
    {
        m_MaxMoneyBtn->Update();
    }

    if (m_ToggleCatAttackBtn)
    {
        m_ToggleCatAttackBtn->Update();
    }

    if (m_ToggleCatSpeedBtn)
    {
        m_ToggleCatSpeedBtn->Update();
    }


    if (m_InstantWinBtn)
    {
        m_InstantWinBtn->Update();
    }

    if (m_BackBtn)
    {
        m_BackBtn->Update();
    }
}

void DebugMenu::Draw()
{
    if (m_PanelBg)
    {
        m_PanelBg->Draw();
    }

    if (m_AddXPBtn)
    {
        m_AddXPBtn->Draw();
    }

    if (m_MaxMoneyBtn)
    {
        m_MaxMoneyBtn->Draw();
    }

    if (m_ToggleCatAttackBtn)
    {
        m_ToggleCatAttackBtn->Draw();
    }

    if (m_ToggleCatSpeedBtn)
    {
        m_ToggleCatSpeedBtn->Draw();
    }


    if (m_InstantWinBtn)
    {
        m_InstantWinBtn->Draw();
    }

    if (m_BackBtn)
    {
        m_BackBtn->Draw();
    }
}

// =========================
// Callback setters
// =========================

void DebugMenu::SetOnAddXP(
    std::function<void()> callback
)
{
    m_OnAddXP = callback;
}

void DebugMenu::SetOnMaxMoney(
    std::function<void()> callback
)
{
    m_OnMaxMoney = callback;
}

void DebugMenu::SetOnToggleCatAttack(
    std::function<void()> callback
)
{
    m_OnToggleCatAttack = callback;
}

void DebugMenu::SetOnToggleCatSpeed(
    std::function<void()> callback
)
{
    m_OnToggleCatSpeed = callback;
}


void DebugMenu::SetOnInstantWin(
    std::function<void()> callback
)
{
    m_OnInstantWin = callback;
}

void DebugMenu::SetOnBack(
    std::function<void()> callback
)
{
    m_OnBack = callback;
}