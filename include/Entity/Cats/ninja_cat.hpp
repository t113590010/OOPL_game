#ifndef NINJACAT_HPP
#define NINJACAT_HPP
#include "../Entity.hpp"
class ninja_cat : public Entity {
public:
    ninja_cat(const Vector2& pos);
    void Update(float dt) override;
    void InitAnimation(const std::vector<SpriteFrame>& allFrames) override;
};
#endif