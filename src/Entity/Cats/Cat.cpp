#include "Entity/Cats/Cat.hpp"

Cat::Cat(const Vector2& pos)
    : Entity(pos, 100, 100.0f, 10, 80.0f, 1.0f, 50) {
    m_Image = std::make_shared<Util::Image>(RESOURCE_DIR"/img/cat.png");
    m_IsPlayerTeam = true;
}

void Cat::Update(float dt) {
    UpdateCooldown(dt);
    if (m_IsMoving) {
        m_Position.x += m_Speed * dt;
    }
}