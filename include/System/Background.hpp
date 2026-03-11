#ifndef BACKGROUND_HPP
#define BACKGROUND_HPP

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "Core/Context.hpp"
#include "GameConfig.hpp" // 💡 引入設定，我們要拿基地座標！
#include <string>

class Background : public Util::GameObject {
public:
    Background(const std::string& imagePath)
        : GameObject(
            std::make_unique<Util::Image>(imagePath),
            -10) {

        m_Visible = true;

        auto context = Core::Context::GetInstance();
        float windowHeight = context->GetWindowHeight();

        // 算出等比例完美縮放
        float perfectScale = windowHeight / 2084.0f;
        m_Transform.scale = {perfectScale, perfectScale};
        m_Transform.translation.y = 0.0f;

        // 算出每一塊背景圖的真實寬度
        m_ScaledWidth = 1080.0f * perfectScale;

        // 💡 計算鋪磚數量：從左邊基地到右邊基地的總長度，除以圖片寬度
        // 多加 2000.0f 是為了當作兩邊的緩衝(出血邊)，避免攝影機看到盡頭的黑邊
        float battleWidth = GameConfig::ENEMY_BASE_X - GameConfig::PLAYER_BASE_X + 2000.0f;
        m_TileCount = (int)(battleWidth / m_ScaledWidth) + 1; // 算總共需要幾張
    }

    void Draw(float cameraX) {
        // 💡 設定第一張圖的起點 (從玩家基地再往左邊退一點點開始鋪)
        float startX = GameConfig::PLAYER_BASE_X - 1000.0f;

        // 💡 迴圈鋪磚：從左到右一路畫過去
        for (int i = 0; i < m_TileCount; ++i) {

            // 這塊磚在真實世界的位置
            float worldX = startX + (i * m_ScaledWidth);

            // 螢幕位置 = 真實位置 - 攝影機位置
            // (如果之後你想加視差效果，可以把 - cameraX 改成 - cameraX * 0.5f)
            m_Transform.translation.x = worldX - cameraX;

            Util::GameObject::Draw();
        }
    }

private:
    float m_ScaledWidth;
    int m_TileCount; // 紀錄需要鋪幾塊磚
};

#endif