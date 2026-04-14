#include "Entity/UnitData.hpp"

const std::unordered_map<UnitID, UnitStats> UnitData::s_Stats = {
    // 💡 格式：{ HP, Speed, Damage, Range, AtkCD, Cost, SpawnCD, Rank,kb, 場上圖片, UI頭像 }
    { UnitID::CAT,
      { 100, 40.0f, 10, 0.0f, 2.0f, 50, 2.0f, 1, 2,false,RESOURCE_DIR"/img/cat_1.png", RESOURCE_DIR"/img/cat_1_icon.png" }
    },

    { UnitID::LONG_LEG_CAT,
      { 300, 30.0f, 25, 200.0f, 4.0f, 200, 4.0f, 1, 3,true,RESOURCE_DIR"/img/longlegcat_1.png", RESOURCE_DIR"/img/longlegcat_1_icon.png" }
    },
  {
    UnitID::AXE_CAT,
      { 200, 50.0f, 55, 30.0f, 3.0f, 150, 4.0f, 1, 3,false,RESOURCE_DIR"/img/axe_cat.png", RESOURCE_DIR"/img/斧頭貓.png" }

  },
    // 💡 把敵人也寫進來，你的 SpawnSystem 就可以直接拿數值了！
    { UnitID::BASIC_ENEMY,
      { 150, 40.0f, 15, 0.0f, 3.5f, 0, 0.0f, 1, 2,false,RESOURCE_DIR"/img/dog_1.png", "" }
    },
  { UnitID::GAY,
    { 300, 60.0f, 5, 60.0f, 0.5f, 0, 0.0f, 1,2,true, RESOURCE_DIR"/img/GAY.png", "" }
  },
  { UnitID::Snack,
    { 250, 60.0f, 30, 5.0f, 3.0f, 0, 0.0f, 1,2,false, RESOURCE_DIR"/img/snack.png", "" }
  },
{ UnitID::p3,
  { 300, 70.0f, 40, 10.0f, 2.0f, 0, 0.0f, 1,2,false, RESOURCE_DIR"/img/p3.png", "" }
},
{ UnitID::bighead,
{ 2000, 20.0f, 200, 30.0f, 10.0f, 0, 0.0f, 1,3,true, RESOURCE_DIR"/img/big_head.png", "" }
},

{ UnitID::ninja_cat,
{ 1000, 40.0f, 100, 100.0f, 5.0f, 500, 10.0f, 3,3,true, RESOURCE_DIR"/img/ninja_cat_3.png", RESOURCE_DIR"/img/ninja_cat_3_icon.png" }
}

};