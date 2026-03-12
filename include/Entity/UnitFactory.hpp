#ifndef UNIT_FACTORY_HPP
#define UNIT_FACTORY_HPP

#include <memory>
#include <string>
#include "Entity/UnitID.hpp"
#include "Entity.hpp"



class UnitFactory {
public:
    // 產兵邏輯維持原樣
    static std::shared_ptr<Entity> CreateUnit(UnitID id, float x, float y, bool isPlayer);

    // 💡 這些 Getter 改成直接去 UnitData 抓資料
    static int GetUnitCost(UnitID id);
    static float GetUnitSpawnCooldown(UnitID id);
    static int GetUnitRank(UnitID id);
    static std::string GetUnitIconPath(UnitID id);

    // 不需要 private 的 s_Configs 了，因為 UnitData 就是我們的 Config
};

#endif