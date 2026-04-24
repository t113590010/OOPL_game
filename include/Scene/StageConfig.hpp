#ifndef STAGE_CONFIG_HPP
#define STAGE_CONFIG_HPP

#include <vector>
#include "Entity/UnitID.hpp"

struct EnemyWave {
    UnitID id;
    float cooldown;
};

struct StageData {
    int id;
    std::vector<EnemyWave> enemyConfig;
};

#endif