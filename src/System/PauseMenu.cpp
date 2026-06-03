#include "System/PauseMenu.hpp"

#include "Core/Context.hpp"
#include "GameConfig.hpp"
#include "Util/Color.hpp"
#include "Util/SFX.hpp"

PauseMenu::PauseMenu()
{
    auto context =
        Core::Context::GetInstance();

    float windowWidth =
        static_cast<float>(
            context->GetWindowWidth()
        );

    float windowHeight =
        static_cast<float>(
            context->GetWindowHeight()
        );

    // =========================
    // 半透明黑底
    // =========================

    m_Background.SetDrawable(
        std::make_shared<Util::Image>(
            RESOURCE_DIR "/img/black.png"
        )
    );

    m_Background.m_Transform.scale =
        glm::vec2(
            windowWidth,
            windowHeight
        );

    m_Background.SetZIndex(49);

    // =========================
    // 中央主面板
    // 先用 black.png 當底，之後可以換成真正的 pause panel 圖
    // =========================

    m_PanelBg =
        std::make_shared<Button>(
            0.0f,
            0.0f,
            620.0f,
            420.0f,
            RESOURCE_DIR "/img/black.png",
            " ",
            30,
            Util::Color(255, 255, 255, 255)
        );

    m_PanelBg->SetZIndex(50);

    // =========================
    // 標題列：選項
    // 先用 escapeBattle.png 當黃色按鈕底
    // =========================

    m_TitleBar =
        std::make_shared<Button>(
            0.0f,
            0.37f,
            420.0f,
            80.0f,
            RESOURCE_DIR "/img/escapeBattle.png",
            "選項",
            42,
            Util::Color(255, 255, 255, 255)
        );

    m_TitleBar->SetZIndex(52);

    // =========================
    // 右上角 X 關閉
    // =========================

    m_CloseBtn =
        std::make_shared<Button>(
            0.36f,
            0.37f,
            90.0f,
            90.0f,
            RESOURCE_DIR "/img/closebtn.png",
            "X",
            50,
            Util::Color(255, 255, 255, 255)
        );

    m_CloseBtn->SetZIndex(55);

    m_CloseBtn->SetOnClick(
        [this]()
        {
            if (m_OnClose)
            {
                m_OnClose();
            }
        }
    );

    // =========================
    // 幫助 / Debug Menu 按鈕
    // =========================

    m_HelpBtn =
        std::make_shared<Button>(
            -0.18f,
            0.13f,
            230.0f,
            70.0f,
            RESOURCE_DIR "/img/helpbtn.png",
            "幫助",
            34,
            Util::Color(255, 255, 255, 255)
        );

    m_HelpBtn->SetZIndex(55);

    m_HelpBtn->SetOnClick(
        [this]()
        {
            if (m_OnHelp)
            {
                m_OnHelp();
            }
        }
    );

    // =========================
    // BGM 音量滑桿：目前是假 UI
    // =========================

    m_BgmLabel =
        std::make_shared<Button>(
            -0.25f,
            -0.03f,
            140.0f,
            55.0f,
            RESOURCE_DIR "/img/black.png",
            "BGM",
            30,
            Util::Color(255, 255, 255, 255)
        );

    m_BgmLabel->SetZIndex(55);

    m_BgmBar =
        std::make_shared<Button>(
            0.07f,
            -0.03f,
            330.0f,
            35.0f,
            RESOURCE_DIR "/img/escapeBattle.png",
            " ",
            20,
            Util::Color(255, 255, 255, 255)
        );

    m_BgmBar->SetZIndex(54);

    m_BgmKnob =
        std::make_shared<Button>(
            0.16f,
            -0.03f,
            45.0f,
            45.0f,
            RESOURCE_DIR "/img/OKBtn.png",
            " ",
            20,
            Util::Color(255, 255, 255, 255)
        );

    m_BgmKnob->SetZIndex(56);

    // =========================
    // SFX 音量滑桿：目前是假 UI
    // =========================

    m_SfxLabel =
        std::make_shared<Button>(
            -0.25f,
            -0.18f,
            140.0f,
            55.0f,
            RESOURCE_DIR "/img/black.png",
            "SFX",
            30,
            Util::Color(255, 255, 255, 255)
        );

    m_SfxLabel->SetZIndex(55);

    m_SfxBar =
        std::make_shared<Button>(
            0.07f,
            -0.18f,
            330.0f,
            35.0f,
            RESOURCE_DIR "/img/escapeBattle.png",
            " ",
            20,
            Util::Color(255, 255, 255, 255)
        );

    m_SfxBar->SetZIndex(54);

    m_SfxKnob =
        std::make_shared<Button>(
            0.16f,
            -0.18f,
            45.0f,
            45.0f,
            RESOURCE_DIR "/img/OKBtn.png",
            " ",
            20,
            Util::Color(255, 255, 255, 255)
        );

    m_SfxKnob->SetZIndex(56);

    // =========================
    // 脫離戰鬥
    // =========================

    m_QuitBtn =
        std::make_shared<Button>(
            0.0f,
            -0.42f,
            430.0f,
            90.0f,
            RESOURCE_DIR "/img/escapeBattle.png",
            "脫離戰鬥",
            40,
            Util::Color(255, 255, 255, 255)
        );

    m_QuitBtn->SetZIndex(55);

    m_QuitBtn->SetOnClick(
        [this]()
        {
            if (m_OnQuit)
            {
                m_OnQuit();
            }
        }
    );
}

void PauseMenu::Update()
{
    if (m_CloseBtn)
    {
        m_CloseBtn->Update();
    }

    if (m_HelpBtn)
    {
        m_HelpBtn->Update();
    }

    if (m_QuitBtn)
    {
        m_QuitBtn->Update();
    }

    // 目前滑桿只是顯示，不處理拖曳
}

void PauseMenu::Draw()
{
    float cutX = 0.0f;
    float cutY = 0.0f;
    float cutW = 10.0f;
    float cutH = 10.0f;

    // 黑色遮罩
    m_Background.DrawRect(
        cutX,
        cutY,
        cutW,
        cutH
    );

    if (m_PanelBg)
    {
        m_PanelBg->Draw();
    }

    if (m_TitleBar)
    {
        m_TitleBar->Draw();
    }

    if (m_CloseBtn)
    {
        m_CloseBtn->Draw();
    }

    if (m_HelpBtn)
    {
        m_HelpBtn->Draw();
    }

    if (m_BgmLabel)
    {
        m_BgmLabel->Draw();
    }

    if (m_BgmBar)
    {
        m_BgmBar->Draw();
    }

    if (m_BgmKnob)
    {
        m_BgmKnob->Draw();
    }

    if (m_SfxLabel)
    {
        m_SfxLabel->Draw();
    }

    if (m_SfxBar)
    {
        m_SfxBar->Draw();
    }

    if (m_SfxKnob)
    {
        m_SfxKnob->Draw();
    }

    if (m_QuitBtn)
    {
        m_QuitBtn->Draw();
    }
}