// #include "App.hpp"
// #include "Util/Image.hpp"
// #include "Util/Input.hpp"
// #include "Util/Keycode.hpp"
// #include "Util/Logger.hpp"
// #include "Entity/UnitID.hpp"
// #include "Core/Context.hpp"
// void App::Start() {
//     LOG_TRACE("Start");
//     m_CurrentState = State::UPDATE; // 啟動後立刻告訴 main.cpp 進入 Update 迴圈
//     LOG_DEBUG("=== SDL_mixer 支援的音樂格式清單 ===");
//     int numDecoders = Mix_GetNumMusicDecoders();
//     for (int i = 0; i < numDecoders; ++i) {
//         LOG_DEBUG("支援格式 {}: {}", i, Mix_GetMusicDecoder(i));
//     }
//     LOG_DEBUG("====================================");
//     int flags = MIX_INIT_MP3; // 💡 換成 MP3
//     if ((Mix_Init(flags) & flags) != flags) {
//         LOG_ERROR("無法解鎖 MP3 格式: {}", Mix_GetError());
//     }
//     m_BattleBGM = std::make_shared<Util::BGM>(RESOURCE_DIR "/music/battle_1.mp3");
//     m_BattleBGM->SetVolume(128); // 設定音量 (0~128)
//     m_BattleBGM->Play(-1);      // -1 代表無限循環播放
//     Mix_AllocateChannels(64);
//     std::vector<UnitID> globalPlayerDeck = {
//         UnitID::CAT,
//         UnitID::LONG_LEG_CAT,
//         UnitID::NONE,
//         UnitID::NONE,
//         UnitID::NONE,
//         UnitID::NONE,
//       UnitID::AXE_CAT,
//       UnitID::CAT,
//       UnitID::CAT,
//       UnitID::CAT,
//     };
//
//
//     m_GameScene = std::make_shared<GameScene>(globalPlayerDeck);
// }
//
// void App::Update() {
//     // 0. 監聽退出按鍵 (一旦執行，main.cpp 會切到 END 並直接關閉視窗)
//     if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) || Util::Input::IfExit()) {
//         m_CurrentState = State::END;
//         return;
//     }
//     float dt = 0.016f; // 假設 60 FPS
//
//     if (m_GameScene) {
//         // ==========================================
//         // 💡 核心解耦：如果遊戲「還沒結束」，才推進時間
//         // ==========================================
//         if (!m_GameScene->IsGameOver()) {
//             m_GameScene->Update(dt); // 貓咪跟敵人繼續互毆
//         }
//         else {
//             // 🚨 遊戲結束了！
//             // 這裡不呼叫 Update(dt)，所有的貓咪跟敵人都會瞬間「定格」
//
//             // TODO: 你之後可以在這裡加上一行印出「勝利圖片」或「失敗圖片」的邏輯
//             // 例如：m_WinImage->Draw();
//             if (m_BattleBGM) {
//                 m_BattleBGM->FadeOut(1500); // 在 1.5 秒內漸弱到無聲
//                 m_BattleBGM.reset();        // 💡 清空指標！避免迴圈每一幀都瘋狂呼叫 FadeOut
//             }
//             // 如果玩家按下 Enter，就重新開始遊戲 (再 new 一個新的 GameScene)
//             if (Util::Input::IsKeyUp(Util::Keycode::RETURN)) {
//                 Start(); // 重新觸發 Start，洗掉舊的場景
//             }
//         }
//
//         // ==========================================
//         // 💡 不管遊戲結不結束，都要繼續畫圖！
//         // 這樣就算定格了，玩家還是看得到死掉的主堡跟滿地的貓咪
//         // ==========================================
//         m_GameScene->Draw();
//     }
// }
//
// void App::End() {
//     LOG_TRACE("End");
//     // 程式即將關閉，不用特別做什麼
//     if (m_BattleBGM) {
//         m_BattleBGM->Pause();
//     }
// }
#include "App.hpp"
#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include "Entity/UnitID.hpp"
#include "Core/Context.hpp"
#include <SDL_mixer.h>

void App::Start() {
    LOG_TRACE("Start");

    LOG_DEBUG("=== SDL_mixer 支援的音樂格式清單 ===");
    int numDecoders = Mix_GetNumMusicDecoders();
    for (int i = 0; i < numDecoders; ++i) {
        LOG_DEBUG("支援格式 {}: {}", i, Mix_GetMusicDecoder(i));
    }
    LOG_DEBUG("====================================");

    int flags = MIX_INIT_MP3;
    if ((Mix_Init(flags) & flags) != flags) {
        LOG_ERROR("無法解鎖 MP3 格式: {}", Mix_GetError());
    }
    Mix_AllocateChannels(64);
    LoadStartScene();
    // StartHomeScene();
}

void App::LoadStartScene() {
    m_CurrentState = State::TITLE;
    m_MenuBGM = std::make_shared<Util::BGM>(RESOURCE_DIR "/music/startbgm.mp3");
    m_MenuBGM->SetVolume(100);
    m_MenuBGM->Play(-1);
    m_StartScene = std::make_shared<StartScene>();

    // 綁定按鈕：當玩家在 StartScene 按下 "START GAME" 時要做什麼？
    m_StartScene->SetOnStartGame([this]() {
        Util::SFX(RESOURCE_DIR "/music/clickbtn.mp3").Play();

        // 💡 目前我們先接去 HomeScene (你原本的畫面)
        // 等我們寫好 LevelSelectScene 後，把這裡改成切換到選關畫面就好！
        StartHomeScene();
    });
}

