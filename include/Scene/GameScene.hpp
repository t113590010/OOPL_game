#ifndef GAME_SCENE_HPP
#define GAME_SCENE_HPP

#include <vector>
#include <memory>
#include "Entity/Base.hpp"
#include "GameConfig.hpp"
#include "System/UIText.hpp"
#include "System/Button.hpp"
#include "System/WorldText.hpp"
#include "System/Background.hpp"
#include "Entity/UnitID.hpp"
#include "System/SpawnSystem.hpp"
#include "System/CollisionSystem.hpp"
#include "System/UISystem.hpp"
#include "System/BattleSystem.hpp"
#include "PauseMenu.hpp"
#include "SureMenu.hpp"
#include "StageConfig.hpp"
#include "NumberSystem.hpp"
#include <functional>
#include "DebugMenu.hpp"
class GameScene {
public:
    GameScene(
        const std::vector<UnitID>& playerDeck,
        const StageData& stage
    );
    void Update(float dt);          // 每幀更新

    void Draw();
    bool IsGameOver() const {
        return !m_PlayerBase->IsAlive() || !m_EnemyBase->IsAlive();
    }

    // (選用) 如果你之後要在 App 裡印出「勝利」或「失敗」的不同圖案，可以用這個來判斷
    bool IsPlayerWin() const {
        return !m_EnemyBase->IsAlive();
    }
    void SetOnQuitGame(std::function<void()> callback) {
        m_OnQuitGame = callback;
    }
    void SetOnBgmVolumeChanged(
        std::function<void(int)> callback
    )
    {
        m_OnBgmVolumeChanged = callback;
    }
private:
    void RemoveDeadEntities();      // 刪除死亡單位
    StageData m_Stage;

    float m_StageTimer = 0.0f;
    std::vector<std::shared_ptr<Entity>> m_Entities;
    std::shared_ptr<Base> m_PlayerBase;
    std::shared_ptr<Base> m_EnemyBase;
    std::shared_ptr<NumberSystem> m_CurrentMoneyNumber;
    std::shared_ptr<NumberSystem> m_MaxMoneyNumber;


    // 攝影機目前的 X 座標 (預設停在最左邊的貓咪基地)
    float m_CameraX = -600.0f;

    // 攝影機移動的速度
    float m_CameraSpeed = 500.0f;
    float m_CurrentMoney = GameConfig::INITIAL_MONEY;
    std::shared_ptr<NumberSystem> m_BaseNameText;
    std::shared_ptr<NumberSystem> m_EnemyBaseText;
    std::shared_ptr<Background> m_Background;
    SpawnSystem m_SpawnSystem;
    CollisionSystem m_CollisionSystem;
    std::vector<UnitID> m_EquippedDeck;
    UISystem m_UISystem;
    BattleSystem m_BattleSystem;
    std::shared_ptr<Button> m_WinImage;  // 使用 Button 單純為了方便 DrawRect
    std::shared_ptr<Button> m_LoseImage;
    std::shared_ptr<Button> pauseBtn;
    std::vector<std::shared_ptr<Button>> m_SlotButtons;
    bool m_IsPaused = false;
    std::shared_ptr<PauseMenu> m_PauseMenu;

    int m_WalletLevel = 1;
    // 🚀 初始花費直接吃 GameConfig
    int m_WalletUpgradeCost = GameConfig::WALLET_UPGRADE_INITIAL_COST;

    // 🚀 新增一個變數，記錄「當下的」金錢上限
    float m_CurrentMaxMoney = GameConfig::MAX_MONEY_LEVEL_1;
    std::function<void()> m_OnQuitGame;
    std::shared_ptr<SureMenu> m_SureMenu;

    bool m_HasSettled = false;                      // 確保獎勵只加一次的防重複鎖
    int m_RewardXP = 0;                             // 本關卡掉落的 XP 數量
    int m_RewardCatFood = 0;
    int m_RewardNormalTicket = 0;
    int m_RewardRareTicket = 0;
    void GiveStageClearRewards();

    std::shared_ptr<Button> m_RewardNotifyBar;
    bool m_ShowRewardNotify = false;

    std::shared_ptr<UIText> m_RewardNotifyTitle;
    std::shared_ptr<UIText> m_RewardNotifyLine1;
    std::shared_ptr<UIText> m_RewardNotifyLine2;
    std::shared_ptr<UIText> m_RewardNotifyLine3;

    void UpdateRewardNotifyText();

    std::shared_ptr<Button> m_RewardTextGet;        // 🚀 新增：「獲得」的切圖
    std::shared_ptr<Button> m_OkBtn;
    int m_CurrentStageID ;
    std::shared_ptr<NumberSystem> m_RewardXPNumber; // 顯示獲得多少 XP 的數字系統

    std::function<void(int)> m_OnBgmVolumeChanged;

    std::shared_ptr<UIText> m_StageNameTextShadow;
    std::shared_ptr<UIText> m_StageNameText;

    std::shared_ptr<DebugMenu> m_DebugMenu;

    // Debug 狀態
    bool m_DebugCatAttackBoost = false;
    bool m_DebugCatSpeedBoost = false;
    bool m_DebugCooldownHalf = false;
    bool m_DebugMaxMoney = false;
    int m_DebugMoneyBeforeMax = 0;


};

#endif