#include "Scene/GameScene.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "GameConfig.hpp"
#include <iostream>
#include <algorithm>
#include "Entity/UnitFactory.hpp"
#include "Util/SFX.hpp"
#include "PlayerData.hpp"
#include "PauseMenu.hpp"
#include "System/DebugCheat.hpp"
#include <cstdlib>
namespace StageReward
{
    const int CAT_FOOD_AMOUNT = 30;
    const int NORMAL_TICKET_AMOUNT = 1;
    const int RARE_TICKET_AMOUNT = 1;

    const int CAT_FOOD_DROP_RATE = 50;
    const int NORMAL_TICKET_DROP_RATE = 25;
    const int RARE_TICKET_DROP_RATE = 15;
}

static bool RollPercent(
    int percent
)
{
    return std::rand() % 100 < percent;
}

GameScene::GameScene(
    const std::vector<UnitID>& playerDeck,
    const StageData& stage
)
    :
      m_Stage(stage),
      m_EquippedDeck(playerDeck),
      m_SpawnSystem(stage.waves){
    m_CurrentStageID = stage.stageID;
    std::string stageName =
    "Stage " + std::to_string(stage.stageID);

    auto displayIt =
        STAGE_DISPLAY_DATA.find(stage.stageID);

    if (displayIt != STAGE_DISPLAY_DATA.end())
    {
        stageName =
            displayIt->second.stageName;
    }
    m_RewardNotifyBar =
        std::make_shared<Button>(
            0.0f,
            -0.28f,
            700.0f,
            190.0f,
            RESOURCE_DIR "/img/notifBar.png",
            " ",
            28,
            Util::Color(255, 255, 255, 255)
        );

    m_RewardNotifyBar->SetZIndex(85);
    // 黑色描邊底層
    m_StageNameTextShadow =
        std::make_shared<UIText>(
            -0.805f,
            0.885f,
            stageName,
            40,
            Util::Color(0, 0, 0, 255)
        );

    m_StageNameTextShadow->SetZIndex(89);

    // 黃色文字上層
    m_StageNameText =
        std::make_shared<UIText>(
            -0.815f,
            0.895f,
            stageName,
            40,
            Util::Color(255, 230, 0, 255)
        );

    m_StageNameText->SetZIndex(90);
    m_CameraX = GameConfig::CAMERA_MAX_X;
    auto context = Core::Context::GetInstance();
    float windowWidth = (float)context->GetWindowWidth();
    float windowHeight = (float)context->GetWindowHeight();
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
    m_WinImage =
        std::make_shared<Button>(
            0.0f,
            0.30f,
            430.0f,
            92.0f,
            RESOURCE_DIR "/img/victoryFont.png",
            " ",
            100,
            Util::Color(255, 255, 255, 255)
        );

    m_WinImage->SetZIndex(100);
    // 建立失敗圖 (放在正中央)
    m_LoseImage = std::make_shared<Button>(
    0.0f, 0.3f, 250.0f, 100.0f,
        RESOURCE_DIR"/img/img004_tw.png", " ", 100, Util::Color(0,0,0,0)
    );

    // 血量文字保留
    // m_BaseNameText = std::make_shared<NumberSystem>(
    //     GameConfig::PLAYER_BASE_X,
    //     GameConfig::BASE_Y + GameConfig::BASE_SIZE_Y+20,
    //     " "
    // );
    // m_EnemyBaseText = std::make_shared<NumberSystem>(
    //       GameConfig::ENEMY_BASE_X,
    //       GameConfig::BASE_Y + GameConfig::BASE_SIZE_Y+20,
    //       " "
    //   );

    m_BaseNameText = std::make_shared<NumberSystem>(0.0f, 0.0f, 10.0f, 15.0f, RESOURCE_DIR"/img/moneyInfo.png", NumberSystem::FontType::WHITE_VERY_SMALL);
    // 強制把它的起始座標設為世界座標 (X: 玩家基地 X, Y: 基地上方)
    m_BaseNameText->SetPosition(GameConfig::PLAYER_BASE_X + 40.0f, GameConfig::BASE_Y + GameConfig::BASE_SIZE_Y +20.0f);
    m_BaseNameText->SetZIndex(60);

    m_EnemyBaseText = std::make_shared<NumberSystem>(0.0f, 0.0f, 10.0f, 15.0f, RESOURCE_DIR"/img/moneyInfo.png", NumberSystem::FontType::WHITE_VERY_SMALL);
    m_EnemyBaseText->SetPosition(GameConfig::ENEMY_BASE_X + 40.0f, GameConfig::BASE_Y + GameConfig::BASE_SIZE_Y +20.0f);
    m_EnemyBaseText->SetZIndex(60);

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

    m_PauseMenu->SetOnBgmVolumeChanged(
        [this](int level)
        {
            if (m_OnBgmVolumeChanged)
            {
                m_OnBgmVolumeChanged(level);
            }
        }
    );
    m_PauseMenu->SetOnClose([this]() {
        Util::SFX(RESOURCE_DIR "/music/clickbtn.mp3").Play();
        m_IsPaused = false;
        m_SureMenu.reset();
    });

m_PauseMenu->SetOnHelp(
    [this]()
    {
        Util::SFX(
            RESOURCE_DIR "/music/clickbtn.mp3"
        ).Play();

        m_DebugMenu =
            std::make_shared<DebugMenu>();

        // +10000 XP
        m_DebugMenu->SetOnAddXP(
            [this]()
            {
                auto playerData =
                    PlayerData::GetInstance();

                playerData->AddXP(
                    10000
                );

                playerData->SaveToFile();

                LOG_DEBUG(
                    "DebugMenu: Add 10000 XP. Current XP = {}",
                    playerData->GetXP()
                );
            }
        );

        // 戰鬥金錢加滿
        m_DebugMenu->SetOnMaxMoney(
            [this]()
            {
                LOG_DEBUG("MAX MONEY CALLBACK TRIGGERED");

                m_DebugMaxMoney =
                    !m_DebugMaxMoney;

                if (m_DebugMaxMoney)
                {
                    m_DebugMoneyBeforeMax =
                        m_CurrentMoney;

                    m_CurrentMoney =
                        m_CurrentMaxMoney;

                    LOG_DEBUG(
                        "DebugMenu: Max Money ON"
                    );
                }
                else
                {
                    m_CurrentMoney =
                        m_DebugMoneyBeforeMax;

                    if (m_CurrentMoney > m_CurrentMaxMoney)
                    {
                        m_CurrentMoney =
                            m_CurrentMaxMoney;
                    }

                    LOG_DEBUG(
                        "DebugMenu: Max Money OFF"
                    );
                }
            }
        );

        // 貓咪攻擊力 x2 開關
        m_DebugMenu->SetOnToggleCatAttack(
            []()
            {
                DebugCheat::CatAttackBoost =
                    !DebugCheat::CatAttackBoost;

                LOG_DEBUG(
                    "DebugMenu: Cat Attack x2 = {}",
                    DebugCheat::CatAttackBoost ? "ON" : "OFF"
                );
            }
        );

        // 貓咪跑速 x2 開關
        m_DebugMenu->SetOnToggleCatSpeed(
            [this]()
            {
                DebugCheat::CatSpeedBoost =
                    !DebugCheat::CatSpeedBoost;

                float speedScale =
                    DebugCheat::CatSpeedBoost
                        ? 2.0f
                        : 0.5f;

                for (auto& entity : m_Entities)
                {
                    if (
                        entity &&
                        entity->IsPlayerTeam()
                    )
                    {
                        entity->SetSpeed(
                            entity->GetSpeed() *
                            speedScale
                        );
                    }
                }

                LOG_DEBUG(
                    "DebugMenu: Cat Speed x2 = {}",
                    DebugCheat::CatSpeedBoost
                        ? "ON"
                        : "OFF"
                );
            }
        );


        // 立即勝利
        m_DebugMenu->SetOnInstantWin(
            [this]()
            {
                LOG_DEBUG(
                    "DebugMenu: Instant Win."
                );

                if (m_EnemyBase)
                {
                    m_EnemyBase->TakeDamage(
                        m_EnemyBase->GetHP()
                    );
                }

                // 關掉 DebugMenu / SureMenu
                m_DebugMenu.reset();
                m_SureMenu.reset();

                // 解除暫停，讓下一幀 GameScene::Update() 可以進入勝利結算
                m_IsPaused = false;
            }
        );

        // 返回 PauseMenu
        m_DebugMenu->SetOnBack(
            [this]()
            {
                Util::SFX(
                    RESOURCE_DIR "/music/clickbtn.mp3"
                ).Play();

                m_DebugMenu.reset();
            }
        );
    }
);
    m_PauseMenu->SetOnQuit([this]() {
        std::cout << "come back start scene\n";
        Util::SFX(RESOURCE_DIR "/music/clickbtn.mp3").Play();
        m_PauseMenu->SetBgZindex(51);
        m_SureMenu = std::make_shared<SureMenu>();

        m_SureMenu->SetOnConfirm(
            [this]()
            {
                Util::SFX(RESOURCE_DIR "/music/clickbtn.mp3").Play();

                m_SureMenu.reset();
                m_PauseMenu.reset();
                m_DebugMenu.reset();

                m_IsPaused = false;

                if (m_OnQuitGame)
                {
                    m_OnQuitGame();
                }
            }
        );

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

    // 💡 設定獎勵數量 (測試用，實際可讀取關卡設定)
    m_RewardXP = stage.rewardXP;
    // 🚀 1. 建立「獲得經驗值!!」文字 (放在稍微偏左上一點)
    m_RewardTextGet = std::make_shared<Button>(
        0.0f, 0.05f, windowWidth, 32.0f,
        RESOURCE_DIR"/img/GetXP.png", " ", 100, Util::Color(0,0,0,0)
    );



    // 🚀 3. 建立 XP 數字 (放在文字的更右邊)
    m_RewardXPNumber = std::make_shared<NumberSystem>(
        0.09f, 0.05f, 25.0f, 35.0f,
        RESOURCE_DIR"/img/moneyInfo.png",
        NumberSystem::FontType::YELLOW_BIG
    );
    m_RewardXPNumber->SetZIndex(90);
    m_OkBtn = std::make_shared<Button>(
        0.0f, -0.7f, 200.0f, 80.0f,
        RESOURCE_DIR"/img/OKBtn.png", // 拿原本的黃色按鈕當底
        " ", 35, Util::Color(255, 255, 255, 255)
    );
    m_OkBtn->SetZIndex(90);
    m_OkBtn->SetOnClick([this]() {
        Util::SFX(RESOURCE_DIR "/music/clickbtn.mp3").Play();
        // 點擊 OK 後，觸發退出遊戲的 callback (回到主選單或關卡選擇)
        if (m_OnQuitGame) m_OnQuitGame();

    });
}

void GameScene::UpdateRewardNotifyText()
{
    std::string title =
        "破關獎勵 入手啦！！";

    std::string rewardLine1;
    std::string rewardLine2;

    if (m_RewardCatFood > 0)
    {
        rewardLine1 +=
            "罐頭 +" +
            std::to_string(m_RewardCatFood);
    }

    if (m_RewardNormalTicket > 0)
    {
        if (!rewardLine1.empty())
        {
            rewardLine1 += "   ";
        }

        rewardLine1 +=
            "銀券 +" +
            std::to_string(m_RewardNormalTicket);
    }

    if (m_RewardRareTicket > 0)
    {
        rewardLine2 =
            "金券 +" +
            std::to_string(m_RewardRareTicket);
    }

    if (
        rewardLine1.empty() &&
        rewardLine2.empty()
    )
    {
        rewardLine1 =
            "本次沒有額外道具獎勵";
    }

    m_RewardNotifyTitle =
        std::make_shared<UIText>(
            -0.26f,
            -0.18f,
            title,
            34,
            Util::Color(255, 255, 255, 255)
        );

    m_RewardNotifyTitle->SetZIndex(90);

    m_RewardNotifyLine1 = nullptr;
    m_RewardNotifyLine2 = nullptr;
    m_RewardNotifyLine3 = nullptr;

    if (!rewardLine1.empty())
    {
        m_RewardNotifyLine1 =
            std::make_shared<UIText>(
                -0.20f,
                -0.33f,
                rewardLine1,
                30,
                Util::Color(220, 220, 220, 255)
            );

        m_RewardNotifyLine1->SetZIndex(90);
    }

    if (!rewardLine2.empty())
    {
        m_RewardNotifyLine2 =
            std::make_shared<UIText>(
                -0.10f,
                -0.45f,
                rewardLine2,
                30,
                Util::Color(220, 220, 220, 255)
            );

        m_RewardNotifyLine2->SetZIndex(90);
    }
}

void GameScene::GiveStageClearRewards()
{
    auto playerData =
        PlayerData::GetInstance();

    // 每次結算前先歸零本場掉落紀錄
    m_RewardCatFood = 0;
    m_RewardNormalTicket = 0;
    m_RewardRareTicket = 0;

    // =========================
    // 100% 掉 XP
    // =========================

    playerData->AddXP(
        m_RewardXP
    );

    LOG_DEBUG(
        "Stage Reward: +{} XP",
        m_RewardXP
    );

    // =========================
    // 50% 掉貓罐頭
    // =========================

    if (
        RollPercent(
            StageReward::CAT_FOOD_DROP_RATE
        )
    )
    {
        m_RewardCatFood =
            StageReward::CAT_FOOD_AMOUNT;

        playerData->AddCatFood(
            m_RewardCatFood
        );

        LOG_DEBUG(
            "Stage Reward: +{} Cat Food",
            m_RewardCatFood
        );
    }

    // =========================
    // 25% 掉銀券
    // =========================

    if (
        RollPercent(
            StageReward::NORMAL_TICKET_DROP_RATE
        )
    )
    {
        m_RewardNormalTicket =
            StageReward::NORMAL_TICKET_AMOUNT;

        playerData->AddNormalTickets(
            m_RewardNormalTicket
        );

        LOG_DEBUG(
            "Stage Reward: +{} Normal Ticket",
            m_RewardNormalTicket
        );
    }

    // =========================
    // 15% 掉金券
    // =========================

    if (
        RollPercent(
            StageReward::RARE_TICKET_DROP_RATE
        )
    )
    {
        m_RewardRareTicket =
            StageReward::RARE_TICKET_AMOUNT;

        playerData->AddRareTickets(
            m_RewardRareTicket
        );

        LOG_DEBUG(
            "Stage Reward: +{} Rare Ticket",
            m_RewardRareTicket
        );
    }

    playerData->ClearStage(
        m_CurrentStageID
    );
    UpdateRewardNotifyText();
    playerData->SaveToFile();
}

void GameScene::Update(float dt) {
    m_StageTimer += dt;

    // ==========================================
    // 1. 暫停邏輯
    // ==========================================
    if (pauseBtn) pauseBtn->Update();
    if (m_IsPaused)
    {
        if (m_SureMenu)
        {
            m_SureMenu->Update();
        }
        else if (m_DebugMenu)
        {
            m_DebugMenu->Update();
        }
        else if (m_PauseMenu)
        {
            m_PauseMenu->Update();
        }

        return;
    }

    // ==========================================
    // 🚀 2. 遊戲結束攔截器 (搬到這裡來！)
    // ==========================================
    if (!m_PlayerBase->IsAlive()) {
        if (m_BaseNameText) m_BaseNameText->SetValue(std::to_string(0)+'/'+std::to_string(m_PlayerBase->GetMaxHp()));

        if (!m_HasSettled) {
            std::cout << "🚨 [Game Over] 玩家主堡被摧毀，你輸了！\n";
            Util::SFX(RESOURCE_DIR "/music/lose.mp3").Play();
            m_HasSettled = true;
        }

        // 只有 OK 按鈕可以運作
        if (m_OkBtn) m_OkBtn->Update();

        // ⚠️ 關鍵：直接 return 結束，底下的貓咪跟隱形 UI 通通凍結！
        return;
    }
    else if (!m_EnemyBase->IsAlive()) {
        if (!m_HasSettled) {
            std::cout << "🏆 [Victory] 敵方主堡被摧毀，你贏了！\n";
            Util::SFX(RESOURCE_DIR "/music/win.mp3").Play();

            GiveStageClearRewards();

            if (m_RewardXPNumber) {
                m_RewardXPNumber->SetValue(  std::to_string(m_RewardXP)); // 加個 + 號更有感覺
            }

            m_HasSettled = true;
        }

        if (m_EnemyBaseText) m_EnemyBaseText->SetValue(std::to_string(0)+'/'+std::to_string(m_EnemyBase->GetMaxHp()));

        // 只有 OK 按鈕可以運作
        if (m_OkBtn) m_OkBtn->Update();

        // ⚠️ 關鍵：直接 return 結束，底下的貓咪跟隱形 UI 通通凍結！
        return;
    }

    // ==========================================
    // 3. 正常的遊戲迴圈 (如果上面沒 return，代表遊戲還在進行)
    // ==========================================
    m_CurrentMoney += GameConfig::MONEY_GROWTH_SPEED * dt;
    if (m_CurrentMoney > m_CurrentMaxMoney) {
        m_CurrentMoney = m_CurrentMaxMoney;
    }
    if (m_DebugMaxMoney)
    {
        m_CurrentMoney =
            m_CurrentMaxMoney;
    }

    if (Util::Input::IsKeyPressed(Util::Keycode::RIGHT) || Util::Input::IsKeyPressed(Util::Keycode::D)) m_CameraX += m_CameraSpeed * dt;
    if (Util::Input::IsKeyPressed(Util::Keycode::LEFT) || Util::Input::IsKeyPressed(Util::Keycode::A)) m_CameraX -= m_CameraSpeed * dt;
    if (m_CameraX < GameConfig::CAMERA_MIN_X) m_CameraX = GameConfig::CAMERA_MIN_X;
    if (m_CameraX > GameConfig::CAMERA_MAX_X) m_CameraX = GameConfig::CAMERA_MAX_X;

    m_UISystem.Update(m_EquippedDeck, m_SpawnSystem.GetCooldownTimers(), m_CurrentMoney, m_WalletUpgradeCost, m_PlayerBase->GetCannonProgress(), m_PlayerBase->IsCannonReady());
    int clickedSlot = m_UISystem.GetClickedSlot();

    m_SpawnSystem.Update(dt, m_StageTimer, m_Stage, m_Entities, m_CurrentMoney, m_PlayerBase, m_EnemyBase, m_EquippedDeck, clickedSlot);
    m_UISystem.ResetClick();

    for (auto& entity : m_Entities) {
        entity->Update(dt);
    }

    m_CollisionSystem.Update(m_Entities);
    m_BattleSystem.Update(dt, m_Entities);
    RemoveDeadEntities();

    // 🚀 更新血量與金錢數字
    if (m_CurrentMoneyNumber) m_CurrentMoneyNumber->SetValue(static_cast<int>(m_CurrentMoney));
    if (m_MaxMoneyNumber) m_MaxMoneyNumber->SetValue(std::to_string(static_cast<int>(m_CurrentMoney))+'/'+std::to_string(static_cast<int>(m_CurrentMaxMoney))+'$') ;

    if (m_BaseNameText) m_BaseNameText->SetValue(std::to_string(m_PlayerBase->GetHP())+'/'+std::to_string(m_PlayerBase->GetMaxHp()));
    if (m_EnemyBaseText) m_EnemyBaseText->SetValue(std::to_string(m_EnemyBase->GetHP())+'/'+std::to_string(m_EnemyBase->GetMaxHp()));
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


    bool isGameOver = !m_PlayerBase->IsAlive() || !m_EnemyBase->IsAlive();
    // 🚀 如果遊戲【還沒結束】，才畫出下方的格子、錢包、大砲與暫停按鈕
    if (!isGameOver) {
        m_UISystem.Draw(m_EquippedDeck, m_SpawnSystem.GetCooldownTimers(), m_CurrentMoney);
        if (pauseBtn) pauseBtn->Draw();
    }
    // 🚀 替換勝利與失敗的畫面，使用 img004_tw.png 切圖
    if (!m_PlayerBase->IsAlive()) {
        if (m_LoseImage) m_LoseImage->DrawRect(0, 110, 309, 110);

        // 畫出 OK 按鈕
        if (m_OkBtn) {
            m_OkBtn->Draw();
        };
    }
    else if (!m_EnemyBase->IsAlive())
    {
        if (m_WinImage)
        {
            m_WinImage->Draw();
        }

        if (m_RewardTextGet)
        {
            m_RewardTextGet->Draw();
        }

        if (m_RewardXPNumber)
        {
            m_RewardXPNumber->Draw();
        }

        if (m_RewardNotifyBar)
        {
            m_RewardNotifyBar->Draw();
        }

        if (m_RewardNotifyTitle)
        {
            m_RewardNotifyTitle->Draw();
        }

        if (m_RewardNotifyLine1)
        {
            m_RewardNotifyLine1->Draw();
        }

        if (m_RewardNotifyLine2)
        {
            m_RewardNotifyLine2->Draw();
        }

        if (m_RewardNotifyLine3)
        {
            m_RewardNotifyLine3->Draw();
        }

        if (m_OkBtn)
        {
            m_OkBtn->Draw();
        }
    }

    if (pauseBtn) pauseBtn->Draw();

    if (m_IsPaused)
    {
        if (m_SureMenu)
        {
            if (m_PauseMenu)
            {
                m_PauseMenu->DrawBackgroundOnly();
            }

            m_SureMenu->Draw();
        }
        else if (m_DebugMenu)
        {
            if (m_PauseMenu)
            {
                m_PauseMenu->DrawBackgroundOnly();
            }

            m_DebugMenu->Draw();
        }
        else if (m_PauseMenu)
        {
            m_PauseMenu->Draw();
        }
    }
    if (m_StageNameTextShadow)
    {
        m_StageNameTextShadow->Draw();
    }

    /*if (m_StageNameText)
    {
        m_StageNameText->Draw();
    }*/
}
