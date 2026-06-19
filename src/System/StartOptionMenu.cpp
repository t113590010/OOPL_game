#include "System/StartOptionMenu.hpp"

#include "Core/Context.hpp"
#include "GameConfig.hpp"
#include "PlayerData.hpp"
#include "Util/Color.hpp"
#include "Util/Image.hpp"
#include "Util/SFX.hpp"
#include "Util/Logger.hpp"

#include <string>

StartOptionMenu::StartOptionMenu()
{
    // =========================
    // 選項面板背景
    // =========================

    m_MenuBg =
        std::make_shared<Button>(
            0.0f,
            0.0f,
            620.0f,
            430.0f,
            RESOURCE_DIR "/img/optionMenuBg.png",
            " ",
            30,
            Util::Color(255, 255, 255, 255)
        );

    m_MenuBg->SetZIndex(50);
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
    // 讀取玩家音量設定
    // =========================

    auto playerData =
        PlayerData::GetInstance();

    m_BgmVolumeLevel =
        playerData->GetBgmVolumeLevel();

    m_SfxVolumeLevel =
        playerData->GetSfxVolumeLevel();

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
            Util::SFX(
                RESOURCE_DIR "/music/clickbtn.mp3"
            ).Play();

            if (m_OnClose)
            {
                m_OnClose();
            }
        }
    );

    // =========================
    // BGM 音量按鈕
    // =========================

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

            Util::SFX(
                RESOURCE_DIR "/music/clickbtn.mp3"
            ).Play();

            LOG_DEBUG(
                "StartOptionMenu: BGM Volume Level = {}",
                m_BgmVolumeLevel
            );
        }
    );

    // =========================
    // SFX 音量按鈕
    // =========================

    m_SfxVolumeBtn =
        std::make_shared<Button>(
            0.16f,
            -0.10f,
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

            int volume =
                VolumeLevelToMixerVolume(
                    m_SfxVolumeLevel
                );

            Util::SFX::SetGlobalVolume(
                volume
            );

            if (m_OnSfxVolumeChanged)
            {
                m_OnSfxVolumeChanged(
                    m_SfxVolumeLevel
                );
            }

            Util::SFX(
                RESOURCE_DIR "/music/clickbtn.mp3"
            ).Play();

            LOG_DEBUG(
                "StartOptionMenu: SFX Volume Level = {}, Volume = {}",
                m_SfxVolumeLevel,
                volume
            );
        }
    );
    // =========================
    // 幫助按鈕
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
            Util::SFX(
                RESOURCE_DIR "/music/clickbtn.mp3"
            ).Play();

            if (m_OnHelp)
            {
                m_OnHelp();
            }
        }
    );
}

void StartOptionMenu::Update()
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
}

void StartOptionMenu::Draw()
{
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
void StartOptionMenu::SetOnHelp(
    std::function<void()> callback
)
{
    m_OnHelp =
        std::move(callback);
}

void StartOptionMenu::SetOnClose(
    std::function<void()> callback
)
{
    m_OnClose =
        std::move(callback);
}

void StartOptionMenu::SetOnBgmVolumeChanged(
    std::function<void(int)> callback
)
{
    m_OnBgmVolumeChanged =
        std::move(callback);
}

void StartOptionMenu::SetOnSfxVolumeChanged(
    std::function<void(int)> callback
)
{
    m_OnSfxVolumeChanged =
        std::move(callback);
}

std::string StartOptionMenu::GetBgmVolumeImagePath(
    int level
) const
{
    return std::string(RESOURCE_DIR)
        + "/img/bgmVolume"
        + std::to_string(level)
        + ".png";
}
void StartOptionMenu::DrawBackgroundOnly()
{
    m_Background.DrawRect(
        0.0f,
        0.0f,
        10.0f,
        10.0f
    );
}
std::string StartOptionMenu::GetSfxVolumeImagePath(
    int level
) const
{
    return std::string(RESOURCE_DIR)
        + "/img/sfxVolume"
        + std::to_string(level)
        + ".png";
}

int StartOptionMenu::VolumeLevelToMixerVolume(
    int level
) const
{
    if (level <= 0)
    {
        return 0;
    }

    if (level == 1)
    {
        return 40;
    }

    if (level == 2)
    {
        return 80;
    }

    return 128;
}