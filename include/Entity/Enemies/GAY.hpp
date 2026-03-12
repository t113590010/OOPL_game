#ifndef GAY_HPP
#define GAY_HPP
#include "../Entity.hpp"

class GAY : public Entity {
public:
    GAY(const Vector2& pos);
    void Update(float dt) override;
};
#endif