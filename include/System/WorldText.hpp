#ifndef WORLD_TEXT_HPP
#define WORLD_TEXT_HPP

#include "Util/GameObject.hpp"
#include "Util/Text.hpp"
#include "Util/Color.hpp"
#include <string>
#include <memory>


class WorldText : public Util::GameObject {
public:
    WorldText(float worldX, float worldY, const std::string& text)
        : GameObject(
            std::make_unique<Util::Text>(RESOURCE_DIR"/Font/arial.ttf", 25, text, Util::Color::FromName(Util::Colors::WHITE)),
            100), // 💡 記取教訓，Z-Index 設為絕對安全的 100
          m_WorldX(worldX), m_WorldY(worldY) {

        m_Visible = true; // 💡 強制確保顯示！
    }

    void Draw(float cameraX) {
        m_Transform.translation.x = m_WorldX - cameraX;
        m_Transform.translation.y = m_WorldY;
        Util::GameObject::Draw();
    }

    // 💡 新增這個功能：讓外部可以更新文字內容
    void UpdateText(const std::string& newText) {
        auto temp = std::dynamic_pointer_cast<Util::Text>(m_Drawable);
        if (temp) {
            temp->SetText(newText);
        }
    }

private:
    float m_WorldX;
    float m_WorldY;
};

#endif