void App::StartHomeScene() {
    m_CurrentState = State::HOME;

    if (m_BattleBGM) {
        m_BattleBGM->FadeOut(1000);
        m_BattleBGM.reset();
    }
    if (m_MenuBGM) {
        m_MenuBGM->FadeOut(500); // 快速淡出舊的
    }
    m_MenuBGM = std::make_shared<Util::BGM>(RESOURCE_DIR "/music/homebgm.mp3");
    m_MenuBGM->SetVolume(100);
    m_MenuBGM->Play(-1);
    // 🚨 絕對不能在這裡 reset GameScene！交給 Update 去做！

    m_HomeScene = std::make_shared<HomeScene>();
    m_HomeScene->SetOnStartBtnClick([this]() {
        if (m_MenuBGM) {
              m_MenuBGM->FadeOut(500); // 快速淡出舊的
          }
        Util::SFX(RESOURCE_DIR "/music/clickbtn.mp3").Play();

        Util::SFX(RESOURCE_DIR "/music/StartBattle.mp3").Play();
        SDL_Delay(4000);
        StartBattleScene();
    });
    m_HomeScene->SetOnUpgradeBtnClick([this]() {
        LOG_DEBUG("點擊了升級按鈕！");
        // TODO: 之後在這裡呼叫 StartUpgradeScene();
        // 你可以先播個簡單的按鈕音效
        Util::SFX(RESOURCE_DIR "/music/what-da-dog-doin.mp3").Play();
    });

    // ==========================================
    // 3. 隊伍編成按鈕 (新增)
    // ==========================================
    m_HomeScene->SetOnTeamBtnClick([this]() {
        LOG_DEBUG("點擊了隊伍按鈕！");
        // TODO: 之後在這裡呼叫 StartTeamScene();
        Util::SFX(RESOURCE_DIR "/music/what-da-dog-doin.mp3").Play();
    });
}

void App::StartBattleScene() {
    m_CurrentState = State::BATTLE;

    m_BattleBGM = std::make_shared<Util::BGM>(RESOURCE_DIR "/music/battle_1.mp3");
    m_BattleBGM->SetVolume(128);
    m_BattleBGM->Play(-1);

    // 🚨 絕對不能在這裡 reset HomeScene！交給 Update 去做！

    std::vector<UnitID> globalPlayerDeck = {
        UnitID::CAT, UnitID::LONG_LEG_CAT, UnitID::AXE_CAT, UnitID::CAT, UnitID::AXE_CAT,
        UnitID::NONE, UnitID::NONE, UnitID::NONE, UnitID::NONE, UnitID::NONE,
    };

    m_GameScene = std::make_shared<GameScene>(globalPlayerDeck);
    m_GameScene->SetOnQuitGame([this]() {
        m_PendingQuit = true;
    });
}

void App::Update() {
    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) || Util::Input::IfExit()) {
        m_CurrentState = State::END;
        return;
    }
    if (m_PendingQuit) {
        m_PendingQuit = false;
        StartHomeScene(); // 這裡才真正執行 reset 和 new
        return;           // 這幀直接結束，下一幀再開始跑 HOME 的邏輯
    }

    float dt = 0.016f;
    if (m_CurrentState == State::TITLE) {
        // 如果有舊的場景，把它清掉
        if (m_HomeScene) m_HomeScene.reset();
        if (m_GameScene) m_GameScene.reset();

        if (m_StartScene) {
            m_StartScene->Update();
            m_StartScene->Draw();
        }
    }else if (m_CurrentState == State::HOME) {
        // 🚀 回到主迴圈安全的地方了，把前一個戰鬥畫面殺掉
        if (m_StartScene) m_StartScene.reset();
        if (m_GameScene) {
            m_GameScene.reset();
        }

        if (m_HomeScene) {
            m_HomeScene->Update();
            m_HomeScene->Draw();
        }
    }
    else if (m_CurrentState == State::BATTLE) {
        // 🚀 回到主迴圈安全的地方了，把前一個主畫面殺掉
        if (m_HomeScene) {
            m_HomeScene.reset();
        }
        if (m_StartScene) m_StartScene.reset();

        if (m_GameScene) {
            if (!m_GameScene->IsGameOver()) {
                m_GameScene->Update(dt);
            }
            else {
                if (m_BattleBGM) {
                    m_BattleBGM->FadeOut(1500);
                    m_BattleBGM.reset();
                }
                if (Util::Input::IsKeyUp(Util::Keycode::RETURN)) {
                    Util::SFX(RESOURCE_DIR "/music/clickbtn.mp3").Play();

                    StartHomeScene();
                }
            }
            m_GameScene->Draw();
        }
    }
}

void App::End() {
    LOG_TRACE("End");
    if (m_BattleBGM) {
        m_BattleBGM->Pause();
    }
}