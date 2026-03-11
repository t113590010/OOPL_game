#ifndef UI_TEXT_HPP
#define UI_TEXT_HPP

#include "Util/GameObject.hpp"
#include "Util/Text.hpp"
#include "Util/Color.hpp"
#include "Core/Context.hpp"
#include <string>

// 通用的螢幕 UI 文字類別 (釘死在螢幕絕對比例位置)
class UIText : public Util::GameObject {
public:
    // 💡 建構子大進化：傳入比例 X, 比例 Y, 初始文字, 字體大小, 顏色
    UIText(float scaleX, float scaleY, const std::string& text, int fontSize, const Util::Color& color)
        : GameObject(
             std::make_unique<Util::Text>(RESOURCE_DIR"/Font/arial.ttf", fontSize, text, color),
             100) {

        auto context = Core::Context::GetInstance();
        float halfW = context->GetWindowWidth() / 2.0f;
        float halfH = context->GetWindowHeight() / 2.0f;

        m_Transform.translation = { halfW * scaleX, halfH * scaleY };
        m_LastText = text; // 記錄初始文字
        m_Visible = true;
    }

    // 💡 通用的更新方法：你給我什麼字，我就顯示什麼字
    void UpdateText(const std::string& newText) {
        // 一樣保留「髒標記 (Dirty Flag)」的優化邏輯：字不一樣才重畫
        if (newText != m_LastText) {
            m_LastText = newText;
            auto temp = std::dynamic_pointer_cast<Util::Text>(m_Drawable);
            if (temp) {
                temp->SetText(newText);
            }
        }
    }

private:
    std::string m_LastText; // 改用字串來比對是否需要更新
};

#endif