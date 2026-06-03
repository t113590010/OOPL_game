#ifndef hippo_HPP
#define hippo_HPP
#include "../Entity.hpp"

class hippo : public Entity {
public:
    hippo(const Vector2& pos);
    void Update(float dt) override;
    void InitAnimation(const std::vector<SpriteFrame>& allFrames) override;

};
#endif