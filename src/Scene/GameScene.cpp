#include "Scene/GameScene.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "GameConfig.hpp"
#include <iostream>
#include <algorithm>

GameScene::GameScene(const std::vector<UnitID>& playerDeck): m_EquippedDeck(playerDeck){
    m_CameraX = GameConfig::CAMERA_MAX_X;
    m_CameraX = GameConfig::CAMERA_MAX_X;
    // 玩家基地放在左下角 (X 為負數，Y 為負數)
    m_PlayerBase = std::make_shared<Base>(Vector2{GameConfig::PLAYER_BASE_X, GameConfig::BASE_Y}, GameConfig::BASE_HP);
    m_PlayerBase->SetImage(RESOURCE_DIR "/img/catBase.png");
    m_PlayerBase->SetSize(GameConfig::BASE_SIZE_X,GameConfig::BASE_SIZE_Y);

    // 敵人基地放在右下角 (X 為正數，Y 為負數)
    m_EnemyBase = std::make_shared<Base>(Vector2{GameConfig::ENEMY_BASE_X, GameConfig::BASE_Y}, GameConfig::BASE_HP);
    m_EnemyBase->SetImage(RESOURCE_DIR "/img/enemyBase.png");
    m_EnemyBase->SetTeam(false);
    m_EnemyBase->SetSize(GameConfig::BASE_SIZE_X,GameConfig::BASE_SIZE_Y);

    m_Entities.push_back(m_PlayerBase);
    m_Entities.push_back(m_EnemyBase);
    m_Background = std::make_shared<Background>(RESOURCE_DIR "/img/bg.png");
    m_MoneyText = std::make_shared<UIText>(
         GameConfig::MONEY_TEXT_X, GameConfig::MONEY_TEXT_Y,
         "MONEY: 0 / " + std::to_string(GameConfig::MAX_MONEY_LEVEL_1),
         30,
         Util::Color(255, 255, 0, 255)
     );

    m_WinText = std::make_shared<UIText>(0, 0, "VICTORY!", 30, SDL_Color{255, 215, 0, 255}); // 金色
    m_LoseText = std::make_shared<UIText>(0, 0, "DEFEAT!", 30, SDL_Color{255, 0, 0, 255});   // 紅色


    m_BaseNameText = std::make_shared<WorldText>(
        GameConfig::PLAYER_BASE_X,
        GameConfig::BASE_Y + GameConfig::BASE_SIZE_Y+20,
        " "
    );
    m_EnemyBaseText = std::make_shared<WorldText>(
          GameConfig::ENEMY_BASE_X,          // 使用敵方基地的 X 座標
          GameConfig::BASE_Y + GameConfig::BASE_SIZE_Y+20,       // 一樣的高度
          " "
      );
    // 產生設定按鈕
    pauseBtn = std::make_shared<Button>(
        GameConfig::PAUSE_BUTTON_RATIO_X,
        GameConfig::PAUSE_BUTTON_RATIO_Y,
        GameConfig::PAUSE_BUTTON_SIZE,
        GameConfig::PAUSE_BUTTON_SIZE,
        RESOURCE_DIR"/img/pause.png", // 你的設定圖示
        "  ", // 設定按鈕通常不用文字
        12,
        Util::Color(255, 255, 255, 255)
    );

    m_UISystem.Init(m_EquippedDeck);
    // 🚀 綁定錢包升級邏輯
    m_UISystem.SetOnWalletUpgrade([this]() {
        // 🚀 1. 判斷是否小於最大等級
        if (m_WalletLevel < GameConfig::MAX_MONEY_LEVEL && m_CurrentMoney >= m_WalletUpgradeCost) {

            m_CurrentMoney -= m_WalletUpgradeCost;
            m_WalletLevel++;

            // 🚀 2. 使用你的 MONEY_LEVEL_INCREASE 來增加上限！
            m_CurrentMaxMoney += GameConfig::MONEY_LEVEL_INCREASE;

            // 🚀 3. 判斷是否滿級
            if (m_WalletLevel == GameConfig::MAX_MONEY_LEVEL) {
                m_WalletUpgradeCost = -1; // 滿級
            } else {
                // 🚀 4. 使用你的花費增加倍率
                m_WalletUpgradeCost += GameConfig::WALLET_UPGRADE_COST_INCREASE;
            }

            std::cout << "錢包升級！目前等級：" << m_WalletLevel << "，新上限：" << m_CurrentMaxMoney << "\n";
            Util::SFX(RESOURCE_DIR "/music/upgrade.mp3").Play();
        } else {
            Util::SFX(RESOURCE_DIR "/music/fail.mp3").Play();
        }
    });
    m_PauseMenu = std::make_shared<PauseMenu>();
    // 綁定暫停選單裡面的按鈕功能
    m_PauseMenu->SetOnResume([this]() {
        m_IsPaused = false; // 恢復遊戲
    });
    m_PauseMenu->SetOnQuit([this]() {
         std::cout << "come back start scene\n"; // 你原本的 Log 保留

         // 🚀 透過我們剛剛開好的接口，向外廣播「玩家要退出了！」
         if (m_OnQuitGame) {
             m_OnQuitGame();
         }
     });

    // 📍 修改你剛才寫的 pauseBtn：
    pauseBtn->SetOnClick([this]() {
        m_IsPaused = true; // 按下暫停鈕，觸發時間停止！
    });
}
void GameScene::Update(float dt) {
    // 1. 金錢與相機邏輯 (保持不變)
    if (pauseBtn) pauseBtn->Update();
    if (m_IsPaused) {
        if (m_PauseMenu) m_PauseMenu->Update();
        return; // <--- 魔法在這裡！底下的產兵、移動、碰撞全部都不會執行！遊戲畫面瞬間凍結！
    }
    m_CurrentMoney += GameConfig::MONEY_GROWTH_SPEED * dt;
    if (m_CurrentMoney > m_CurrentMaxMoney) {
        m_CurrentMoney = m_CurrentMaxMoney;
    }

    if (Util::Input::IsKeyPressed(Util::Keycode::RIGHT) || Util::Input::IsKeyPressed(Util::Keycode::D)) m_CameraX += m_CameraSpeed * dt;
    if (Util::Input::IsKeyPressed(Util::Keycode::LEFT) || Util::Input::IsKeyPressed(Util::Keycode::A)) m_CameraX -= m_CameraSpeed * dt;
    if (m_CameraX < GameConfig::CAMERA_MIN_X) m_CameraX = GameConfig::CAMERA_MIN_X;
    if (m_CameraX > GameConfig::CAMERA_MAX_X) m_CameraX = GameConfig::CAMERA_MAX_X;
    // 🚀 3. 新增按鈕更新邏輯 (抓取滑鼠座標與左鍵狀態)



    // 2. 叫生成系統做事
    // 💡 以前幾十行的代碼現在變一行！
    m_UISystem.Update(m_EquippedDeck, m_SpawnSystem.GetCooldownTimers(), m_CurrentMoney, m_WalletUpgradeCost);
    int clickedSlot = m_UISystem.GetClickedSlot();

    m_SpawnSystem.Update(dt, m_Entities, m_CurrentMoney, m_PlayerBase, m_EnemyBase, m_EquippedDeck, clickedSlot);

    m_UISystem.ResetClick();
    // 3. 更新所有 Entity
    for (auto& entity : m_Entities) {
        entity->Update(dt);
    }

    // 4. 叫碰撞系統做事
    // 💡 以前那坨雙重迴圈變一行！
    m_CollisionSystem.Update(m_Entities);
    m_BattleSystem.Update(dt, m_Entities);
    // 5. 移除死亡單位
    RemoveDeadEntities();
    if (!m_PlayerBase->IsAlive()) {
        std::cout << "🚨 [Game Over] 玩家主堡被摧毀，你輸了！\n";
        if (m_BaseNameText) m_BaseNameText->UpdateText(std::to_string(0));
        Util::SFX(RESOURCE_DIR "/music/lose.mp3").Play();
        return; // 遊戲結束，提早 return 停止更新
    }
    else if (!m_EnemyBase->IsAlive()) {
        std::cout << "🏆 [Victory] 敵方主堡被摧毀，你贏了！\n";
        Util::SFX(RESOURCE_DIR "/music/win.mp3").Play();
        if (m_EnemyBaseText) m_EnemyBaseText->UpdateText(std::to_string(0));

        return; // 遊戲結束，提早 return 停止更新
    }
    // 6. UI 更新 (保持不變)
    if (m_MoneyText) {
        m_MoneyText->UpdateText("MONEY: " + std::to_string((int)m_CurrentMoney) + " / " + std::to_string((int)m_CurrentMaxMoney));
    }
    if (m_BaseNameText) m_BaseNameText->UpdateText(std::to_string(m_PlayerBase->GetHP()));
    if (m_EnemyBaseText) m_EnemyBaseText->UpdateText(std::to_string(m_EnemyBase->GetHP()));
}
void GameScene::RemoveDeadEntities() {
    m_Entities.erase(
        std::remove_if(m_Entities.begin(), m_Entities.end(),
                       [](const std::shared_ptr<Entity>& e) { return e->CanBeDeleted();}),
        m_Entities.end());
}

void GameScene::Draw() {
    if (m_Background) {
        m_Background->Draw(m_CameraX);
    }
    for (auto& entity : m_Entities) {
        // 把 m_CameraX 交給實體，讓它自己算該畫在哪裡
        entity->Draw(m_CameraX);

    }
    if (m_BaseNameText) m_BaseNameText->Draw(m_CameraX);
    if (m_EnemyBaseText) m_EnemyBaseText->Draw(m_CameraX);
    if (m_MoneyText) {
        m_MoneyText->Draw(); // 👈 只要這短短一行就夠了！
    }

    m_UISystem.Draw(m_EquippedDeck, m_SpawnSystem.GetCooldownTimers(), m_CurrentMoney);
    if (!m_PlayerBase->IsAlive()) {
        if (m_LoseText) m_LoseText->Draw(); // 主堡死了，每一幀都狂畫 DEFEAT
    }
    else if (!m_EnemyBase->IsAlive()) {
        if (m_WinText) m_WinText->Draw();   // 敵人死了，每一幀都狂畫 VICTORY
    }
    if (pauseBtn) {
        pauseBtn->Draw();
    }
    if (m_IsPaused) {
        if (m_PauseMenu) m_PauseMenu->Draw();
    }
}
