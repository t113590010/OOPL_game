#ifndef COLLISIONSYSTEM_HPP
#define COLLISIONSYSTEM_HPP

#include <vector>
#include <memory>
#include "Entity/Entity.hpp"

class CollisionSystem {
public:
    // 碰撞系統不需要知道時間 dt，只需要知道實體陣列
    void Update(std::vector<std::shared_ptr<Entity>>& entities);
};

#endif