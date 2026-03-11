#include "Entity/Enemies/Enemy.hpp"

Enemy::Enemy(const Vector2& pos)
    : Entity(pos, 120, 80.0f, 12, 90.0f, 1.2f, 0, 0.0f,RESOURCE_DIR"/img/enemy.png") {
    m_IsPlayerTeam = false;
}

void Enemy::Update(float dt) {
    UpdateCooldown(dt);
    if (m_IsMoving) {
        m_Position.x -= m_Speed * dt;
    }
}