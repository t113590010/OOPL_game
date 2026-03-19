#include "Entity/Cats/AxeCat.hpp"
#include "Entity/UnitFactory.hpp"
AxeCat::AxeCat(const Vector2& pos)
    : Entity(
        pos,
        UnitData::Get(UnitID::AXE_CAT).hp,           // 血量
        UnitData::Get(UnitID::AXE_CAT).speed,        // 跑速
        UnitData::Get(UnitID::AXE_CAT).damage,       // 傷害
        UnitData::Get(UnitID::AXE_CAT).attackRange,  // 攻擊距離
        UnitData::Get(UnitID::AXE_CAT).attackCd,     // 攻擊冷卻
        UnitData::Get(UnitID::AXE_CAT).cost,         // 價格
        UnitData::Get(UnitID::AXE_CAT).spawnCd,      // 產兵冷卻
        UnitData::Get(UnitID::AXE_CAT).rank,         // 階級
        UnitData::Get(UnitID::AXE_CAT).imgPath ,      // 圖片路徑
        UnitData::Get(UnitID::AXE_CAT).kb   // 擊退

    ) {
    SetAoE(UnitData::Get(UnitID::AXE_CAT).isAoE);

    m_IsPlayerTeam = true;
    SetSize(80,130);
}

void AxeCat::Update(float dt) {
    UpdateCooldown(dt);
    UpdateAnimation(dt);
    if (UpdateKnockback(dt)) {
        return;
    }
    if (m_IsMoving && m_CurrentState == EntityState::WALK) {
        m_Position.x -= m_Speed * dt;
    }
}