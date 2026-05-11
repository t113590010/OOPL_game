#ifndef LONGCAT_HPP
#define LONGCAT_HPP
#include "../Entity.hpp"

class LongCat : public Entity {
public:
    LongCat(const Vector2& pos);
    void Update(float dt) override;
    void InitAnimation(const std::vector<SpriteFrame>& allFrames) override;

};
#endif