#include "App.hpp"
#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include "Entity/UnitID.hpp"
#include "Core/Context.hpp"
void App::Start() {
    LOG_TRACE("Start");
    m_CurrentState = State::UPDATE; // 啟動後立刻告訴 main.cpp 進入 Update 迴圈
    LOG_DEBUG("=== SDL_mixer 支援的音樂格式清單 ===");
    int numDecoders = Mix_GetNumMusicDecoders();
    for (int i = 0; i < numDecoders; ++i) {
        LOG_DEBUG("支援格式 {}: {}", i, Mix_GetMusicDecoder(i));
    }
    LOG_DEBUG("====================================");
    int flags = MIX_INIT_MP3; // 💡 換成 MP3
    if ((Mix_Init(flags) & flags) != flags) {
        LOG_ERROR("無法解鎖 MP3 格式: {}", Mix_GetError());
    }
    m_BattleBGM = std::make_shared<Util::BGM>(RESOURCE_DIR "/music/battle_1.mp3");
    m_BattleBGM->SetVolume(128); // 設定音量 (0~128)
    m_BattleBGM->Play(-1);      // -1 代表無限循環播放
    Mix_AllocateChannels(64);
    std::vector<UnitID> globalPlayerDeck = {
        UnitID::CAT,
        UnitID::LONG_LEG_CAT,
        UnitID::NONE,
        UnitID::NONE,
        UnitID::NONE,
        UnitID::GAY,
      UnitID::AXE_CAT,
      UnitID::CAT,
      UnitID::CAT,
      UnitID::CAT,
    };

    m_GameScene = std::make_shared<GameScene>(globalPlayerDeck);
}

void App::Update() {
    // 0. 監聽退出按鍵 (一旦執行，main.cpp 會切到 END 並直接關閉視窗)
    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) || Util::Input::IfExit()) {
        m_CurrentState = State::END;
        return; 
    }
    float dt = 0.016f; // 假設 60 FPS

    if (m_GameScene) {
        // ==========================================
        // 💡 核心解耦：如果遊戲「還沒結束」，才推進時間
        // ==========================================
        if (!m_GameScene->IsGameOver()) {
            m_GameScene->Update(dt); // 貓咪跟敵人繼續互毆
        } 
        else {
            // 🚨 遊戲結束了！
            // 這裡不呼叫 Update(dt)，所有的貓咪跟敵人都會瞬間「定格」
            
            // TODO: 你之後可以在這裡加上一行印出「勝利圖片」或「失敗圖片」的邏輯
            // 例如：m_WinImage->Draw();
            if (m_BattleBGM) {
                m_BattleBGM->FadeOut(1500); // 在 1.5 秒內漸弱到無聲
                m_BattleBGM.reset();        // 💡 清空指標！避免迴圈每一幀都瘋狂呼叫 FadeOut
            }
            // 如果玩家按下 Enter，就重新開始遊戲 (再 new 一個新的 GameScene)
            if (Util::Input::IsKeyUp(Util::Keycode::RETURN)) {
                Start(); // 重新觸發 Start，洗掉舊的場景
            }
        }

        // ==========================================
        // 💡 不管遊戲結不結束，都要繼續畫圖！
        // 這樣就算定格了，玩家還是看得到死掉的主堡跟滿地的貓咪
        // ==========================================
        m_GameScene->Draw();
    }
}

void App::End() { 
    LOG_TRACE("End");
    // 程式即將關閉，不用特別做什麼
    if (m_BattleBGM) {
        m_BattleBGM->Pause();
    }
}