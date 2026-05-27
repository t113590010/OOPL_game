#ifndef UNIT_DATA_HPP
#define UNIT_DATA_HPP

#include <string>
#include <unordered_map>

#include "Entity/UnitID.hpp"
#include "GameConfig.hpp"
#include "System/LevelSystem.hpp"

// ========================================================
// 💡 Unit 陣營分類
// 用於 Battle / AI / Target 判斷
// ========================================================

enum class UnitCategory {

    PLAYER,

    ENEMY
};

// ========================================================
// 💡 所有單位的「基礎模板資料」
// 注意：
// 這裡全部都是 Lv1 Base Stat
// 真正戰鬥數值應該經過 LevelSystem 計算
// ========================================================

struct UnitStats {

    // =========================
    // Base Stats (Lv1)
    // =========================

    int hp;

    float speed;

    int damage;

    float attackRange;

    float attackCd;

    int cost;

    float spawnCd;

    int rank;

    int kb;

    bool isAoE;

    // =========================
    // Unit Category
    // =========================

    UnitCategory category;

    // =========================
    // Level Archetype
    // =========================

    LevelType levelType;

    // =========================
    // Resources
    // =========================

    std::string imgPath;

    std::string iconPath; // UI 專用方形頭像
};

// ========================================================
// 💡 全域 Unit 資料中心
// ========================================================

class UnitData {
public:

    static const UnitStats& Get(UnitID id) {

        // ============================================
        // 防呆 fallback
        // ============================================

        if (s_Stats.find(id) == s_Stats.end()) {

            static const UnitStats defaultStats = {

                // =========================
                // Base Stats
                // =========================

                1,          // hp
                0.0f,       // speed
                0,          // damage
                0.0f,       // attackRange
                1.0f,       // attackCd
                9999,       // cost
                99.0f,      // spawnCd
                1,          // rank
                3,          // kb
                false,      // isAoE

                // =========================
                // Category
                // =========================

                UnitCategory::ENEMY,

                // =========================
                // Level Type
                // =========================

                LevelType::NONE,

                // =========================
                // Resources
                // =========================

                "",
                ""
            };

            return defaultStats;
        }

        return s_Stats.at(id);
    }

private:

    static const std::unordered_map<
        UnitID,
        UnitStats
    > s_Stats;
};

#endif