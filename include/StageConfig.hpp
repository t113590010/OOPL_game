#ifndef STAGECONFIG_HPP
#define STAGECONFIG_HPP

#include <vector>
#include <string>
#include "Entity/UnitID.hpp"

enum class WaveTriggerType {
    TIME,
    ENEMY_HP
};

struct EnemyWave {

    UnitID id;

    WaveTriggerType triggerType;

    float triggerValue;

    int totalCount; // -1 = 無限

    float firstDelay;

    float repeatMin;
    float repeatMax;
    float nextSpawnDelay = 0.0f;
    // runtime
    bool activated = false;

    bool completed = false;

    int spawnedCount = 0;

    float timer = 0.0f;
};

struct StageData {

    int stageID;

    // =========================
    // 關卡資訊
    // =========================

    int enemyBaseHP;

    float battlefieldLength;

    int maxEnemyCount;

    float difficultyMultiplier;

    int rewardXP;

    int backgroundID;

    // =========================
    // enemy waves
    // =========================

    std::vector<EnemyWave> waves;
};

extern const std::vector<StageData> STAGES;

#endif