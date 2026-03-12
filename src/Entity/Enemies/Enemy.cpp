#include "Entity/Enemies/Enemy.hpp"

Enemy::Enemy(const Vector2& pos)
    : Entity(
        pos,
        UnitData::Get(UnitID::BASIC_ENEMY).hp,           // 150
        UnitData::Get(UnitID::BASIC_ENEMY).speed,        // -40.0f
        UnitData::Get(UnitID::BASIC_ENEMY).damage,       // 15
        UnitData::Get(UnitID::BASIC_ENEMY).attackRange,  // 60.0f
        UnitData::Get(UnitID::BASIC_ENEMY).attackCd,     // 1.5f
        UnitData::Get(UnitID::BASIC_ENEMY).cost,         // 0
        UnitData::Get(UnitID::BASIC_ENEMY).spawnCd,      // 0.0f
        UnitData::Get(UnitID::BASIC_ENEMY).rank,         // 1 (雖然敵人 UI 看不到，但建構子要填)
        UnitData::Get(UnitID::BASIC_ENEMY).imgPath       // "/img/enemy.png"
    ) {

    m_IsPlayerTeam = false; // 💡 敵人記得設為 false
}
void Enemy::Update(float dt) {
    UpdateCooldown(dt);
    if (m_IsMoving) {
        m_Position.x += m_Speed * dt;
    }
}