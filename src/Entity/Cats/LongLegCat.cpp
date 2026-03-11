#include "Entity/Cats/LongLegCat.hpp"

LongLegCat::LongLegCat(const Vector2& pos)
    : Entity(pos, 80, 60.0f, 25, 250.0f, 2.5f, 200,4.0f,RESOURCE_DIR"/img/longlegcat.png") {

    m_IsPlayerTeam = true;
}

void LongLegCat::Update(float dt) {
    UpdateCooldown(dt);
    if (m_IsMoving) {
        m_Position.x += m_Speed * dt;
    }
}