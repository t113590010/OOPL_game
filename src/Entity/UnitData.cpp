#include "Entity/UnitData.hpp"

// ========================================================
// 💡 所有 Unit 的 Lv1 Base Template
// 真正戰鬥數值請交給 LevelSystem 計算
// ========================================================

const std::unordered_map<UnitID, UnitStats>
UnitData::s_Stats = {

    // ====================================================
    // BASIC CAT
    // ====================================================

    {
        UnitID::CAT,

        {
            250,        // hp
            10.0f,      // speed
            20,         // damage
            140.0f,     // attackRange
            1.23f,      // attackCd
            50,         // cost
            2.0f,       // spawnCd
            1,          // rank
            3,          // kb
            false,      // isAoE
            UnitCategory::PLAYER,
            LevelType::BASIC_CAT,

            RESOURCE_DIR "/img/cat_1.png",
            RESOURCE_DIR "/img/cat_1_icon.png",
            RESOURCE_DIR "/img/cat/edi_f.png"
        }
    },

    {
        UnitID::LONG_LEG_CAT,

        {
            300,
            30.0f,
            25,
            200.0f,
            4.0f,
            200,
            4.0f,
            1,
            3,
            true,
            UnitCategory::PLAYER,
            LevelType::BASIC_CAT,

            RESOURCE_DIR "/img/longlegcat_1.png",
            RESOURCE_DIR "/img/longlegcat_1_icon (2).png",
            RESOURCE_DIR "/img/longlegcat/edi_f.png"

        }
    },

{
    UnitID::AXE_CAT,

    {
        500,        // hp
        12.0f,      // speed
        62,         // damage
        150.0f,     // attackRange
        0.9f,       // attackCd
        200,        // cost
        2.0f,       // spawnCd
        1,          // rank
        3,          // kb
        false,      // isAoE
        UnitCategory::PLAYER,
        LevelType::BASIC_CAT,

        RESOURCE_DIR "/img/axe_cat.png",
        RESOURCE_DIR "/img/axe_cat_icon.png",
        RESOURCE_DIR "/img/axecat/edi_f.png"
    }
},

    {
        UnitID::ninja_cat,

        {
            500,
            40.0f,
            100,
            100.0f,
            5.0f,
            500,
            10.0f,
            3,
            3,
            true,
            UnitCategory::PLAYER,
            LevelType::SUPER_RARE,

            RESOURCE_DIR "/img/ninja_cat_3.png",
            RESOURCE_DIR "/img/ninja_cat_3_icon.png",
            RESOURCE_DIR "/img/ninja_cat_3.png"//我打算放棄這隻角色 所以給他預設的
        }
    },

{
    UnitID::LongCat,

    {
        1000,       // hp
        8.0f,       // speed
        5,          // damage
        110.0f,     // attackRange
        2.23f,      // attackCd
        100,        // cost
        2.0f,       // spawnCd
        1,          // rank
        1,          // kb
        true,       // isAoE
        UnitCategory::PLAYER,
        LevelType::BASIC_CAT,

        RESOURCE_DIR "/img/LongCat.png",
        RESOURCE_DIR "/img/LongCatIcon.png",
        RESOURCE_DIR "/img/longcat/edi_f.png"
    }
},

{
    UnitID::CowCat,

    {
        1250,       // hp
        30.0f,      // speed
        32,         // damage
        140.0f,     // attackRange
        0.33f,      // attackCd
        500,        // cost
        2.0f,       // spawnCd
        1,          // rank
        5,          // kb
        false,      // isAoE
        UnitCategory::PLAYER,
        LevelType::BASIC_CAT,

        RESOURCE_DIR "/img/CowCat.png",
        RESOURCE_DIR "/img/CowCatIcon.png",
        RESOURCE_DIR "/img/cowcat/edi_f.png"
    }
},

{
    UnitID::FlyCat,

    {
        750,        // hp
        10.0f,      // speed
        350,        // damage
        170.0f,     // attackRange
        1.63f,      // attackCd
        650,        // cost
        2.0f,       // spawnCd
        1,          // rank
        4,          // kb
        true,       // isAoE
        UnitCategory::PLAYER,
        LevelType::BASIC_CAT,

        RESOURCE_DIR "/img/FlyCat.png",
        RESOURCE_DIR "/img/FlyCatIcon.png",
        RESOURCE_DIR "/img/flycat/edi_f.png"
    }
},

{
    UnitID::FishCat,

    {
        1750,       // hp
        10.0f,      // speed
        450,        // damage
        150.0f,     // attackRange
        1.77f,      // attackCd
        800,        // cost
        4.2f,       // spawnCd
        1,          // rank
        3,          // kb
        false,      // isAoE
        UnitCategory::PLAYER,
        LevelType::BASIC_CAT,

        RESOURCE_DIR "/img/FishCat.png",
        RESOURCE_DIR "/img/FishCatIcon.png",
        RESOURCE_DIR "/img/fishcat/edi_f.png"
    }
},

{
    UnitID::DinoCat,

    {
        2000,       // hp
        10.0f,      // speed
        875,        // damage
        400.0f,     // attackRange
        4.3f,       // attackCd
        1000,       // cost
        10.2f,      // spawnCd
        1,          // rank
        3,          // kb
        false,      // isAoE
        UnitCategory::PLAYER,
        LevelType::BASIC_CAT,

        RESOURCE_DIR "/img/DinoCat.png",
        RESOURCE_DIR "/img/DinoCatIcon.png",
        RESOURCE_DIR "/img/dinocat/edi_f.png"
    }
},

{
    UnitID::GaintCat,

    {
        2500,       // hp
        8.0f,       // speed
        700,        // damage
        150.0f,     // attackRange
        2.23f,      // attackCd
        1300,       // cost
        18.2f,      // spawnCd
        1,          // rank
        1,          // kb
        true,       // isAoE
        UnitCategory::PLAYER,
        LevelType::SUPER_RARE,

        RESOURCE_DIR "/img/GaintCat.png",
        RESOURCE_DIR "/img/GaintCatIcon.png",
        RESOURCE_DIR "/img/gaintcat/edi_f.png"
    }
},

    {
        UnitID::DogDoin,

        {
            1000,
            130.0f,
            100,
            20.0f,
            4.0f,
            200,
            6.0f,
            1,
            3,
            true,
            UnitCategory::PLAYER,
            LevelType::SUPER_RARE,

            RESOURCE_DIR "/img/DogDoin.png",
            RESOURCE_DIR "/img/DogDoinIcon.png",
            RESOURCE_DIR "/img/edi_f.png"
        }
    },

    {
        UnitID::Peashooter,

        {
            300,
            130.0f,
            100,
            600.0f,
            0.5f,
            200,
            6.0f,
            1,
            3,
            false,
            UnitCategory::PLAYER,
            LevelType::SUPER_RARE,

            RESOURCE_DIR "/img/Peashooter.png",
            RESOURCE_DIR "/img/PeashooterIdel.png",
            RESOURCE_DIR "/img/edi_f.png"
        }
    },

    {
        UnitID::Queen,

        {
            500,
            30.0f,
            300,
            50.0f,
            4.0f,
            200,
            6.0f,
            1,
            3,
            false,
            UnitCategory::PLAYER,
            LevelType::SUPER_RARE,

            RESOURCE_DIR "/img/Queen.png",
            RESOURCE_DIR "/img/QueenIcon.png",
            RESOURCE_DIR "/img/edi_f.png"
        }
    },

    // ====================================================
    // ENEMIES
    // ====================================================

{
    UnitID::BASIC_ENEMY,

    {
        90,         // hp
        5.0f,       // speed
        8,          // damage
        110.0f,     // attackRange
        1.57f,      // attackCd
        0,          // cost
        0.0f,       // spawnCd
        1,          // rank
        3,          // kb
        false,      // isAoE

        UnitCategory::ENEMY,
        LevelType::NONE,

        RESOURCE_DIR "/img/dog_1.png",
        ""
    }
},

    {
        UnitID::GAY,

        {
            300,
            60.0f,
            5,
            60.0f,
            0.5f,
            0,
            0.0f,
            1,
            2,
            true,
            UnitCategory::ENEMY,
            LevelType::NONE,

            RESOURCE_DIR "/img/GAY.png",
            ""
        }
    },

{
    UnitID::Snack,

    {
        100,        // hp
        8.0f,       // speed
        15,         // damage
        110.0f,     // attackRange
        1.23f,      // attackCd
        0,          // cost
        0.0f,       // spawnCd
        1,          // rank
        3,          // kb
        false,      // isAoE

        UnitCategory::ENEMY,
        LevelType::NONE,

        RESOURCE_DIR "/img/snack.png",
        ""
    }
},
{
    UnitID::p3,

    {
        200,        // hp
        10.0f,      // speed
        20,         // damage
        110.0f,     // attackRange
        1.0f,       // attackCd
        0,          // cost
        0.0f,       // spawnCd
        1,          // rank
        1,          // kb
        false,      // isAoE

        UnitCategory::ENEMY,
        LevelType::NONE,

        RESOURCE_DIR "/img/p3.png",
        ""
    }
},

{
    UnitID::bighead,

    {
        99999,      // hp
        1.0f,       // speed
        2000,       // damage
        340.0f,     // attackRange
        9.1f,       // attackCd
        0,          // cost
        0.0f,       // spawnCd
        1,          // rank
        2,          // kb
        true,       // isAoE

        UnitCategory::ENEMY,
        LevelType::NONE,

        RESOURCE_DIR "/img/big_head.png",
        "",
        RESOURCE_DIR "/img/edi_f.png"
    }
},

{
    UnitID::redPig,

    {
        1500,       // hp
        5.0f,       // speed
        120,        // damage
        150.0f,     // attackRange
        2.43f,      // attackCd
        0,          // cost
        0.0f,       // spawnCd
        1,          // rank
        2,          // kb
        true,       // isAoE

        UnitCategory::ENEMY,
        LevelType::NONE,

        RESOURCE_DIR "/img/redPig.png",
        ""
    }
},

{
    UnitID::hippo,

    {
        1000,       // hp
        4.0f,       // speed
        100,        // damage
        160.0f,     // attackRange
        2.23f,      // attackCd
        0,          // cost
        0.0f,       // spawnCd
        1,          // rank
        1,          // kb
        true,       // isAoE

        UnitCategory::ENEMY,
        LevelType::NONE,

        RESOURCE_DIR "/img/hippo.png",
        ""
    }
},
};