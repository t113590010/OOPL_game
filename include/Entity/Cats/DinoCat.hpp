#ifndef DINOCAT_HPP
#define DINOCAT_HPP
#include "../Entity.hpp"
#include "../Entity/UnitData.hpp" // 💡 引入神級資料庫

class DinoCat : public Entity {
public:
    DinoCat(const Vector2& pos);
    void Update(float dt) override;
    void InitAnimation(const std::vector<SpriteFrame>& allFrames) override;
private:
    static constexpr UnitID MY_ID = UnitID::DinoCat;
};
#endif