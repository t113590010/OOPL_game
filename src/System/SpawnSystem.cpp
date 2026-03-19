#include "System/SpawnSystem.hpp"
#include "Entity/UnitFactory.hpp"
#include "Util/Input.hpp"
#include "GameConfig.hpp"
#include <iostream>

// 💡 1. 在建構子裡「註冊」你要生成的敵人 (完全解耦！)
SpawnSystem::SpawnSystem() {
    // 參數：{ 怪物種類, 生成冷卻時間, 初始計時(預設0) }
    m_EnemySpawners.push_back({UnitID::BASIC_ENEMY, 5.0f, 0.0f}); // 每 3 秒生一隻基礎小怪
    // m_EnemySpawners.push_back({UnitID::GAY,         8.0f, 0.0f}); // 每 8 秒生一隻新敵人
}

void SpawnSystem::Update(float dt, std::vector<std::shared_ptr<Entity>>& entities, float& currentMoney,
                         const std::shared_ptr<Base>& playerBase, const std::shared_ptr<Base>& enemyBase,
                         const std::vector<UnitID>& playerDeck) {

// 0. 更新玩家的冷卻碼表
    for (int i = 0; i < playerDeck.size(); ++i) {
        if (m_CooldownTimers[i] > 0.0f) {
            float lastTime = m_CooldownTimers[i]; // 紀錄減去前的時間
            m_CooldownTimers[i] -= dt;

            // ==========================================
            // 🔔 偵測冷卻結束的瞬間
            // 如果原本 > 0，減完之後 <= 0，代表這幀剛好冷卻完！
            // ==========================================
            if (m_CooldownTimers[i] <= 0.0f) {
                m_CooldownTimers[i] = 0.0f; // 歸零防呆

                // 播放冷卻完成的音效 (例如那種「叮」一聲)
                Util::SFX(RESOURCE_DIR "/music/cd_done.mp3").Play();

                std::cout << "[System] Slot " << (i + 1) << " Ready!\n";
            }
        }
    }
    // 1. 處理敵人生成 (💡 陣列資料驅動版)
    for (auto& spawner : m_EnemySpawners) {
        spawner.currentTimer += dt;

        if (spawner.currentTimer > spawner.cooldown) {
            // 🚨 方向鐵律：敵人在左邊，往右推進，所以生在基地的「右邊」 (+)
            float spawnX = enemyBase->GetPosition().x + GameConfig::SPAWN_OFFSET_X;
            float spawnY = GameConfig::BASE_Y + GameConfig::SPAWN_OFFSET_Y;

            auto enemy = UnitFactory::CreateUnit(spawner.id, spawnX, spawnY, false);
            if (enemy) {
                entities.push_back(enemy);
            }

            // 重置這隻專屬怪物的計時器
            spawner.currentTimer = 0.0f;
        }
    }

    // 2. 處理玩家按鍵選兵
    const Util::Keycode slotKeys[10] = {
        Util::Keycode::NUM_1, Util::Keycode::NUM_2, Util::Keycode::NUM_3,
        Util::Keycode::NUM_4, Util::Keycode::NUM_5, Util::Keycode::NUM_6, Util::Keycode::NUM_7, Util::Keycode::NUM_8,
        Util::Keycode::NUM_9, Util::Keycode::NUM_0
    };

    for (size_t i = 0; i < playerDeck.size(); ++i) {
        if (Util::Input::IsKeyDown(slotKeys[i])) {
            UnitID targetId = playerDeck[i];

            if (targetId != UnitID::NONE && m_CooldownTimers[i] <= 0.0f) {

                int cost = UnitFactory::GetUnitCost(targetId);

                if (currentMoney >= cost) {
                    // 🚨 方向鐵律：玩家在右邊，往左推進，所以生在基地的「左邊」 (-)
                    float spawnX = playerBase->GetPosition().x - GameConfig::SPAWN_OFFSET_X;
                    float spawnY = GameConfig::BASE_Y + GameConfig::SPAWN_OFFSET_Y;

                    auto newUnit = UnitFactory::CreateUnit(targetId, spawnX, spawnY, true);

                    if (newUnit) {
                        currentMoney -= cost;
                        m_CooldownTimers[i] = UnitFactory::GetUnitSpawnCooldown(targetId);
                        entities.push_back(newUnit);
                        Util::SFX(RESOURCE_DIR "/music/succes_summon_cat.mp3").Play();
                        std::cout << "[System] Slot " << (i + 1) << " Spawned! Money: " << (int)currentMoney << "\n";
                    }
                } else {
                    std::cout << "[System] Not enough money! Need: " << cost << "\n";
                    Util::SFX(RESOURCE_DIR "/music/fail_summon_cat.mp3").Play();
                }
            }else {
                    Util::SFX(RESOURCE_DIR "/music/fail_summon_cat.mp3").Play();

            }
        }
    }
}