#include "Entity/Cats/LongLegCat.hpp"
#include "Entity/UnitFactory.hpp"
LongLegCat::LongLegCat(const Vector2& pos)
    : Entity(
        pos,
        UnitData::Get(UnitID::LONG_LEG_CAT).hp,           // 血量
        UnitData::Get(UnitID::LONG_LEG_CAT).speed,        // 跑速
        UnitData::Get(UnitID::LONG_LEG_CAT).damage,       // 傷害
        UnitData::Get(UnitID::LONG_LEG_CAT).attackRange,  // 攻擊距離
        UnitData::Get(UnitID::LONG_LEG_CAT).attackCd,     // 攻擊冷卻
        UnitData::Get(UnitID::LONG_LEG_CAT).cost,         // 價格
        UnitData::Get(UnitID::LONG_LEG_CAT).spawnCd,      // 產兵冷卻
        UnitData::Get(UnitID::LONG_LEG_CAT).rank,         // 階級
        UnitData::Get(UnitID::LONG_LEG_CAT).imgPath       // 圖片路徑
    ) {

    m_IsPlayerTeam = true;
    SetSize(50,100);
}

void LongLegCat::Update(float dt) {
    UpdateCooldown(dt);
    if (m_IsMoving) {
        m_Position.x -= m_Speed * dt;
    }
}