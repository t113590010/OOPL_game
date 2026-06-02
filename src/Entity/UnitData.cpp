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
            250,
            40.0f,
            20,
            0.0f,
            2.0f,
            50,
            2.0f,
            1,
            2,
            false,
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
            500,
            50.0f,
            62,
            30.0f,
            3.0f,
            150,
            4.0f,
            1,
            3,
            false,
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
            1000,
            20.0f,
            5,
            10.0f,
            4.0f,
            100,
            6.0f,
            1,
            3,
            false,
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
            400,
            70.0f,
            20,
            20.0f,
            1.0f,
            200,
            6.0f,
            1,
            3,
            false,
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
            300,
            40.0f,
            40,
            20.0f,
            2.0f,
            200,
            6.0f,
            1,
            3,
            false,
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
            800,
            30.0f,
            200,
            40.0f,
            4.0f,
            200,
            6.0f,
            1,
            3,
            false,
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
            600,
            130.0f,
            150,
            40.0f,
            3.0f,
            200,
            6.0f,
            1,
            3,
            false,
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
            1000,
            130.0f,
            300,
            40.0f,
            4.0f,
            200,
            6.0f,
            1,
            3,
            true,
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
            150,
            40.0f,
            15,
            0.0f,
            3.5f,
            0,
            0.0f,
            1,
            2,
            false,

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
            250,
            60.0f,
            30,
            5.0f,
            3.0f,
            0,
            0.0f,
            1,
            2,
            false,
            UnitCategory::ENEMY,
            LevelType::NONE,

            RESOURCE_DIR "/img/snack.png",
            ""
        }
    },

    {
        UnitID::p3,

        {
            300,
            70.0f,
            40,
            10.0f,
            2.0f,
            0,
            0.0f,
            1,
            2,
            false,
            UnitCategory::ENEMY,
            LevelType::NONE,

            RESOURCE_DIR "/img/p3.png",
            ""
        }
    },

    {
        UnitID::bighead,

        {
            2000,
            20.0f,
            200,
            30.0f,
            10.0f,
            0,
            0.0f,
            1,
            3,
            true,
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
            1000,
            200.0f,
            1000,
            30.0f,
            10.0f,
            0,
            0.0f,
            1,
            3,
            true,
            UnitCategory::ENEMY,
            LevelType::NONE,

            RESOURCE_DIR "/img/redPig.png",
            ""
        }
    }
};