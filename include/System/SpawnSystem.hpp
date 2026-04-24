#ifndef SPAWNSYSTEM_HPP
#define SPAWNSYSTEM_HPP
#include "StageConfig.hpp" // 加上這行
#include <vector>
#include <memory>
#include "Entity/Entity.hpp"
#include "Entity/Base.hpp"
#include "Entity/UnitID.hpp"

// 💡 定義敵人的「生成器」結構
struct EnemySpawner {
    UnitID id;            // 怪物種類
    float cooldown;       // 幾秒生一隻
    float currentTimer;   // 目前計時碼表
};

class SpawnSystem {
public:
    SpawnSystem(const std::vector<EnemyWave>& waves);

    void Update(float dt, std::vector<std::shared_ptr<Entity>>& entities, float& currentMoney,
                const std::shared_ptr<Base>& playerBase, const std::shared_ptr<Base>& enemyBase,
                const std::vector<UnitID>& playerDeck,
                int clickedSlot = -1);

    // 給 UI 系統讀取冷卻條用的
    const float* GetCooldownTimers() const { return m_CooldownTimers; }

private:
    float m_CooldownTimers[10] = {0.0f};          // 玩家的 10 格產兵冷卻
    std::vector<EnemySpawner> m_EnemySpawners;   // 💡 敵人的資料驅動產兵陣列
};

#endif