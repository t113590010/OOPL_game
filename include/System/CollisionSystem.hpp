#ifndef COLLISION_SYSTEM_HPP
#define COLLISION_SYSTEM_HPP

#include <vector>
#include <memory>
#include "Entity/Entity.hpp"

class CollisionSystem {
public:
    // 只需要傳入實體清單
    void Update(std::vector<std::shared_ptr<Entity>>& entities);
};

#endif