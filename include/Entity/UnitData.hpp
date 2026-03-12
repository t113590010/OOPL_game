#ifndef UNIT_DATA_HPP
#define UNIT_DATA_HPP

#include <string>
#include <unordered_map>
#include "Entity/UnitID.hpp"
#include "GameConfig.hpp"

// 💡 1. 這是所有單位的「身家調查表」
struct UnitStats {
    int hp;
    float speed;
    int damage;
    float attackRange;
    float attackCd;
    int cost;
    float spawnCd;
    int rank;
    std::string imgPath;
    std::string iconPath; // UI 專用的方形圖
};

// 💡 2. 全域資料中心
class UnitData {
public:
    static const UnitStats& Get(UnitID id) {
        // 如果找不到，回傳一個防呆的預設值，保證不閃退！
        if (s_Stats.find(id) == s_Stats.end()) {
            static const UnitStats defaultStats = {1, 0, 0, 0, 1.0f, 9999, 99.0f, 1, "", ""};
            return defaultStats;
        }
        return s_Stats.at(id);
    }

private:
    static const std::unordered_map<UnitID, UnitStats> s_Stats;
};

#endif