#ifndef AXECAT_HPP
#define AXECAT_HPP
#include "../Entity.hpp"

class AxeCat : public Entity {
public:
    AxeCat(const Vector2& pos);
    void Update(float dt) override;
};
#endif