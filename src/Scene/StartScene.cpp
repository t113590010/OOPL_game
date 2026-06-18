#include "StartScene.hpp"
#include "GameConfig.hpp"
#include "Util/SFX.hpp"
#include "Util/Logger.hpp"
#include "PlayerData.hpp"
#include "DebugCheat.hpp"
#include <utility>
#include "StartDebugMenu.hpp"

StartScene::StartScene()
{
    // ==========================================
    // 1. 設定背景圖
    // ==========================================

    auto bgImage =
        std::make_shared<Util::Image>(
            RESOURCE_DIR "/img/StartScene.png"
        );

    m_Background.SetDrawable(bgImage);
    m_Background.SetZIndex(10);

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

    glm::vec2 imgSize =
        bgImage->GetSize();

    m_Background.m_Transform.scale =
        glm::vec2(
            windowWidth / imgSize.x,
            windowHeight / imgSize.y
        );

    // ==========================================
    // 2. 設定開始按鈕
    // ==========================================

    m_StartBtn =
        std::make_shared<Button>(
            0.0f,
            -0.215f,
            220.0f,
            70.0f,
            RESOURCE_DIR "/img/battleStart.png",
            " ",
            40,
            Util::Color(255, 255, 255, 255)
        );

    m_StartBtn->SetZIndex(0);

    m_StartBtn->SetOnClick(
        [this]()
        {
            if (m_OnStartGame)
            {
                m_OnStartGame();
            }
        }
    );

    // ==========================================
    // 3. 選項透明點擊區
    // ==========================================
    // 注意：
    // 這個位置是你自己調好的，不要改。
    // 這顆 Button 不需要 Draw，只負責偵測點擊。

    m_OptionHitBtn =
        std::make_shared<Button>(
            0.0f,
            -0.52f,
            340.0f,
            80.0f,
            RESOURCE_DIR "/img/black.png",
            " ",
            30,
            Util::Color(0, 0, 0, 0)
        );

    m_OptionHitBtn->SetZIndex(21);

    m_OptionHitBtn->SetOnClick(
        [this]()
        {
            Util::SFX(
                RESOURCE_DIR "/music/clickbtn.mp3"
            ).Play();

            m_OptionMenu =
                std::make_shared<StartOptionMenu>();

            m_OptionMenu->SetOnClose(
                [this]()
                {
                    m_OptionMenu.reset();
                }
            );

            m_OptionMenu->SetOnHelp(
    [this]()
    {
        Util::SFX(
            RESOURCE_DIR "/music/clickbtn.mp3"
        ).Play();

        m_StartDebugMenu =
            std::make_shared<StartDebugMenu>();

        m_StartDebugMenu->SetOnBack(
            [this]()
            {
                m_ShouldCloseStartDebugMenu = true;
            }
        );
    }
);
            m_OptionMenu->SetOnBgmVolumeChanged(
                [this](int level)
                {
                    if (m_OnBgmVolumeChanged)
                    {
                        m_OnBgmVolumeChanged(level);
                    }
                }
            );

            m_OptionMenu->SetOnSfxVolumeChanged(
                [this](int level)
                {
                    if (m_OnSfxVolumeChanged)
                    {
                        m_OnSfxVolumeChanged(level);
                    }
                }
            );
        }
    );
}

void StartScene::SetOnStartGame(
    std::function<void()> callback
)
{
    m_OnStartGame =
        std::move(callback);
}

void StartScene::SetOnBgmVolumeChanged(
    std::function<void(int)> callback
)
{
    m_OnBgmVolumeChanged =
        std::move(callback);
}

void StartScene::SetOnSfxVolumeChanged(
    std::function<void(int)> callback
)
{
    m_OnSfxVolumeChanged =
        std::move(callback);
}
void StartScene::Update()
{
    if (m_StartDebugMenu)
    {
        m_StartDebugMenu->Update();

        if (m_ShouldCloseStartDebugMenu)
        {
            m_StartDebugMenu.reset();

            m_ShouldCloseStartDebugMenu =
                false;
        }

        return;
    }

    if (m_OptionMenu)
    {
        m_OptionMenu->Update();
        return;
    }

    if (m_OptionHitBtn)
    {
        m_OptionHitBtn->Update();
    }

    if (m_StartBtn)
    {
        m_StartBtn->Update();
    }
}
void StartScene::Draw()
{
    m_Background.Draw();

    if (m_StartBtn)
    {
        m_StartBtn->Draw();
    }

    if (m_StartDebugMenu)
    {
        if (m_OptionMenu)
        {
            m_OptionMenu->DrawBackgroundOnly();
        }

        m_StartDebugMenu->Draw();
    }
    else if (m_OptionMenu)
    {
        m_OptionMenu->Draw();
    }
}