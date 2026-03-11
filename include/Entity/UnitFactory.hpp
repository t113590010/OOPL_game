#ifndef UNIT_FACTORY_HPP
#define UNIT_FACTORY_HPP

#include <memory>
#include <string> // 💡 記得 include string
#include "Entity/Entity.hpp"
#include "Entity/UnitID.hpp"

class UnitFactory {
public:
    static std::shared_ptr<Entity> CreateUnit(UnitID id, const Vector2& pos);

    // 💡 新增：給 UI 系統查資料用的接口！
    static int GetUnitCost(UnitID id);
    static float GetUnitSpawnCooldown(UnitID id);
    static std::string GetUnitIconPath(UnitID id);
};

#endif