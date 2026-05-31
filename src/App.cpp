
#include "App.hpp"
#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include "Entity/UnitID.hpp"
#include "Core/Context.hpp"
#include <SDL_mixer.h>
#include "PlayerData.hpp"
#include <iostream>
namespace {
    int stageIdx = -1;
}
void App::Start() {
    LOG_TRACE("Start");
    PlayerData::GetInstance()->LoadFromFile();
    SDL_StopTextInput();


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
    LOG_DEBUG(
    "Unlocked Stage = {}",
    PlayerData::GetInstance()
        ->GetMaxUnlockedStage()
);
}

void App::LoadStartScene() {
    m_CurrentState = State::TITLE;
    m_MenuBGM = std::make_shared<Util::BGM>(RESOURCE_DIR "/music/startbgm.mp3");

    // m_MenuBGM = std::make_shared<Util::BGM>(RESOURCE_DIR "/music/【貓咪大戰爭】6週年電視廣告 - 貓咪大戰爭中文版官方YouTube (youtube).mp3");

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
    if (!m_HomeScene) {
        m_HomeScene = std::make_shared<HomeScene>();
        
        // 1. 開始遊戲按鈕 (整合了 GUI 的選關邏輯)
        m_HomeScene->SetOnStartBtnClick([this]() {
            Util::SFX(RESOURCE_DIR "/music/clickbtn.mp3").Play();
            stageIdx = -1;
            StartLevelSelectScene();
        });

        // 2. 升級按鈕
        m_HomeScene->SetOnUpgradeBtnClick([this]() {
            LOG_DEBUG("點擊了升級按鈕！");
            // TODO: 之後在這裡呼叫 StartUpgradeScene();
            Util::SFX(RESOURCE_DIR "/music/clickbtn.mp3").Play();

            StartLevelUpgradeScene();
        });

        // ==========================================
        // 3. 隊伍編成按鈕 (保留你的新增)
        // ==========================================
        m_HomeScene->SetOnTeamBtnClick([this]() {
            LOG_DEBUG("點擊了隊伍按鈕！");
            // TODO: 之後在這裡呼叫 StartTeamScene();
            Util::SFX(RESOURCE_DIR "/music/clickbtn.mp3").Play();

            StartDeckScene();
        });

        // 4. 倉庫按鈕
        m_HomeScene->SetOnStorageBtnClick([this]() {
            Util::SFX(RESOURCE_DIR "/music/clickbtn.mp3").Play();
            LOG_DEBUG("ready to click STORAGE！");
            StartStorageScene();
        });

        // 5. 稀有轉蛋按鈕
        m_HomeScene->SetOnRareGachaBtnClick([this]() {
            Util::SFX(RESOURCE_DIR "/music/clickbtn.mp3").Play();
            LOG_DEBUG("ready to click StartRareGachaScene！");
            StartRareGachaScene();
        });

        // 6. 一般轉蛋按鈕
        m_HomeScene->SetOnNormalGachaBtnClick([this]() {
            Util::SFX(RESOURCE_DIR "/music/clickbtn.mp3").Play();
            LOG_DEBUG("ready to click StartNormalGachaScene！");
            StartNormalGachaScene();
        });
        // 7. 返回按鈕
        m_HomeScene->SetOnReturnBtnClick(
    [this]()
    {
        Util::SFX(
            RESOURCE_DIR "/music/clickbtn.mp3"
        ).Play();

        LoadStartScene();
    }
);
    }
}

