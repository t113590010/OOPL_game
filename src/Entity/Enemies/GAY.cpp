#include "Entity/Enemies/GAY.hpp"

GAY::GAY(const Vector2& pos)
    : Entity(
        pos,
        UnitData::Get(UnitID::GAY).hp,           // 150
        UnitData::Get(UnitID::GAY).speed,        // -40.0f
        UnitData::Get(UnitID::GAY).damage,       // 15
        UnitData::Get(UnitID::GAY).attackRange,  // 60.0f
        UnitData::Get(UnitID::GAY).attackCd,     // 1.5f
        UnitData::Get(UnitID::GAY).cost,         // 0
        UnitData::Get(UnitID::GAY).spawnCd,      // 0.0f
        UnitData::Get(UnitID::GAY).rank,         // 1 (雖然敵人 UI 看不到，但建構子要填)
        UnitData::Get(UnitID::GAY).imgPath       // "/img/enemy.png"
    ) {

    m_IsPlayerTeam = false; // 💡 敵人記得設為 false
    SetSize(50,80);
}
void GAY::Update(float dt) {
    UpdateCooldown(dt);
    if (m_IsMoving) {
        m_Position.x += m_Speed * dt;
    }
}