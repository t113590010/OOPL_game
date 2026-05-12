#ifndef DOGDOIN_HPP
#define DOGDOIN_HPP
#include "../Entity.hpp"
#include "../Entity/UnitData.hpp" // 💡 引入神級資料庫

class DogDoin : public Entity {
public:
    DogDoin(const Vector2& pos);
    void Update(float dt) override;
    void InitAnimation(const std::vector<SpriteFrame>& allFrames) override;
private:
    static constexpr UnitID MY_ID = UnitID::DogDoin;
};
#endif