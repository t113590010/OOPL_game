#include "System/BattleSystem.hpp"
#include <iostream>

// ✅ 注意這裡的命名空間是 BattleSystem::，這樣就不會跟 CollisionSystem 衝突了
void BattleSystem::Update(float dt, std::vector<std::shared_ptr<Entity>>& entities) {
    for (auto& attacker : entities) {
        if (!attacker->IsAlive()) continue;

        // 1. 系統負責更新實體的冷卻時間
        attacker->UpdateCooldown(dt);

        // 2. 冷卻還沒轉好，直接跳過，不浪費效能找目標
        if (!attacker->CanAttack()) continue;

        // 3. 尋找攻擊目標
        for (auto& target : entities) {
            // 排除自己、死者、同陣營
            if (attacker == target || !target->IsAlive()) continue;
            if (attacker->IsPlayerTeam() == target->IsPlayerTeam()) continue;

            // 4. 再次呼叫判定：是否在射程內且 Y 軸重疊
            if (attacker->CheckCollision(target)) {

                // 執行傷害
                target->TakeDamage(attacker->GetDamage());

                // 系統下令重置冷卻
                attacker->ResetAttackTimer();

                // 偵錯訊息 (正式版可註解掉)
                std::cout << (attacker->IsPlayerTeam() ? "[Cat] " : "[Enemy] ")
                          << "Dealt " << attacker->GetDamage() << " DMG! "
                          << "Target HP left: " << target->GetHP() << "\n";

                // 找到第一個目標打完就收手
                break;
            }
        }
    }
}