#ifndef GAME_SCENE_HPP
#define GAME_SCENE_HPP

#include <vector>
#include <memory>
#include "Entity/Cats/LongLegCat.hpp"
#include "Entity/Enemies/Enemy.hpp"
#include "Entity/Base.hpp"
#include "GameConfig.hpp"
#include "System/UIText.hpp"
#include "Util/Text.hpp"
#include "System/WorldText.hpp"
#include "System/Background.hpp"
#include "Entity/UnitID.hpp"
#include "System/SpawnSystem.hpp"
#include "System/CollisionSystem.hpp"
#include "System/UISystem.hpp"
#include "System/BattleSystem.hpp"

class GameScene {
public:
    GameScene(const std::vector<UnitID>& playerDeck);

    void Update(float dt);          // 每幀更新

    void Draw();
    bool IsGameOver() const {
        return !m_PlayerBase->IsAlive() || !m_EnemyBase->IsAlive();
    }

    // (選用) 如果你之後要在 App 裡印出「勝利」或「失敗」的不同圖案，可以用這個來判斷
    bool IsPlayerWin() const {
        return !m_EnemyBase->IsAlive();
    }
private:

    void RemoveDeadEntities();      // 刪除死亡單位

    std::vector<std::shared_ptr<Entity>> m_Entities;
    std::shared_ptr<Base> m_PlayerBase;
    std::shared_ptr<Base> m_EnemyBase;
    std::shared_ptr<UIText> m_MoneyText;


    // 攝影機目前的 X 座標 (預設停在最左邊的貓咪基地)
    float m_CameraX = -600.0f;

    // 攝影機移動的速度
    float m_CameraSpeed = 500.0f;
    float m_CurrentMoney = GameConfig::INITIAL_MONEY;
    std::shared_ptr<WorldText> m_BaseNameText;
    std::shared_ptr<WorldText> m_EnemyBaseText;
    std::shared_ptr<Background> m_Background;
    SpawnSystem m_SpawnSystem;
    CollisionSystem m_CollisionSystem;
    std::vector<UnitID> m_EquippedDeck;
    UISystem m_UISystem;
    BattleSystem m_BattleSystem;
    std::shared_ptr<UIText> m_WinText;
    std::shared_ptr<UIText> m_LoseText;
};

#endif