void App::StartStorageScene() {
    // LOG_DEBUG("click STORAGE！");

    m_CurrentState = State::STORAGE;
    if (!m_StorageScene) {
        m_StorageScene = std::make_shared<StorageScene>();

        m_StorageScene->SetOnReturnBtnClick([this]() {
            Util::SFX(RESOURCE_DIR "/music/clickbtn.mp3").Play();
            // 這裡只切換狀態，絕對不要呼叫 StartHomeScene()！
            m_CurrentState = State::HOME;
        });
        m_StorageScene->SetOnToUse([this]() {
            LOG_DEBUG("點擊了：使用貓咪！");
            Util::SFX(RESOURCE_DIR "/music/clickbtn.mp3").Play();
            // 這裡只切換狀態，絕對不要呼叫 StartHomeScene()！
            // m_CurrentState = State::HOME;
        });

        m_StorageScene->SetOnToXp([this]() {
            LOG_DEBUG("點擊了：兌換 XP！");
            Util::SFX(RESOURCE_DIR "/music/clickbtn.mp3").Play();
              // 這裡只切換狀態，絕對不要呼叫 StartHomeScene()！
              // m_CurrentState = State::HOME;
        });
    }
}
void App::StartDeckScene() {
    LOG_DEBUG("click LEVEL UPGRADE！");

    m_CurrentState = State::DECK;
    if (!m_DeckScene) {
        m_DeckScene = std::make_shared<DeckScene>();

        m_DeckScene->SetOnReturnBtnClick([this]() {
            Util::SFX(RESOURCE_DIR "/music/clickbtn.mp3").Play();
            // 這裡只切換狀態，絕對不要呼叫 StartHomeScene()！
            m_CurrentState = State::HOME;
        });

        m_DeckScene->SetOnUpgClick([this]() {
            Util::SFX(RESOURCE_DIR "/music/clickbtn.mp3").Play();
            // 這裡只切換狀態，絕對不要呼叫 StartHomeScene()！
            StartLevelUpgradeScene();
        });
    }
    if (m_DeckScene) {
        m_DeckScene->Refresh();
    }

}
void App::StartLevelSelectScene()
{
    m_CurrentState = State::LEVEL_SELECT;

    if (!m_LevelSelectScene)
    {
        m_LevelSelectScene =
            std::make_shared<LevelSelectScene>();

        m_LevelSelectScene
            ->SetOnReturnBtnClick(
            [this]()
            {
                m_CurrentState = State::HOME;
            }
        );

        m_LevelSelectScene
            ->SetOnStageSelected(
            [this](int stageId)
            {
                if (m_MenuBGM)
                {
                    m_MenuBGM->FadeOut(500);
                    m_MenuBGM.reset();
                }

                Util::SFX(
                    RESOURCE_DIR "/music/StartBattle.mp3"
                ).Play();

                SDL_Delay(4000);

                StartBattleScene(stageId);
            }
        );
    }

}
void App::StartRareGachaScene() {
    m_CurrentState = State::RARE_GACHA;
    if (!m_RareGachaScene) {
        m_RareGachaScene = std::make_shared<RareGachaScene>();

        m_RareGachaScene->SetOnReturnBtnClick([this]() {
            Util::SFX(RESOURCE_DIR "/music/clickbtn.mp3").Play();
            m_CurrentState = State::HOME;
        });

        m_RareGachaScene->SetOnStorageBtnClick([this]() {
          // 音效可以保留，但切換場景要交給專屬函數！
          StartStorageScene(); // 👈 改成這樣！
      });

        m_RareGachaScene->SetOnspanClick([this]() {
          Util::SFX(RESOURCE_DIR "/music/clickbtn.mp3").Play();
          LOG_DEBUG("執行了 單抽 1 次！");
          // TODO: 未來這裡要呼叫抽卡動畫的邏輯
           // StartStorageScene(); // 👈 改成這樣！

      });
        // 💰 綁定十連抽事件
        m_RareGachaScene->SetOn_muti_spanClick([this]() {
            Util::SFX(RESOURCE_DIR "/music/clickbtn.mp3").Play();
            LOG_DEBUG("執行了 十連抽！");
            // TODO: 未來這裡要呼叫抽卡動畫的邏輯
             // StartStorageScene(); // 👈 改成這樣！

        });
    }
}


void App::StartNormalGachaScene() {
    m_CurrentState = State::NORMAL_GACHA;
    if (!m_NormalGachaScene) {
        m_NormalGachaScene = std::make_shared<NormalGachaScene>();

        m_NormalGachaScene->SetOnReturnBtnClick([this]() {
            Util::SFX(RESOURCE_DIR "/music/clickbtn.mp3").Play();
            m_CurrentState = State::HOME;
        });

        m_NormalGachaScene->SetOnStorageBtnClick([this]() {
             // 音效可以保留，但切換場景要交給專屬函數！
             StartStorageScene(); // 👈 改成這樣！
         });

        m_NormalGachaScene->SetOnspanClick([this]() {
            Util::SFX(RESOURCE_DIR "/music/clickbtn.mp3").Play();
            LOG_DEBUG("執行了 單抽 1 次！");
            // TODO: 未來這裡要呼叫抽卡動畫的邏輯
             // StartStorageScene(); // 👈 改成這樣！

        });

        // 💰 綁定十連抽事件
        m_NormalGachaScene->SetOn_muti_spanClick([this]() {
            Util::SFX(RESOURCE_DIR "/music/clickbtn.mp3").Play();
            LOG_DEBUG("執行了 十連抽！");
            // TODO: 未來這裡要呼叫抽卡動畫的邏輯
             // StartStorageScene(); // 👈 改成這樣！

        });
    }
}

