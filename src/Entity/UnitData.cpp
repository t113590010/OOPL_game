#include "Entity/UnitData.hpp"

const std::unordered_map<UnitID, UnitStats> UnitData::s_Stats = {
    // 💡 格式：{ HP, Speed, Damage, Range, AtkCD, Cost, SpawnCD, Rank,kb, 場上圖片, UI頭像 }
    { UnitID::CAT,
      { 100, 80.0f, 10, 0.0f, 1.0f, 50, 2.0f, 1, 2,false,RESOURCE_DIR"/img/cat.png", RESOURCE_DIR"/img/cat.png" }
    },

    { UnitID::LONG_LEG_CAT,
      { 80, 60.0f, 25, 250.0f, 2.5f, 200, 4.0f, 2, 3,true,RESOURCE_DIR"/img/longlegcat.png", RESOURCE_DIR"/img/longlegcat.png" }
    },

    // 💡 把敵人也寫進來，你的 SpawnSystem 就可以直接拿數值了！
    { UnitID::BASIC_ENEMY,
      { 150, 40.0f, 15, 0.0f, 1.5f, 0, 0.0f, 1, 2,false,RESOURCE_DIR"/img/enemy.png", "" }
    },
  { UnitID::GAY,
    { 300, 60.0f, 5, 60.0f, 0.5f, 0, 0.0f, 1,2,true, RESOURCE_DIR"/img/GAY.png", "" }
  }
};