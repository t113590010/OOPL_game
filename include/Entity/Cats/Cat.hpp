#ifndef CAT_HPP
#define CAT_HPP
#include "../Entity.hpp"

class Cat : public Entity {
public:
    Cat(const Vector2& pos);
    void Update(float dt) override;
    void InitAnimation(const std::vector<SpriteFrame>& allFrames) override;

};
#endif