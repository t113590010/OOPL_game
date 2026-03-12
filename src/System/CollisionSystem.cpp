#include "System/CollisionSystem.hpp"

void CollisionSystem::Update(std::vector<std::shared_ptr<Entity>>& entities) {
    // 1. 每幀開始，先重置所有存活單位的移動狀態
    for (auto& e : entities) {
        if (e->IsAlive()) {
            e->SetMoving(true);
        }
    }

    // 2. 判斷誰進入了誰的射程 (物理阻擋)
    for (auto& entityA : entities) {
        if (!entityA->IsAlive()) continue;

        for (auto& entityB : entities) {
            // 排除自己、死者、同陣營
            if (entityA == entityB || !entityB->IsAlive()) continue;
            if (entityA->IsPlayerTeam() == entityB->IsPlayerTeam()) continue;

            // 呼叫 Entity 規範好的 AABB + 射程判定
            if (entityA->CheckCollision(entityB)) {
                entityA->SetMoving(false);
                break;
            }
        }
    }
}