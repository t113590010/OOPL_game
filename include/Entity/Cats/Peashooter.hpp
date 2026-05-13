#ifndef PEASHOOTER_HPP
#define PEASHOOTER_HPP
#include "../Entity.hpp"
#include "../Entity/UnitData.hpp" // 💡 引入神級資料庫
#include <algorithm> // 💡 為了等下清理飛出畫面的豌豆
class Peashooter : public Entity {
public:
    Peashooter(const Vector2& pos);
    void Update(float dt) override;
    void InitAnimation(const std::vector<SpriteFrame>& allFrames) override;
    void Draw(float cameraX = 0.0f) override;
    struct Pea {
        Vector2 position;
        float speed = 600.0f;
        bool active = true;
    };
    std::vector<Pea>& GetPeas() { return m_Peas; }
private:
    static constexpr UnitID MY_ID = UnitID::Peashooter;

    std::vector<Pea> m_Peas;                 // 裝目前場上所有豌豆的陣列
    std::shared_ptr<Util::Image> m_PeaImage; // 豌豆的圖片
    Util::GameObject m_PeaRenderer;          // 豌豆專用的渲染器
    bool m_HasFired = false;                 // 攻擊開關：防止一幀噴出一百顆豌豆
};
#endif