#ifndef SPAWN_SYSTEM_HPP
#define SPAWN_SYSTEM_HPP

#include <vector>
#include <memory>
#include "Entity/Entity.hpp"
#include "Entity/Base.hpp"

class SpawnSystem {
public:
    // 💡 系統的 Update，需要把 GameScene 的資料「借」給它用 (使用 & 傳參考)
    void Update(float dt,
                std::vector<std::shared_ptr<Entity>>& entities,
                float& currentMoney,
                const std::shared_ptr<Base>& playerBase,
                const std::shared_ptr<Base>& enemyBase);

private:
    void SpawnCat(std::vector<std::shared_ptr<Entity>>& entities, float& currentMoney, const std::shared_ptr<Base>& playerBase);
    void SpawnEnemy(std::vector<std::shared_ptr<Entity>>& entities, const std::shared_ptr<Base>& enemyBase);

    // 💡 我們一直想加的計時器，現在乾淨地收攏在這裡了！
    float m_EnemySpawnTimer = 0.0f;
    const float ENEMY_SPAWN_INTERVAL = 2.0f; // 敵人每 2 秒生一隻

    float m_CatCooldownTimer = 0.0f;
    const float CAT_COOLDOWN_TIME = 2.0f;    // 產貓冷卻 2 秒
};

#endif