#include "System/BattleSystem.hpp"
#include <iostream>

void BattleSystem::Update(float dt, std::vector<std::shared_ptr<Entity>>& entities) {
    for (auto& attacker : entities) {
        if (!attacker->IsAlive()) continue;

        // 1. 系統負責更新實體的冷卻時間
        attacker->UpdateCooldown(dt);

        // 2. 冷卻還沒轉好，直接跳過，不浪費效能找目標
        if (!attacker->CanAttack()) continue;

        // 💡 增加一個開關：記錄這回合有沒有揮出拳頭
        bool hasAttacked = false;

        // 3. 尋找攻擊目標
        for (auto& target : entities) {
            // 排除自己、死者、同陣營
            if (attacker == target || !target->IsAlive()) continue;
            if (attacker->IsPlayerTeam() == target->IsPlayerTeam()) continue;

            // 4. 再次呼叫判定：是否在射程內且 Y 軸重疊
            if (attacker->CheckCollision(target)) {
                if (attacker->GetState() != EntityState::ATTACK) {
                    attacker->SetState(EntityState::ATTACK);
                }
                // 執行傷害
                target->TakeDamage(attacker->GetDamage());

                // 💡 標記為「打中了！」，但不急著在這裡重置冷卻
                hasAttacked = true;

                // 偵錯訊息 (正式版可註解掉)
                std::cout << (attacker->IsPlayerTeam() ? "[Cat] " : "[Enemy] ")
                          << "Dealt " << attacker->GetDamage() << " DMG! "
                          << "Target HP left: " << target->GetHP() << "\n";

                if (!attacker->IsAoE()) {
                    break; // ❌ 單體攻擊：打到第一隻就立刻收手，跳出內層迴圈！
                }
            }
        }

        // ==========================================
        // 💡 5. 結算：如果這回合有打到任何人，才重置冷卻
        // 這樣可以保證，不管是單體還是範圍攻擊，冷卻都只會被重置一次！
        // ==========================================
        if (hasAttacked) {
            attacker->ResetAttackTimer();
        } else {
            // 如果這回合沒打到任何人，且實體不是在播死亡動畫，就應該恢復走路
            if (attacker->GetState() != EntityState::DEATH && attacker->GetState() != EntityState::KNOCKBACK) {
                attacker->SetState(EntityState::WALK);
            }
        }
    }
}