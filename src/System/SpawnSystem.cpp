#include "System/SpawnSystem.hpp"
#include "Entity/UnitFactory.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "GameConfig.hpp"
#include <iostream>

void SpawnSystem::Update(float dt, std::vector<std::shared_ptr<Entity>>& entities, float& currentMoney, const std::shared_ptr<Base>& playerBase, const std::shared_ptr<Base>& enemyBase, const std::vector<UnitID>& playerDeck) {

    // 0. 更新碼表
    for (int i = 0; i < 5; ++i) {
        if (m_CooldownTimers[i] > 0.0f) m_CooldownTimers[i] -= dt;
    }

    // 1. 處理敵人生成
    m_EnemySpawnTimer += dt;
    if (m_EnemySpawnTimer > 2.0f) {
        float spawnX = enemyBase->GetPosition().x - GameConfig::SPAWN_OFFSET_X;
        entities.push_back(UnitFactory::CreateUnit(UnitID::BASIC_ENEMY, Vector2{spawnX, GameConfig::BASE_Y}));
        m_EnemySpawnTimer = 0.0f;
    }

    // 💡 2. 處理玩家按鍵選兵 (不再偷懶寫死 NUM_1 和 NUM_2！)
    // 定義槽位對應的按鍵 (未來這裡甚至可以從 GameConfig 讀取玩家自訂按鍵)
    const Util::Keycode slotKeys[5] = {
        Util::Keycode::NUM_1,
        Util::Keycode::NUM_2,
        Util::Keycode::NUM_3,
        Util::Keycode::NUM_4,
        Util::Keycode::NUM_5
    };

    // 用迴圈自動掃描玩家的牌組陣列
    for (size_t i = 0; i < playerDeck.size() && i < 5; ++i) {

        // 檢查該槽位對應的按鍵是否被按下
        if (Util::Input::IsKeyDown(slotKeys[i])) {

            UnitID targetId = playerDeck[i];

            // 檢查該槽位是否有裝備貓咪，且冷卻完畢
            if (targetId != UnitID::NONE && m_CooldownTimers[i] <= 0.0f) {

                float spawnX = playerBase->GetPosition().x + GameConfig::SPAWN_OFFSET_X;
                auto newUnit = UnitFactory::CreateUnit(targetId, Vector2{spawnX, GameConfig::BASE_Y});

                // 檢查是否買得起
                if (newUnit && currentMoney >= newUnit->GetUnitCost()) {
                    currentMoney -= newUnit->GetUnitCost();
                    m_CooldownTimers[i] = newUnit->GetSpawnCooldown();
                    entities.push_back(newUnit);

                    std::cout << "[System] Spawned Unit from Slot " << (i + 1) << "! Money left: " << (int)currentMoney << "\n";
                }
            }
        }
    }
}