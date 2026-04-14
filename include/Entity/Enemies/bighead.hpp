#ifndef BIGHEAD_HPP
#define BIGHEAD_HPP
#include "../Entity.hpp"

class bighead : public Entity {
public:
    bighead(const Vector2& pos);
    void Update(float dt) override;
    void InitAnimation(const std::vector<SpriteFrame>& allFrames) override;

};
#endif