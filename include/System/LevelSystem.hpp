#ifndef LEVEL_SYSTEM_HPP
#define LEVEL_SYSTEM_HPP

#include "Entity/UnitID.hpp"

enum class LevelType {
    NONE,
    BASIC_CAT,
    NORMAL_GACHA,
    SUPER_RARE,
    UBER_RARE,
    LEGEND_RARE,
    CRAZED,
    FIXED_50
};

struct UnitLevelData {
    int baseLevel = 1;
    int plusLevel = 0;
};

struct LevelGrowthRule {
    int maxBaseLevel;
    bool canPlusLevel;
    int maxPlusLevel;
    int softCapLevel;
    float preSoftCapGrowth;
    float postSoftCapGrowth;
};

class LevelSystem {
public:
    static int CalculateHP(UnitID id, const UnitLevelData& levelData);
    static int CalculateDamage(UnitID id, const UnitLevelData& levelData);
};

#endif