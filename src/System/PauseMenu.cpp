#include "System/PauseMenu.hpp"

#include "Core/Context.hpp"
#include "GameConfig.hpp"
#include "Util/Color.hpp"
#include "Util/SFX.hpp"

#include <string>
#include "PlayerData.hpp"
namespace
{
    std::string GetBgmVolumeImagePath(int level)
    {
        return std::string(RESOURCE_DIR)
            + "/img/bgmVolume"
            + std::to_string(level)
            + ".png";
    }

    std::string GetSfxVolumeImagePath(int level)
    {
        return std::string(RESOURCE_DIR)
            + "/img/sfxVolume"
            + std::to_string(level)
            + ".png";
    }
}

PauseMenu::PauseMenu() {
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
    // 整張暫停選單背景
    // =========================

    m_MenuBg =
        std::make_shared<Button>(
            0.0f,
            0.0f,
            620.0f,
            430.0f,
            RESOURCE_DIR "/img/pauseMenuBg.png",
            " ",
            30,
            Util::Color(255, 255, 255, 255)
        );

    m_MenuBg->SetZIndex(50);

    // =========================
    // 右上角 X 關閉
    // =========================

    m_CloseBtn =
        std::make_shared<Button>(
            0.43f,
            0.56f,
            92.0f,
            92.0f,
            RESOURCE_DIR "/img/closebtn.png",
            " ",
            30,
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
    // 幫助 / Debug Menu
    // =========================

    m_HelpBtn =
        std::make_shared<Button>(
            -0.20f,
            0.10f,
            210.0f,
            70.0f,
            RESOURCE_DIR "/img/helpbtn.png",
            " ",
            30,
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
    // BGM 音量按鈕
    // =========================

    m_BgmVolumeLevel =
        PlayerData::GetInstance()
            ->GetBgmVolumeLevel();
    m_BgmVolumeBtn =
        std::make_shared<Button>(
            0.16f,
            0.10f,
            75.0f,
            75.0f,
            GetBgmVolumeImagePath(m_BgmVolumeLevel),
            " ",
            30,
            Util::Color(255, 255, 255, 255)
        );

    m_BgmVolumeBtn->SetZIndex(55);

    m_BgmVolumeBtn->SetOnClick(
        [this]()
        {
            m_BgmVolumeLevel =
                (m_BgmVolumeLevel + 1) % 4;

            m_BgmVolumeBtn->SetImage(
                GetBgmVolumeImagePath(
                    m_BgmVolumeLevel
                )
            );

            auto playerData =
                PlayerData::GetInstance();

            playerData->SetBgmVolumeLevel(
                m_BgmVolumeLevel
            );

            playerData->SaveToFile();
            if (m_OnBgmVolumeChanged)
            {
                m_OnBgmVolumeChanged(
                    m_BgmVolumeLevel
                );
            }
        }
    );

    // =========================
    // SFX 音量按鈕
    // =========================

    m_SfxVolumeLevel =
        PlayerData::GetInstance()
            ->GetSfxVolumeLevel();

    m_SfxVolumeBtn =
        std::make_shared<Button>(
            0.16f,
            -0.1f,
            75.0f,
            75.0f,
            GetSfxVolumeImagePath(m_SfxVolumeLevel),
            " ",
            30,
            Util::Color(255, 255, 255, 255)
        );

    m_SfxVolumeBtn->SetZIndex(55);

    m_SfxVolumeBtn->SetOnClick(
        [this]()
        {
            m_SfxVolumeLevel =
                (m_SfxVolumeLevel + 1) % 4;

            m_SfxVolumeBtn->SetImage(
                GetSfxVolumeImagePath(
                    m_SfxVolumeLevel
                )
            );
            auto playerData =
                PlayerData::GetInstance();

            playerData->SetSfxVolumeLevel(
                m_SfxVolumeLevel
            );

            playerData->SaveToFile();
            int volume = 0;

            if (m_SfxVolumeLevel == 0)
            {
                volume = 0;
            }
            else if (m_SfxVolumeLevel == 1)
            {
                volume = 40;
            }
            else if (m_SfxVolumeLevel == 2)
            {
                volume = 80;
            }
            else if (m_SfxVolumeLevel == 3)
            {
                volume = 128;
            }

            Util::SFX::SetGlobalVolume(
                volume
            );

            LOG_DEBUG(
                "SFX Volume Level = {}, Volume = {}",
                m_SfxVolumeLevel,
                volume
            );

            // 播放測試音效，讓玩家立刻聽到目前音量
            Util::SFX(
                RESOURCE_DIR "/music/clickbtn.mp3"
            ).Play();

            if (m_OnSfxVolumeChanged)
            {
                m_OnSfxVolumeChanged(
                    m_SfxVolumeLevel
                );
            }
        }
 );

    // =========================
    // 脫離戰鬥透明點擊區
    // =========================
    // 注意：
    // 因為 pauseMenuBg.png 裡已經有「脫離戰鬥」外觀，
    // 這顆按鈕只負責接收點擊。
    // Draw() 裡不要畫它。

    m_QuitBtn =
        std::make_shared<Button>(
            0.0f,
            -0.43f,
            430.0f,
            90.0f,
            RESOURCE_DIR "/img/black.png",
            " ",
            30,
            Util::Color(0, 0, 0, 0)
        );

    m_QuitBtn->SetImage(
        RESOURCE_DIR "/img/black.png"
    );

    m_QuitBtn->SetZIndex(56);


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

    if (m_BgmVolumeBtn)
    {
        m_BgmVolumeBtn->Update();
    }

    if (m_SfxVolumeBtn)
    {
        m_SfxVolumeBtn->Update();
    }

    if (m_QuitBtn)
    {
        m_QuitBtn->Update();
    }
}
void PauseMenu::DrawBackgroundOnly()
{
    m_Background.DrawRect(
        0.0f,
        0.0f,
        10.0f,
        10.0f
    );
}
void PauseMenu::Draw()
{
    // 黑色遮罩
    m_Background.DrawRect(
        0.0f,
        0.0f,
        10.0f,
        10.0f
    );

    if (m_MenuBg)
    {
        m_MenuBg->Draw();
    }

    if (m_CloseBtn)
    {
        m_CloseBtn->Draw();
    }

    if (m_HelpBtn)
    {
        m_HelpBtn->Draw();
    }

    if (m_BgmVolumeBtn)
    {
        m_BgmVolumeBtn->Draw();
    }

    if (m_SfxVolumeBtn)
    {
        m_SfxVolumeBtn->Draw();
    }

}