void App::StartLevelUpgradeScene() {
    LOG_DEBUG("click LEVEL UPGRADE！");

    m_CurrentState = State::LEVEL_UPGRADE;
    if (!m_LevelUpgradeScene) {
        m_LevelUpgradeScene = std::make_shared<LevelUpgradeScene>();

        m_LevelUpgradeScene->SetOnReturnBtnClick([this]() {
            Util::SFX(RESOURCE_DIR "/music/clickbtn.mp3").Play();
            // 這裡只切換狀態，絕對不要呼叫 StartHomeScene()！
            m_CurrentState = State::HOME;
        });
    }
}
void App::StartBattleScene(int stageIdx){
    m_CurrentStageID =
    STAGES[stageIdx].stageID;

    m_CurrentState = State::BATTLE;

    m_BattleBGM = std::make_shared<Util::BGM>(RESOURCE_DIR "/music/battle_1.mp3");
    m_BattleBGM->SetVolume(128);
    m_BattleBGM->Play(-1);

    // 🚨 絕對不能在這裡 reset HomeScene！交給 Update 去做！

    // std::vector<UnitID> globalPlayerDeck = {
    //     UnitID::Peashooter, UnitID::LONG_LEG_CAT, UnitID::AXE_CAT, UnitID::CowCat, UnitID::Queen,
    //     UnitID::FlyCat, UnitID::FishCat, UnitID::DinoCat, UnitID::GaintCat, UnitID::DogDoin,
    // };
    std::vector<UnitID> realPlayerDeck = PlayerData::GetInstance()->GetDeck();
    m_GameScene = std::make_shared<GameScene>(
        realPlayerDeck,
        STAGES[stageIdx]
    );
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

    static Uint64 lastTime = SDL_GetPerformanceCounter();

    Uint64 currentTime = SDL_GetPerformanceCounter();

    float dt = static_cast<float>(
        (currentTime - lastTime) /
        static_cast<double>(SDL_GetPerformanceFrequency())
    );

    lastTime = currentTime;
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
        // if (m_StorageScene) m_StorageScene.reset();        // 👈 補上
        // if (m_RareGachaScene) m_RareGachaScene.reset();    // 👈 補上
        // if (m_NormalGachaScene) m_NormalGachaScene.reset();// 👈 補上
        // if (m_LevelUpgradeScene) m_LevelUpgradeScene.reset(); // 👈 補上
        // if (m_DeckScene) m_DeckScene.reset();              // 👈 補上
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

                static bool rewardGiven = false;

                if (!rewardGiven)
                {
                    if (m_GameScene->IsPlayerWin())
                    {
                        PlayerData::GetInstance()
                            ->ClearStage(
                                m_CurrentStageID
                            );

                        PlayerData::GetInstance()
                            ->SaveToFile();

                        LOG_DEBUG(
                            "Stage {} cleared!",
                            m_CurrentStageID
                        );

                        LOG_DEBUG(
                            "Unlocked Stage = {}",
                            PlayerData::GetInstance()
                                ->GetMaxUnlockedStage()
                        );
                    }

                    rewardGiven = true;
                }

                if (m_BattleBGM)
                {
                    m_BattleBGM->FadeOut(1500);
                    m_BattleBGM.reset();
                }

                if (Util::Input::IsKeyUp(
                    Util::Keycode::RETURN))
                {
                    rewardGiven = false;

                    Util::SFX(
                        RESOURCE_DIR "/music/clickbtn.mp3"
                    ).Play();

                    StartHomeScene();
                }
            }
            m_GameScene->Draw();
        }
    }else if (m_CurrentState == State::STORAGE) {

        if (m_StartScene) m_StartScene.reset();
        // LOG_DEBUG("STORAGE！");

        if (m_StorageScene) {
            m_StorageScene->Update();
            m_StorageScene->Draw();
        }
    }
    else if (m_CurrentState == State::RARE_GACHA) {

        if (m_StartScene) m_StartScene.reset();
        // LOG_DEBUG("m_RareGachaScene！");


        if (m_RareGachaScene) {
            m_RareGachaScene->Update();
            m_RareGachaScene->Draw();
        }
    }
    else if (m_CurrentState == State::NORMAL_GACHA) {

        if (m_StartScene) m_StartScene.reset();

        // LOG_DEBUG("m_NormalGachaScene！");

        if (m_NormalGachaScene) {
            m_NormalGachaScene->Update();
            m_NormalGachaScene->Draw();
        }
    }
    else if (m_CurrentState == State::LEVEL_UPGRADE) {

        if (m_StartScene) m_StartScene.reset();
        // LOG_DEBUG("m_LevelUpgradeScene！");


        if (m_LevelUpgradeScene) {
            m_LevelUpgradeScene->Update();
            m_LevelUpgradeScene->Draw();
        }
    }
    else if (m_CurrentState == State::DECK) {

        if (m_StartScene) m_StartScene.reset();
        // LOG_DEBUG("m_DeckScene！");


        if (m_DeckScene) {
            m_DeckScene->Update();
            m_DeckScene->Draw();
        }
    } else if (
    m_CurrentState ==
    State::LEVEL_SELECT
)
    {
        if (m_StartScene)
            m_StartScene.reset();

        if (m_LevelSelectScene)
        {
            m_LevelSelectScene->Update();
            m_LevelSelectScene->Draw();
        }
    }
}

void App::End() {
    // LOG_TRACE("End");
    PlayerData::GetInstance()->SaveToFile();
    if (m_BattleBGM) {
        m_BattleBGM->Pause();
    }
}