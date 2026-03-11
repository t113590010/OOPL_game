#ifndef SPAWN_SYSTEM_HPP
#define SPAWN_SYSTEM_HPP

#include <vector>
#include <memory>
#include "Entity/Entity.hpp"
#include "Entity/Base.hpp"
#include "Entity/UnitID.hpp"

class SpawnSystem {
public:
    // 💡 參數新增了 const std::vector<UnitID>& playerDeck
    void Update(float dt,
                std::vector<std::shared_ptr<Entity>>& entities,
                float& currentMoney,
                const std::shared_ptr<Base>& playerBase,
                const std::shared_ptr<Base>& enemyBase,
                const std::vector<UnitID>& playerDeck); // 👈 這裡！

    const float* GetCooldownTimers() const { return m_CooldownTimers; }

private:
    float m_EnemySpawnTimer = 0.0f;
    float m_CooldownTimers[5] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    // ❌ 刪除原本寫死在這裡的 m_PlayerDeck
};

#endif