#include "System/BattleSystem.hpp"
#include "Entity/Cats/Peashooter.hpp"

#include <iostream>

void BattleSystem::Update(float dt, std::vector<std::shared_ptr<Entity>>& entities) {
    for (auto& attacker : entities) {
        if (!attacker->IsAlive()) continue;

        // 1. 系統負責更新實體的冷卻時間
        attacker->UpdateCooldown(dt);

        // ==========================================
        // 🟢 特殊處理：檢查天上飛的「豌豆」有沒有撞到人！
        // ==========================================
        if (attacker->IsProjectileUnit()) {
            // 把 Entity 轉型成 Peashooter，這樣才能拿它的豌豆
            auto peashooter = std::dynamic_pointer_cast<Peashooter>(attacker);
            if (peashooter) {
                for (auto& pea : peashooter->GetPeas()) {
                    if (!pea.active) continue;

                    // 檢查這顆豌豆有沒有撞到任何敵人
                    for (auto& target : entities) {
                        if (attacker == target || !target->IsAlive()) continue;
                        if (attacker->IsPlayerTeam() == target->IsPlayerTeam()) continue;

                        // 豌豆的碰撞判定 (拿敵人的 Hitbox 邊界來比對)
                        float oHalfW  = target->GetSize().x / 2.0f;
                        float oHalfH  = target->GetSize().y / 2.0f;

                        // ==========================================
                        // 🚀 新增這兩行：把子彈專用的判定範圍縮小！
                        // ==========================================
                        float hitBoxScaleX = 0.85f; // X 軸 (左右)：維持 85%，子彈要飛深一點才爆炸
                        float hitBoxScaleY = 1.50f; // Y 軸 (上下)：放大到 150% (1.5倍)，讓上下的判定變寬鬆！

                        oHalfW *= hitBoxScaleX;
                        oHalfH *= hitBoxScaleY;
                        // 算出來的邊界就會往內縮，子彈必須飛得更深才會觸發
                        float oLeft   = target->GetPosition().x - oHalfW;
                        float oRight  = target->GetPosition().x + oHalfW;
                        float oTop    = target->GetPosition().y + oHalfH;
                        float oBottom = target->GetPosition().y - oHalfH;
                        // 如果豌豆的中心點進入了敵人的框框裡面
                        if (pea.position.x >= oLeft && pea.position.x <= oRight &&
                            pea.position.y <= oTop && pea.position.y >= oBottom) {

                            // 💥 撞到了！造成傷害！
                            target->TakeDamage(attacker->GetDamage());
                            pea.active = false; // 豌豆碎裂銷毀

                            std::cout << "[Pea] Hit! Target HP: " << target->GetHP() << "\n";
                            break; // 一顆豌豆只能打一隻怪，打到就結束這顆豌豆的檢查
                            }
                    }
                }
            }
        }

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
                if (!attacker->IsProjectileUnit()) {
                    target->TakeDamage(attacker->GetDamage());
                }
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