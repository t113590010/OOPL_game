#include "Scene/GameScene.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "GameConfig.hpp"
#include <iostream>
#include <algorithm>
#include "Entity/UnitFactory.hpp"
#include "Util/SFX.hpp"

GameScene::GameScene(
    const std::vector<UnitID>& playerDeck,
    const StageData& stage
)
    :
      m_Stage(stage),
      m_EquippedDeck(playerDeck),
      m_SpawnSystem(stage.waves){
    m_CameraX = GameConfig::CAMERA_MAX_X;

    m_PlayerBase = std::make_shared<Base>(Vector2{GameConfig::PLAYER_BASE_X, GameConfig::BASE_Y}, GameConfig::BASE_HP);
    m_PlayerBase->SetImage(RESOURCE_DIR "/img/catBase.png");
    m_PlayerBase->SetSize(GameConfig::BASE_SIZE_X,GameConfig::BASE_SIZE_Y);
    std::string boomImgcutPath = RESOURCE_DIR "/imgcut/Boom.imgcut";

    auto boomFrames = ParseImgCut(boomImgcutPath);
    if (!boomFrames.empty()) {
        m_PlayerBase->InitBoomAnimation(boomFrames);
    } else {
        std::cout << "cant find imgcut \n";
    }

    m_EnemyBase = std::make_shared<Base>(Vector2{GameConfig::ENEMY_BASE_X, GameConfig::BASE_Y}, stage.enemyBaseHP);
    m_EnemyBase->SetImage(RESOURCE_DIR "/img/enemyBase.png");
    m_EnemyBase->SetTeam(false);
    m_EnemyBase->SetSize(GameConfig::BASE_SIZE_X,GameConfig::BASE_SIZE_Y);

    m_Entities.push_back(m_PlayerBase);
    m_Entities.push_back(m_EnemyBase);
    m_Background = std::make_shared<Background>(RESOURCE_DIR "/img/bg.png");

    // ==========================================
    // 🚀 新增：用圖片數字顯示戰鬥金錢 (右上角)
    // ==========================================
    m_CurrentMoneyNumber = std::make_shared<NumberSystem>(0.63f, 0.85f, 25.0f, 35.0f, RESOURCE_DIR"/img/moneyInfo.png");
    m_MaxMoneyNumber = std::make_shared<NumberSystem>(0.85f, 0.85f, 25.0f, 35.0f, RESOURCE_DIR"/img/moneyInfo.png");
    m_CurrentMoneyNumber->SetZIndex(0);

    // ==========================================
    // 🚀 新增：載入勝利/失敗的 img004_tw.png 切圖
    // ==========================================
    // 建立勝利圖 (放在正中央)
    m_WinImage = std::make_shared<Button>(
        0.0f, 0.3f, 200.0f, 100.0f,
        RESOURCE_DIR"/img/img004_tw.png", " ", 100, Util::Color(0,0,0,0)
    );
    // 建立失敗圖 (放在正中央)
    m_LoseImage = std::make_shared<Button>(
    0.0f, 0.3f, 250.0f, 100.0f,
        RESOURCE_DIR"/img/img004_tw.png", " ", 100, Util::Color(0,0,0,0)
    );

    // 血量文字保留
    m_BaseNameText = std::make_shared<WorldText>(
        GameConfig::PLAYER_BASE_X,
        GameConfig::BASE_Y + GameConfig::BASE_SIZE_Y+20,
        " "
    );
    m_EnemyBaseText = std::make_shared<WorldText>(
          GameConfig::ENEMY_BASE_X,
          GameConfig::BASE_Y + GameConfig::BASE_SIZE_Y+20,
          " "
      );

    pauseBtn = std::make_shared<Button>(
        GameConfig::PAUSE_BUTTON_RATIO_X, GameConfig::PAUSE_BUTTON_RATIO_Y,
        GameConfig::PAUSE_BUTTON_SIZE, GameConfig::PAUSE_BUTTON_SIZE,
        RESOURCE_DIR"/img/pause.png", "  ", 12, Util::Color(255, 255, 255, 255)
    );
    pauseBtn->SetZIndex(80);

    m_UISystem.Init(m_EquippedDeck);
    m_UISystem.SetOnFireCannon([this]() {
        if (m_PlayerBase->IsCannonReady()) {
            m_PlayerBase->FireCannon();
        } else {
            Util::SFX(RESOURCE_DIR "/music/fail_summon_cat.mp3").Play();
        }
    });

    m_UISystem.SetOnWalletUpgrade([this]() {
        if (m_WalletLevel < GameConfig::MAX_MONEY_LEVEL && m_CurrentMoney >= m_WalletUpgradeCost) {
            Util::SFX(RESOURCE_DIR "/music/upgmoney.mp3").Play();
            m_CurrentMoney -= m_WalletUpgradeCost;
            m_WalletLevel++;
            m_CurrentMaxMoney += GameConfig::MONEY_LEVEL_INCREASE;
            if (m_WalletLevel == GameConfig::MAX_MONEY_LEVEL) {
                m_WalletUpgradeCost = -1;
            } else {
                m_WalletUpgradeCost += GameConfig::WALLET_UPGRADE_COST_INCREASE;
            }
            std::cout << "錢包升級！目前等級：" << m_WalletLevel << "，新上限：" << m_CurrentMaxMoney << "\n";
        } else {
            Util::SFX(RESOURCE_DIR "/music/fail_summon_cat.mp3").Play();
        }
    });

    m_PauseMenu = std::make_shared<PauseMenu>();
    m_PauseMenu->SetOnQuit([this]() {
        std::cout << "come back start scene\n";
        Util::SFX(RESOURCE_DIR "/music/clickbtn.mp3").Play();
        m_PauseMenu->SetBgZindex(51);
        m_SureMenu = std::make_shared<SureMenu>();

       m_SureMenu->SetOnConfirm([this]() {
           Util::SFX(RESOURCE_DIR "/music/clickbtn.mp3").Play();
           m_SureMenu.reset();
           if (m_OnQuitGame) m_OnQuitGame();
       });

       m_SureMenu->SetOnCancel([this]() {
           Util::SFX(RESOURCE_DIR "/music/clickbtn.mp3").Play();
           m_SureMenu.reset();
           m_PauseMenu->SetBgZindex(49);
       });
     });

    pauseBtn->SetOnClick([this]() {
        Util::SFX(RESOURCE_DIR "/music/clickbtn.mp3").Play();
        m_IsPaused = !m_IsPaused;
        m_PauseMenu->SetBgZindex(49);
        m_SureMenu.reset();
    });
}

