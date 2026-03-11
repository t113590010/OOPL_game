#include "System/CollisionSystem.hpp"
#include <iostream>

void CollisionSystem::Update(std::vector<std::shared_ptr<Entity>>& entities) {
    for (auto& e : entities) {
        e->SetMoving(true);
    }

    for (auto& entityA : entities) {
        if (!entityA->IsAlive()) continue;

        for (auto& entityB : entities) {
            if (entityA == entityB || !entityB->IsAlive()) continue;
            if (entityA->IsPlayerTeam() == entityB->IsPlayerTeam()) continue;

            if (entityA->CheckCollision(entityB, entityA->GetAttackRange())) {
                entityA->SetMoving(false);
                if (entityA->CanAttack()) {
                    entityB->TakeDamage(entityA->GetDamage());
                    entityA->ResetAttackTimer();
                    std::cout << (entityA->IsPlayerTeam() ? "Cat" : "Enemy")
                              << " attacked! Target HP left: " << entityB->GetHP() << "\n";
                }
                break;
            }
        }
    }
}