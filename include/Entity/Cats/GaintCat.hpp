#ifndef GAINTCAT_HPP
#define GAINTCAT_HPP
#include "../Entity.hpp"
#include "../Entity/UnitData.hpp" // 💡 引入神級資料庫

class GaintCat : public Entity {
public:
    GaintCat(const Vector2& pos);
    void Update(float dt) override;
    void InitAnimation(const std::vector<SpriteFrame>& allFrames) override;
private:
    static constexpr UnitID MY_ID = UnitID::GaintCat;
};
#endif