void GameScene::Update(float dt) {
    m_StageTimer += dt;
    if (pauseBtn) pauseBtn->Update();
    if (m_IsPaused) {
        if (m_SureMenu) m_SureMenu->Update();
        else if (m_PauseMenu) m_PauseMenu->Update();
        return;
    }
    m_CurrentMoney += GameConfig::MONEY_GROWTH_SPEED * dt;
    if (m_CurrentMoney > m_CurrentMaxMoney) {
        m_CurrentMoney = m_CurrentMaxMoney;
    }

    if (Util::Input::IsKeyPressed(Util::Keycode::RIGHT) || Util::Input::IsKeyPressed(Util::Keycode::D)) m_CameraX += m_CameraSpeed * dt;
    if (Util::Input::IsKeyPressed(Util::Keycode::LEFT) || Util::Input::IsKeyPressed(Util::Keycode::A)) m_CameraX -= m_CameraSpeed * dt;
    if (m_CameraX < GameConfig::CAMERA_MIN_X) m_CameraX = GameConfig::CAMERA_MIN_X;
    if (m_CameraX > GameConfig::CAMERA_MAX_X) m_CameraX = GameConfig::CAMERA_MAX_X;

    m_UISystem.Update(m_EquippedDeck, m_SpawnSystem.GetCooldownTimers(), m_CurrentMoney, m_WalletUpgradeCost,m_PlayerBase->GetCannonProgress(),m_PlayerBase->IsCannonReady());
    int clickedSlot = m_UISystem.GetClickedSlot();

    m_SpawnSystem.Update(
        dt, m_StageTimer, m_Stage, m_Entities, m_CurrentMoney, m_PlayerBase, m_EnemyBase, m_EquippedDeck, clickedSlot
    );
    m_UISystem.ResetClick();

    for (auto& entity : m_Entities) {
        entity->Update(dt);
    }

    m_CollisionSystem.Update(m_Entities);
    m_BattleSystem.Update(dt, m_Entities);
    RemoveDeadEntities();

    if (!m_PlayerBase->IsAlive()) {
        std::cout << "🚨 [Game Over] 玩家主堡被摧毀，你輸了！\n";
        if (m_BaseNameText) m_BaseNameText->UpdateText(std::to_string(0));
        Util::SFX(RESOURCE_DIR "/music/lose.mp3").Play();
        return;
    }
    else if (!m_EnemyBase->IsAlive()) {
        std::cout << "🏆 [Victory] 敵方主堡被摧毀，你贏了！\n";
        Util::SFX(RESOURCE_DIR "/music/win.mp3").Play();
        if (m_EnemyBaseText) m_EnemyBaseText->UpdateText(std::to_string(0));
        return;
    }

    // 🚀 更新金錢數字 (只更新，不拼接字串了！)
    if (m_CurrentMoneyNumber) m_CurrentMoneyNumber->SetValue(static_cast<int>(m_CurrentMoney));
    if (m_MaxMoneyNumber) m_MaxMoneyNumber->SetValue(std::to_string(static_cast<int>(m_CurrentMoney))+'/'+std::to_string(static_cast<int>(m_CurrentMaxMoney))+'$') ;

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
    if (m_Background) m_Background->Draw(m_CameraX);

    for (auto& entity : m_Entities) {
        entity->Draw(m_CameraX);
    }

    if (m_BaseNameText) m_BaseNameText->Draw(m_CameraX);
    if (m_EnemyBaseText) m_EnemyBaseText->Draw(m_CameraX);

    // 🚀 畫出金錢數字
    // if (m_CurrentMoneyNumber) m_CurrentMoneyNumber->Draw();
    if (m_MaxMoneyNumber) m_MaxMoneyNumber->Draw();

    m_UISystem.Draw(m_EquippedDeck, m_SpawnSystem.GetCooldownTimers(), m_CurrentMoney);

    // 🚀 替換勝利與失敗的畫面，使用 img004_tw.png 切圖
    if (!m_PlayerBase->IsAlive()) {
        // 抓取 img004_tw.png 裡面 "慘敗..." 的大概座標
        if (m_WinImage) m_WinImage->DrawRect(0, 110, 309, 110);

    }
    else if (!m_EnemyBase->IsAlive()) {
        // 抓取 img004_tw.png 裡面 "大獲全勝!!" 的大概座標
        if (m_LoseImage) m_LoseImage->DrawRect(0, 0, 512, 110);

    }

    if (pauseBtn) pauseBtn->Draw();

    if (m_IsPaused) {
        if (m_PauseMenu) m_PauseMenu->Draw();
        if (m_SureMenu) m_SureMenu->Draw();
    }
}