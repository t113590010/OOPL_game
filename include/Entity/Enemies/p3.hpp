#ifndef P3_HPP
#define P3_HPP
#include "../Entity.hpp"

class p3 : public Entity {
public:
    p3(const Vector2& pos);
    void Update(float dt) override;
    void InitAnimation(const std::vector<SpriteFrame>& allFrames) override;

};
#endif