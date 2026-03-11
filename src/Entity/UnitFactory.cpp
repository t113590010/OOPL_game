#include "Entity/UnitFactory.hpp"
#include <map>

// 💡 必須包含這些，編譯器才認識具體的貓和敵人
#include "Entity/Cats/Cat.hpp"
#include "Entity/Cats/LongLegCat.hpp"
#include "Entity/Enemies/Enemy.hpp"

// 建立靜態快取，避免重複生成物件浪費效能
static std::map<UnitID, std::shared_ptr<Entity>> s_SampleCache;

// 輔助函式：取得或建立樣本
static std::shared_ptr<Entity> GetSample(UnitID id) {
    if (id == UnitID::NONE) return nullptr;

    // 如果沒生過，就生一隻存在展示櫃 (map) 裡
    if (s_SampleCache.find(id) == s_SampleCache.end()) {
        s_SampleCache[id] = UnitFactory::CreateUnit(id, {0,0});
    }
    return s_SampleCache[id];
}

int UnitFactory::GetUnitCost(UnitID id) {
    auto sample = GetSample(id);
    return sample ? sample->GetUnitCost() : 0;
}

float UnitFactory::GetUnitSpawnCooldown(UnitID id) {
    auto sample = GetSample(id);
    return sample ? sample->GetSpawnCooldown() : 0.0f;
}

std::string UnitFactory::GetUnitIconPath(UnitID id) {
    auto sample = GetSample(id);
    return sample ? sample->GetImgPath() : "";
}

// 實際戰鬥時生成的單位
std::shared_ptr<Entity> UnitFactory::CreateUnit(UnitID id, const Vector2& pos) {
    switch (id) {
        case UnitID::BASIC_CAT:    return std::make_shared<Cat>(pos);
        case UnitID::LONG_LEG_CAT: return std::make_shared<LongLegCat>(pos);
        case UnitID::BASIC_ENEMY:  return std::make_shared<Enemy>(pos);
        default: return nullptr;
    }
}