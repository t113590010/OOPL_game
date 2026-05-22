#ifndef SPAWNSYSTEM_HPP
#define SPAWNSYSTEM_HPP

#include "../StageConfig.hpp"
#include <vector>
#include <memory>
#include "Entity/Entity.hpp"
#include "Entity/Base.hpp"
#include "Entity/UnitID.hpp"

class SpawnSystem {
public:
    SpawnSystem(const std::vector<EnemyWave>& waves);

    void Update(
        float dt,
        float stageTimer,
        const StageData& stage,
        std::vector<std::shared_ptr<Entity>>& entities,
        float& currentMoney,
        const std::shared_ptr<Base>& playerBase,
        const std::shared_ptr<Base>& enemyBase,
        const std::vector<UnitID>& playerDeck,
        int clickedSlot = -1
    );

    const float* GetCooldownTimers() const { return m_CooldownTimers; }

private:
    float m_CooldownTimers[10] = {0.0f};
    int enemyCount;
    std::vector<EnemyWave> m_EnemyWaves;
};

#endif