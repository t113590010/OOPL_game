#ifndef LongLegCat_HPP
#define LongLegCat_HPP
#include "../Entity.hpp"
class LongLegCat : public Entity {
public:
    LongLegCat(const Vector2& pos);
    void Update(float dt) override;
    void InitAnimation(const std::vector<SpriteFrame>& allFrames) override;
};
#endif