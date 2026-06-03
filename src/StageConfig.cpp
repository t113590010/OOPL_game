#include "StageConfig.hpp"

namespace {

    float F(float frames) {
        return frames / 30.0f;
    }

}
const std::unordered_map<
    int,
    StageDisplayData
> STAGE_DISPLAY_DATA = {
    {1, {1, "台灣", 2}},
{2, {2, "香港", 3}},
{3, {3, "泰國", 4}},
{4, {4, "中國", 8}},
{5, {5, "蒙古", 10}},
{6, {6, "韓國", 10}},
{7, {7, "日本", 16}},
{8, {8, "澳洲", 12}},
{9, {9, "新加坡", 12}},
{10, {10, "馬爾地夫", 14}},
};
const std::vector<StageData> STAGES = {

    {
        1,          // stageID

        500,        // enemyBaseHP

        3000.0f,    // battlefieldLength

        3,          // maxEnemyCount

        1.1f,       // difficultyMultiplier

        1000,       // rewardXP

        0,          // backgroundID

        {
            {
                UnitID::BASIC_ENEMY,

                WaveTriggerType::TIME,

                0.0f,

                1,

                0.0f,

                0.0f,
                0.0f
            },

            {
                UnitID::BASIC_ENEMY,

                WaveTriggerType::TIME,

                600.0f,

                -1,

                0.0f,

                180.0f,
                300.0f
            }
        }
    },
{
    2,

    500,

    3000,

    4,

    1.1f,

    1300,

    0,

    {
            {
                UnitID::BASIC_ENEMY,

                WaveTriggerType::TIME,

                F(0),

                -1,

                0.0f,

                F(180),
                F(300)
            },

            {
                UnitID::Snack,

                WaveTriggerType::TIME,

                F(600),

                -1,

                0.0f,

                F(300),
                F(800)
            }
    }
},

{
    3,

    500,

    3400,

    30,

    1.1f,

    1600,

    0,

    {
            {
                UnitID::BASIC_ENEMY,

                WaveTriggerType::TIME,

                F(20),

                3,

                0.0f,

                F(800),
                F(800)
            },

            {
                UnitID::Snack,

                WaveTriggerType::TIME,

                F(300),

                3,

                0.0f,

                F(800),
                F(800)
            },

            {
                UnitID::p3,

                WaveTriggerType::TIME,

                F(500),

                2,

                0.0f,

                F(500),
                F(500)
            },

            {
                UnitID::Snack,

                WaveTriggerType::TIME,

                F(900),

                2,

                0.0f,

                F(400),
                F(400)
            }
    }
},

{
    4,

    500,

    4200,

    5,

    1.1f,

    1900,

    0,

    {
            {
                UnitID::BASIC_ENEMY,

                WaveTriggerType::TIME,

                F(0),

                -1,

                0.0f,

                F(180),
                F(300)
            },

            {
                UnitID::Snack,

                WaveTriggerType::TIME,

                F(0),

                -1,

                0.0f,

                F(300),
                F(800)
            },

            {
                UnitID::p3,

                WaveTriggerType::TIME,

                F(1200),

                -1,

                0.0f,

                F(300),
                F(800)
            }
    }
},

{
    5,

    750,      // enemyBaseHP

    3600,     // battlefieldLength

    6,        // maxEnemyCount

    1.1f,     // difficultyMultiplier

    2200,     // rewardXP

    0,        // backgroundID

    {
            {
                UnitID::BASIC_ENEMY,

                WaveTriggerType::TIME,

                F(0),

                -1,

                0.0f,

                F(180),
                F(300)
            },

            {
                UnitID::Snack,

                WaveTriggerType::TIME,

                F(0),

                -1,

                0.0f,

                F(300),
                F(800)
            },

            {
                UnitID::p3,

                WaveTriggerType::TIME,

                F(1200),

                -1,

                0.0f,

                F(300),
                F(800)
            }
    }
},

{
    6,

    750,      // enemyBaseHP

    4600,     // battlefieldLength

    7,        // maxEnemyCount

    1.1f,     // difficultyMultiplier

    2500,     // rewardXP

    1,        // backgroundID

    {
            {
                UnitID::BASIC_ENEMY,

                WaveTriggerType::TIME,

                F(0),

                -1,

                0.0f,

                F(180),
                F(300)
            },

            {
                UnitID::Snack,

                WaveTriggerType::TIME,

                F(0),

                -1,

                0.0f,

                F(300),
                F(800)
            },

            {
                UnitID::p3,

                WaveTriggerType::TIME,

                F(1200),

                -1,

                0.0f,

                F(300),
                F(800)
            }
    }
},

{
    7,

    2400,

    4800,

    6,

    1.1f,

    2800,

    2,

    {
            {
                UnitID::BASIC_ENEMY,

                WaveTriggerType::TIME,

                F(0),

                -1,

                0.0f,

                F(320),
                F(560)
            },

            {
                UnitID::Snack,

                WaveTriggerType::TIME,

                F(0),

                -1,

                0.0f,

                F(600),
                F(1600)
            },

            {
                UnitID::p3,

                WaveTriggerType::TIME,

                F(1200),

                -1,

                0.0f,

                F(600),
                F(1600)
            },

            {
                UnitID::p3,

                WaveTriggerType::TIME,

                F(0),

                6,

                0.0f,

                F(60),
                F(120)
            },

            {
                UnitID::hippo,

                WaveTriggerType::TIME,

                F(0),

                1,

                0.0f,

                0.0f,
                0.0f
            }
    }
},

{
    8,

    1500,     // enemyBaseHP

    3600,     // battlefieldLength

    5,        // maxEnemyCount

    1.1f,     // difficultyMultiplier

    3100,     // rewardXP

    0,        // backgroundID

    {
            {
                UnitID::BASIC_ENEMY,

                WaveTriggerType::TIME,

                F(0),

                -1,

                0.0f,

                F(180),
                F(300)
            },

            {
                UnitID::Snack,

                WaveTriggerType::TIME,

                F(300),

                -1,

                0.0f,

                F(300),
                F(800)
            },

            {
                UnitID::p3,

                WaveTriggerType::TIME,

                F(600),

                -1,

                0.0f,

                F(300),
                F(800)
            },

            {
                UnitID::p3,

                WaveTriggerType::TIME,

                F(0),

                20,

                0.0f,

                F(30),
                F(60)
            }
    }
},
{
    9,

    3000,     // enemyBaseHP

    6000,     // battlefieldLength

    10,       // maxEnemyCount

    1.1f,     // difficultyMultiplier

    3400,     // rewardXP

    0,        // backgroundID

    {
            {
                UnitID::BASIC_ENEMY,

                WaveTriggerType::TIME,

                F(0),

                -1,

                0.0f,

                F(120),
                F(300)
            },

            {
                UnitID::BASIC_ENEMY,

                WaveTriggerType::TIME,

                F(900),

                -1,

                0.0f,

                F(180),
                F(900)
            },

            {
                UnitID::Snack,

                WaveTriggerType::TIME,

                F(1800),

                -1,

                0.0f,

                F(300),
                F(900)
            },

            {
                UnitID::p3,

                WaveTriggerType::TIME,

                F(2700),

                -1,

                0.0f,

                F(300),
                F(900)
            },

            {
                UnitID::p3,

                WaveTriggerType::TIME,

                F(0),

                20,

                0.0f,

                F(2),
                F(4)
            }
    }
},

{
    10,

    2500,     // enemyBaseHP

    3800,     // battlefieldLength

    5,        // maxEnemyCount

    1.1f,     // difficultyMultiplier

    3700,     // rewardXP

    2,        // backgroundID

    {
            {
                UnitID::BASIC_ENEMY,

                WaveTriggerType::TIME,

                F(0),

                -1,

                0.0f,

                F(320),
                F(560)
            },

            {
                UnitID::Snack,

                WaveTriggerType::TIME,

                F(240),

                -1,

                0.0f,

                F(600),
                F(1600)
            },

            {
                UnitID::p3,

                WaveTriggerType::TIME,

                F(1200),

                -1,

                0.0f,

                F(600),
                F(1600)
            },

            {
                UnitID::p3,

                WaveTriggerType::TIME,

                F(0),

                6,

                0.0f,

                F(60),
                F(120)
            },

            {
                UnitID::redPig,

                WaveTriggerType::TIME,

                F(0),

                1,

                0.0f,

                0.0f,
                0.0f
            }
    }
},
};