#ifndef SNACK_HPP
#define SNACK_HPP
#include "../Entity.hpp"

class Snack : public Entity {
public:
    Snack(const Vector2& pos);
    void Update(float dt) override;
    void InitAnimation(const std::vector<SpriteFrame>& allFrames) override;

};
#endif