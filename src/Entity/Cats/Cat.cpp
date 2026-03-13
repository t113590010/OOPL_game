#include "Entity/Cats/Cat.hpp"
#include "Entity/UnitData.hpp" // 💡 引入神級資料庫

Cat::Cat(const Vector2& pos)
    : Entity(
        pos,
        UnitData::Get(UnitID::CAT).hp,           // 血量
        UnitData::Get(UnitID::CAT).speed,        // 跑速
        UnitData::Get(UnitID::CAT).damage,       // 傷害
        UnitData::Get(UnitID::CAT).attackRange,  // 攻擊距離
        UnitData::Get(UnitID::CAT).attackCd,     // 攻擊冷卻
        UnitData::Get(UnitID::CAT).cost,         // 價格
        UnitData::Get(UnitID::CAT).spawnCd,      // 產兵冷卻
        UnitData::Get(UnitID::CAT).rank,         // 階級
        UnitData::Get(UnitID::CAT).imgPath  ,     // 圖片路徑
        UnitData::Get(UnitID::CAT).kb       // 圖片路徑

    ) {

    m_IsPlayerTeam = true;
    SetAoE(UnitData::Get(UnitID::CAT).isAoE);

}

void Cat::Update(float dt) {
    UpdateCooldown(dt);
    if (UpdateKnockback(dt)) {
        return;
    }
    if (m_IsMoving) {
        m_Position.x -= m_Speed * dt;
    }
}