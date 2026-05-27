#include "System/SpawnSystem.hpp"
#include "Entity/UnitFactory.hpp"
#include "Util/Input.hpp"
#include "GameConfig.hpp"
#include <iostream>

SpawnSystem::SpawnSystem(const std::vector<EnemyWave>& waves)
    : m_EnemyWaves(waves) {
}

void SpawnSystem::Update(
    float dt,
    float stageTimer,
    const StageData& stage,
    std::vector<std::shared_ptr<Entity>>& entities,
    float& currentMoney,
    const std::shared_ptr<Base>& playerBase,
    const std::shared_ptr<Base>& enemyBase,
    const std::vector<UnitID>& playerDeck,
    int clickedSlot
) {
    // 0. 更新玩家冷卻
    for (size_t i = 0; i < playerDeck.size() && i < 10; ++i) {
        if (m_CooldownTimers[i] > 0.0f) {
            m_CooldownTimers[i] -= dt;

            if (m_CooldownTimers[i] <= 0.0f) {
                m_CooldownTimers[i] = 0.0f;
                Util::SFX(RESOURCE_DIR "/music/cd_done.mp3").Play();
                std::cout << "[System] Slot " << (i + 1) << " Ready!\n";
            }
        }
    }
    // 1. 敵人 Wave 出怪系統
    enemyCount = 0;

    for (const auto& entity : entities) {

        if (!entity) continue;

        // 不算敵方基地
        if (entity == enemyBase) continue;

        // 只算敵人
        if (!entity->IsPlayerTeam()) {
            enemyCount++;
        }
    }
    for (auto& wave : m_EnemyWaves) {

        if (!wave.activated) {
            switch (wave.triggerType) {
                case WaveTriggerType::TIME:
                    if (stageTimer >= wave.triggerValue) {
                        wave.activated = true;
                        wave.timer = 0.0f;

                        float random01 =
                            static_cast<float>(rand()) / static_cast<float>(RAND_MAX);

                        wave.nextSpawnDelay =
                            wave.repeatMin +
                            (wave.repeatMax - wave.repeatMin) * random01;
                    }
                    break;

                case WaveTriggerType::ENEMY_HP:
                    if (enemyBase &&
                        static_cast<float>(enemyBase->GetHP()) / stage.enemyBaseHP <= wave.triggerValue) {
                        wave.activated = true;
                        wave.timer = 0.0f;
                        }
                    break;
            }
        }

        if (!wave.activated || wave.completed) continue;

        wave.timer += dt;

        float targetTime = (wave.spawnedCount == 0)
            ? wave.firstDelay
            : wave.nextSpawnDelay;

        if (wave.timer >= targetTime) {
            if (wave.timer >= targetTime) {

                // =========================
                // 敵人數量上限
                // =========================

                if (enemyCount >= stage.maxEnemyCount) {
                    continue;
                }
                float spawnX = enemyBase->GetPosition().x + GameConfig::SPAWN_OFFSET_X;
                float spawnY = GameConfig::BASE_Y + GameConfig::SPAWN_OFFSET_Y
                             + (static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f)
                             * GameConfig::RANDOM_SPAWN_OFFSET_Y_MAX;

                auto enemy = UnitFactory::CreateUnit(wave.id, spawnX, spawnY, false);
                if (enemy) {
                    entities.push_back(enemy);
                }

                wave.spawnedCount++;
                wave.timer = 0.0f;

                // ========================
                // 重新決定下一次 cooldown
                // ========================

                if (wave.repeatMax <= wave.repeatMin) {
                    wave.nextSpawnDelay = wave.repeatMin;
                }
                else {
                    if (wave.repeatMax <= wave.repeatMin) {
                        wave.nextSpawnDelay = wave.repeatMin;
                    }
                    else {

                        float random01 =
                            static_cast<float>(rand()) /
                            static_cast<float>(RAND_MAX);

                        wave.nextSpawnDelay =
                            wave.repeatMin +
                            (wave.repeatMax - wave.repeatMin) * random01;
                    }
                }

                if (wave.totalCount != -1 &&
                    wave.spawnedCount >= wave.totalCount) {
                    wave.completed = true;
                    }
            }
        }
    }
    const Util::Keycode slotKeys[10] = {
        Util::Keycode::NUM_1, Util::Keycode::NUM_2, Util::Keycode::NUM_3,
        Util::Keycode::NUM_4, Util::Keycode::NUM_5, Util::Keycode::NUM_6,
        Util::Keycode::NUM_7, Util::Keycode::NUM_8,
        Util::Keycode::NUM_9, Util::Keycode::NUM_0
    };

    for (size_t i = 0; i < playerDeck.size() && i < 10; ++i) {
        UnitID targetId = playerDeck[i];
        if (targetId == UnitID::NONE) continue;

        bool keyTriggered = Util::Input::IsKeyDown(slotKeys[i]);
        bool buttonTriggered = (clickedSlot == static_cast<int>(i));

        if (keyTriggered || buttonTriggered) {
            if (m_CooldownTimers[i] <= 0.0f) {
                int cost = UnitData::Get(targetId).cost;

                if (currentMoney >= cost) {
                    float spawnX = playerBase->GetPosition().x + GameConfig::SPAWN_OFFSET_X;
                    float spawnY = GameConfig::BASE_Y + GameConfig::SPAWN_OFFSET_Y
                                 + (static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f)
                                 * GameConfig::RANDOM_SPAWN_OFFSET_Y_MAX;
                    UnitLevelData debugLevel;

                    debugLevel.baseLevel = 20;
                    debugLevel.plusLevel = 0;

                    auto newUnit = UnitFactory::CreateUnit(targetId, spawnX, spawnY, true, debugLevel);

                    if (newUnit) {
                        currentMoney -= cost;
                        m_CooldownTimers[i] = UnitData::Get(targetId).spawnCd;
                        entities.push_back(newUnit);
                        Util::SFX(RESOURCE_DIR "/music/succes_summon_cat.mp3").Play();
                    }
                } else {
                    Util::SFX(RESOURCE_DIR "/music/fail_summon_cat.mp3").Play();
                }
            } else if (buttonTriggered) {
                Util::SFX(RESOURCE_DIR "/music/fail_summon_cat.mp3").Play();
            }
        }
    }
}