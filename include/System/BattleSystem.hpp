#ifndef BATTLE_SYSTEM_HPP
#define BATTLE_SYSTEM_HPP

#include <vector>
#include <memory>
#include "Entity/Entity.hpp"

class BattleSystem {
public:
    // 💡 傳入 dt，由系統統一管理時間與戰鬥
    void Update(float dt, std::vector<std::shared_ptr<Entity>>& entities);
};

#endif