#ifndef ENEMY_HPP
#define ENEMY_HPP
#include "../Entity.hpp"

class Enemy : public Entity {
public:
    Enemy(const Vector2& pos);
    void Update(float dt) override;
};
#endif