#ifndef redPig_HPP
#define redPig_HPP
#include "../Entity.hpp"

class redPig : public Entity {
public:
    redPig(const Vector2& pos);
    void Update(float dt) override;
    void InitAnimation(const std::vector<SpriteFrame>& allFrames) override;
private:
    static constexpr UnitID MY_ID = UnitID::redPig;
};
#endif