#include "System/LevelSystem.hpp"
#include "Entity/UnitData.hpp"

#include <unordered_map>
#include <algorithm>
#include <cmath>

// ========================================================
// 💡 Growth Rule Table
// ========================================================

namespace {

const std::unordered_map<
    LevelType,
    LevelGrowthRule
> LEVEL_RULES = {

    {
        LevelType::BASIC_CAT,

        {
            20,     // maxBaseLevel
            true,   // canPlusLevel
            90,     // maxPlusLevel

            60,     // softCapLevel

            0.20f,  // preSoftCapGrowth
            0.10f   // postSoftCapGrowth
        }
    },

    {
        LevelType::SUPER_RARE,

        {
            30,
            true,
            60,

            60,

            0.20f,
            0.10f
        }
    },

    {
        LevelType::CRAZED,

        {
            50,
            false,
            0,

            50,

            0.20f,
            0.10f
        }
    },

    {
        LevelType::FIXED_50,

        {
            50,
            false,
            0,

            50,

            0.20f,
            0.10f
        }
    },

    {
        LevelType::NORMAL_GACHA,

        {
            30,
            true,
            70,

            60,

            0.20f,
            0.10f
        }
    },

    {
        LevelType::UBER_RARE,

        {
            30,
            true,
            60,

            60,

            0.20f,
            0.10f
        }
    },

    {
        LevelType::LEGEND_RARE,

        {
            30,
            true,
            10,

            60,

            0.20f,
            0.10f
        }
    }
};

// ========================================================
// 💡 真正數值計算
// ========================================================

int CalculateScaledValue(
    int baseValue,
    LevelType levelType,
    const UnitLevelData& levelData
) {

    // 不吃等級
    if (levelType == LevelType::NONE) {
        return baseValue;
    }

    auto it = LEVEL_RULES.find(levelType);

    if (it == LEVEL_RULES.end()) {
        return baseValue;
    }

    const LevelGrowthRule& rule = it->second;

    // =====================================
    // Clamp Level
    // =====================================

    int baseLevel = std::clamp(
        levelData.baseLevel,
        1,
        rule.maxBaseLevel
    );

    int plusLevel = rule.canPlusLevel
        ? std::clamp(
            levelData.plusLevel,
            0,
            rule.maxPlusLevel
        )
        : 0;

    int realLevel =
        baseLevel + plusLevel;

    // =====================================
    // 💡 每級 individually round
    // =====================================

    float value =
        static_cast<float>(baseValue);

    float multiplier = 1.0f;

    for (int lv = 2; lv <= realLevel; lv++) {

        float growthRate;

        if (lv <= rule.softCapLevel) {

            growthRate =
                rule.preSoftCapGrowth;

        } else {

            growthRate =
                rule.postSoftCapGrowth;
        }

        multiplier += growthRate;
    }

    return static_cast<int>(
        std::round(
            baseValue * multiplier
        )
    );
}

} // namespace

// ========================================================
// 💡 HP 計算
// ========================================================

int LevelSystem::CalculateHP(
    UnitID id,
    const UnitLevelData& levelData
) {

    const auto& stats =
        UnitData::Get(id);

    // 敵人不吃 scaling
    if (stats.category == UnitCategory::ENEMY) {
        return stats.hp;
    }

    return CalculateScaledValue(
        stats.hp,
        stats.levelType,
        levelData
    );
}

// ========================================================
// 💡 Damage 計算
// ========================================================

int LevelSystem::CalculateDamage(
    UnitID id,
    const UnitLevelData& levelData
) {

    const auto& stats =
        UnitData::Get(id);

    // 敵人不吃 scaling
    if (stats.category == UnitCategory::ENEMY) {
        return stats.damage;
    }

    return CalculateScaledValue(
        stats.damage,
        stats.levelType,
        levelData
    );
}