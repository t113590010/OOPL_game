#include "Entity/UnitFactory.hpp"
#include "Entity/Cats/Cat.hpp"
#include "Entity/Cats/LongLegCat.hpp"
#include "Entity/Enemies/Enemy.hpp" // 💡 記得補上
#include "Entity/Enemies/GAY.hpp" // 💡 記得補上

#include "Entity/UnitData.hpp"     // 💡 核心：引入神級資料庫

// 💡 Getter 群現在變得超級乾淨，全部去 UnitData 拿
int UnitFactory::GetUnitCost(UnitID id) {
    return UnitData::Get(id).cost;
}

float UnitFactory::GetUnitSpawnCooldown(UnitID id) {
    return UnitData::Get(id).spawnCd;
}

int UnitFactory::GetUnitRank(UnitID id) {
    return UnitData::Get(id).rank;
}

std::string UnitFactory::GetUnitIconPath(UnitID id) {
    // 優先回傳頭像路徑，如果沒有頭像就拿場上圖片路徑
    auto& stats = UnitData::Get(id);
    return stats.iconPath.empty() ? stats.imgPath : stats.iconPath;
}

// 實體產兵區
std::shared_ptr<Entity> UnitFactory::CreateUnit(UnitID id, float x, float y, bool isPlayer) {
    std::shared_ptr<Entity> newUnit = nullptr;

    switch (id) {
        case UnitID::CAT:
            newUnit = std::make_shared<Cat>(Vector2{x, y});
            break;
        case UnitID::LONG_LEG_CAT:
            newUnit = std::make_shared<LongLegCat>(Vector2{x, y});
            break;
        case UnitID::BASIC_ENEMY:
            newUnit = std::make_shared<Enemy>(Vector2{x, y});
            break;
        case UnitID::GAY:
            newUnit = std::make_shared<GAY>(Vector2{x, y});
            break;
        default:
            return nullptr;
    }

    if (newUnit) {
        newUnit->SetTeam(isPlayer);

    }
    return newUnit;
}