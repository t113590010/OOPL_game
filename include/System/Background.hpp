#ifndef BACKGROUND_HPP
#define BACKGROUND_HPP

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "Core/Context.hpp"
#include "GameConfig.hpp"
#include <string>
#include <cmath> // 💡 引入 abs

class Background : public Util::GameObject {
public:
    Background(const std::string& imagePath)
        : GameObject(
            std::make_shared<Util::Image>(imagePath), // 💡 建議用 shared_ptr 比較統一
            -10) {

        m_Visible = true;
        auto context = Core::Context::GetInstance();
        float windowHeight = (float)context->GetWindowHeight();

        // 假設原圖高度是 2084.0f
        float perfectScale = windowHeight / 2084.0f;
        m_Transform.scale = {perfectScale, perfectScale};
        m_Transform.translation.y = 0.0f;

        m_ScaledWidth = 1080.0f * perfectScale;

        // 💡 修正：用絕對值算出總戰鬥寬度，確保 m_TileCount 是正數
        float battleWidth = std::abs(GameConfig::ENEMY_BASE_X - GameConfig::PLAYER_BASE_X) + 4000.0f;
        m_TileCount = (int)(battleWidth / m_ScaledWidth) + 1;

        // 💡 修正：找出最左邊的那個點作為起點 (不管是誰在左邊)
        m_StartX = std::min(GameConfig::ENEMY_BASE_X, GameConfig::PLAYER_BASE_X) - 2000.0f;
    }

    void Draw(float cameraX) {
        for (int i = 0; i < m_TileCount; ++i) {
            float worldX = m_StartX + (i * m_ScaledWidth);

            // 💡 繪製位置 = 世界座標 - 相機座標
            m_Transform.translation.x = worldX - cameraX;
            Util::GameObject::Draw();
        }
    }

private:
    float m_ScaledWidth;
    float m_StartX; // 💡 存起來，不用每次 Draw 都算
    int m_TileCount;
};

#endif