#include "StageConfig.hpp"

namespace {

    float F(float frames) {
        return frames / 30.0f;
    }

}

const std::vector<StageData> STAGES = {

    {
        1,

        // =========================
        // Stage Info
        // =========================

        500,        // enemyBaseHP

        3000.0f,    // battlefieldLength

        3,          // maxEnemyCount

        1.1f,       // difficultyMultiplier

        1000,       // rewardXP

        0,          // backgroundID

        // =========================
        // Enemy Waves
        // =========================

        {

            // 開場狗仔
                {
                    UnitID::BASIC_ENEMY,

                    WaveTriggerType::TIME,

                    F(0),

                    1,

                    0.0f,

                    0.0f,

                    99999.0f,
                    99999.0f
                },

                // 第二波狗仔
                {
                    UnitID::BASIC_ENEMY,

                    WaveTriggerType::TIME,

                    F(600),

                    -1,

                    0.0f,
                    F(180),
                    F(300)
                },

                // 看板小妹
                {
                    UnitID::Snack,

                    WaveTriggerType::TIME,

                    F(27000),

                    -1,

                    0.0f,

                    F(27000),
                    F(27000)
                }
        }
    },

    {
        2,

        // =========================
        // Stage Info
        // =========================

        500,        // enemyBaseHP

        3000.0f,    // battlefieldLength

        4,          // maxEnemyCount

        1.1f,       // difficultyMultiplier

        1300,       // rewardXP

        0,          // backgroundID

        // =========================
        // Enemy Waves
        // =========================

        {

            // 狗仔
            {
                UnitID::BASIC_ENEMY,

                WaveTriggerType::TIME,

                F(0),

                -1,

                0.0f,

                F(180),
                F(300)
            },

            // 扭扭蛇
            {
                UnitID::Snack,

                WaveTriggerType::TIME,

                F(600),

                -1,

                0.0f,

                F(300),
                F(800)
            },

            // 看板小妹
            {
                UnitID::GAY,

                WaveTriggerType::TIME,

                F(27000),

                -1,

                0.0f,

                F(27000),
                F(27000)
            }
        }
    },

{
    3,

    // =========================
    // Stage Info
    // =========================

    500,        // enemyBaseHP

    4200.0f,    // battlefieldLength

    5,          // maxEnemyCount

    1.1f,       // difficultyMultiplier

    1600,       // rewardXP

    0,          // backgroundID

    // =========================
    // Enemy Waves
    // =========================

    {

        // 狗仔
            {
                UnitID::BASIC_ENEMY,

                WaveTriggerType::TIME,

                F(0),

                -1,

                0.0f,

                F(140),
                F(240)
            },

            // 扭扭蛇
            {
                UnitID::Snack,

                WaveTriggerType::TIME,

                F(600),

                -1,

                0.0f,

                F(300),
                F(800)
            },

            // 看板小妹
            {
                UnitID::Snack,

                WaveTriggerType::TIME,

                F(27000),

                -1,

                0.0f,

                F(27000),
                F(27000)
            }
    }
},

{
    4,

    // =========================
    // Stage Info
    // =========================

    500,        // enemyBaseHP

    4200.0f,    // battlefieldLength

    5,          // maxEnemyCount

    1.1f,       // difficultyMultiplier

    1900,       // rewardXP

    0,          // backgroundID

    // =========================
    // Enemy Waves
    // =========================

    {

        // 狗仔
            {
                UnitID::BASIC_ENEMY,

                WaveTriggerType::TIME,

                F(0),

                -1,

                0.0f,

                F(180),
                F(300)
            },

            // 扭扭蛇
            {
                UnitID::Snack,

                WaveTriggerType::TIME,

                F(0),

                -1,

                0.0f,

                F(300),
                F(800)
            },

            // 團結筷子幫
            {
                UnitID::p3,

                WaveTriggerType::TIME,

                F(1200),

                -1,

                0.0f,

                F(300),
                F(800)
            },

            // 看板小妹
            {
                UnitID::Snack,

                WaveTriggerType::TIME,

                F(27000),

                -1,

                0.0f,

                F(27000),
                F(27000)
            }
    }
},

{
    5,

    // =========================
    // Stage Info
    // =========================

    750,        // enemyBaseHP

    3600.0f,    // battlefieldLength

    6,          // maxEnemyCount

    1.1f,       // difficultyMultiplier

    2200,       // rewardXP

    0,          // backgroundID

    // =========================
    // Enemy Waves
    // =========================

    {

        // 狗仔
            {
                UnitID::BASIC_ENEMY,

                WaveTriggerType::TIME,

                F(0),

                -1,

                0.0f,

                F(180),
                F(300)
            },

            // 扭扭蛇
            {
                UnitID::Snack,

                WaveTriggerType::TIME,

                F(0),

                -1,

                0.0f,

                F(300),
                F(800)
            },

            // 團結筷子幫
            {
                UnitID::p3,

                WaveTriggerType::TIME,

                F(1200),

                -1,

                0.0f,

                F(300),
                F(800)
            },

            // 看板小妹
            {
                UnitID::GAY,

                WaveTriggerType::TIME,

                F(27000),

                -1,

                0.0f,

                F(27000),
                F(27000)
            }
    }
},
};