#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "Util/Color.hpp"
#include "Core/Context.hpp"
#include <string>
#include <functional>
#include <memory>
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
// 🚀 引入你寫好的神級 UI 類別 (如果路徑不同請自行微調)
#include "System/UIText.hpp"

// ❌ 已經把那坨有 Bug 的 ButtonText 徹底刪除了

class Button : public Util::GameObject {
public:
    Button(float pos_ratio_x, float pos_ratio_y, float width, float height, const std::string& img_path, const std::string& text_content, int font_size, const Util::Color& text_color)
        : Button(pos_ratio_x, pos_ratio_y, width, height, img_path, text_content, font_size, text_color, pos_ratio_x, pos_ratio_y) {
    }

    // 💡 2. 新增的「全參數」建構子 (負責所有真正的初始化工作)
    Button(float pos_ratio_x, float pos_ratio_y, float width, float height, const std::string& img_path, const std::string& text_content, int font_size, const Util::Color& text_color, float text_pos_ratio_x, float text_pos_ratio_y)
        : GameObject(std::make_unique<Util::Image>(img_path), 30) {

        auto context = Core::Context::GetInstance();
        float halfW = context->GetWindowWidth() / 2.0f;
        float halfH = context->GetWindowHeight() / 2.0f;

        m_Transform.translation = { halfW * pos_ratio_x, halfH * pos_ratio_y };

        m_Width = width;
        m_Height = height;

        auto tempImg = std::dynamic_pointer_cast<Util::Image>(m_Drawable);
        if (tempImg) {
            glm::vec2 originalSize = tempImg->GetSize();
            float scaleX = m_Width / originalSize.x;
            float scaleY = m_Height / originalSize.y;
            float finalScale = std::max(scaleX, scaleY);
            m_Transform.scale = { finalScale, finalScale };
        }

        // 🚀 關鍵改動：這裡餵給 UIText 的是新傳入的 text_pos_ratio 參數
        m_TextGO = std::make_shared<UIText>(
            text_pos_ratio_x,
            text_pos_ratio_y,
            text_content,
            font_size,
            text_color
        );

        m_TextGO->SetZIndex(35);

        m_CurrentContent = text_content;
        m_Visible = true;
        m_IsPressed = false;
    }
    // Button(float pos_ratio_x, float pos_ratio_y, float width, float height, const std::string& img_path, const std::string& text_content, int font_size, const Util::Color& text_color)
    //     : GameObject(std::make_unique<Util::Image>(img_path), 100) {
    //
    //     auto context = Core::Context::GetInstance();
    //     float halfW = context->GetWindowWidth() / 2.0f;
    //     float halfH = context->GetWindowHeight() / 2.0f;
    //
    //     m_Transform.translation = { halfW * pos_ratio_x, halfH * pos_ratio_y };
    //
    //     m_Width = width;
    //     m_Height = height;
    //
    //     auto tempImg = std::dynamic_pointer_cast<Util::Image>(m_Drawable);
    //     if (tempImg) {
    //         glm::vec2 originalSize = tempImg->GetSize();
    //         float scaleX = m_Width / originalSize.x;
    //         float scaleY = m_Height / originalSize.y;
    //         float finalScale = std::max(scaleX, scaleY);
    //         m_Transform.scale = { finalScale, finalScale };
    //     }
    //
    //     // 🚀 降維打擊：直接呼叫你的 UIText！
    //     // 把 Button 接到的 pos_ratio 直接餵給 UIText 的 scale
    //     m_TextGO = std::make_shared<UIText>(
    //         pos_ratio_x,
    //         pos_ratio_y,
    //         text_content,
    //         font_size,
    //         text_color
    //     );
    //
    //     // 💡 關鍵防呆：UIText 預設是 100，我們強制拉高到 101，保證壓在按鈕底圖之上！
    //     m_TextGO->SetZIndex(80);
    //
    //     m_CurrentContent = text_content;
    //     m_Visible = true;
    //     m_IsPressed = false;
    // }

    // 🚀 新增一個可以動態換底圖的函數
    // void SetImage(const std::string& img_path) {
    //     SetDrawable(std::make_shared<Util::Image>(img_path));
    // }
    void SetImage(const std::string& img_path) {

        auto img = std::make_shared<Util::Image>(img_path);

        SetDrawable(img);

        glm::vec2 originalSize = img->GetSize();

        // 🚀 防止圖片還沒載入完成
        if (originalSize.x <= 0 || originalSize.y <= 0)
            return;

        // 🚀 正確縮放
        float scaleX = m_Width / originalSize.x;
        float scaleY = m_Height / originalSize.y;

        // 🚀 不再強制等比
        m_Transform.scale = { scaleX, scaleY };
    }
    void UpdateText(const std::string& new_content) {
        if (new_content != m_CurrentContent) {
            m_CurrentContent = new_content;
            if (m_TextGO) {
                m_TextGO->UpdateText(new_content); // 🚀 無腦呼叫 UIText 內建的更新
            }
        }
    }

    void SetOnClick(std::function<void()> callback) {
        m_OnClick = callback;
    }

    // void Update() {
    //     if (!m_Visible) return;
    //
    //     // UIText 在建構時已經算好螢幕絕對位置，且 Button 不會亂跑，所以不用 UpdatePosition 了
    //
    //     // 🚀 直接在內部抓取滑鼠座標與左鍵狀態
    //     glm::vec2 mouse_pos = Util::Input::GetCursorPosition();
    //     bool is_mouse_down = Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB);
    //
    //     // 以下碰撞判定邏輯完全維持你原本的寫法
    //     float halfWidth = m_Width / 2.0f;
    //     float halfHeight = m_Height / 2.0f;
    //
    //     float left = m_Transform.translation.x - halfWidth;
    //     float right = m_Transform.translation.x + halfWidth;
    //     float top = m_Transform.translation.y + halfHeight;
    //     float bottom = m_Transform.translation.y - halfHeight;
    //
    //     bool isHovered = (mouse_pos.x >= left && mouse_pos.x <= right &&
    //                       mouse_pos.y >= bottom && mouse_pos.y <= top);
    //
    //     if (is_mouse_down) {
    //         if (isHovered && !m_IsPressed) {
    //             if (m_OnClick) m_OnClick();
    //             m_IsPressed = true;
    //         }
    //     } else {
    //         m_IsPressed = false;
    //     }
    // }

    void Update() {
        if (!m_Visible) return;

        glm::vec2 mouse_pos = Util::Input::GetCursorPosition();
        bool is_mouse_down = Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB);

        // 🚀 取得目前圖片
        auto img = std::dynamic_pointer_cast<Util::Image>(m_Drawable);

        // 🚀 真正顯示出來的尺寸
        float drawWidth = m_Width;
        float drawHeight = m_Height;

        if (img) {
            drawWidth = img->GetSize().x * m_Transform.scale.x;
            drawHeight = img->GetSize().y * m_Transform.scale.y;
        }

        float halfWidth = drawWidth / 2.0f;
        float halfHeight = drawHeight / 2.0f;

        float left = m_Transform.translation.x - halfWidth;
        float right = m_Transform.translation.x + halfWidth;
        float top = m_Transform.translation.y + halfHeight;
        float bottom = m_Transform.translation.y - halfHeight;

        bool isHovered =
            (mouse_pos.x >= left && mouse_pos.x <= right &&
             mouse_pos.y >= bottom && mouse_pos.y <= top);

        if (is_mouse_down) {
            if (isHovered && !m_IsPressed) {
                if (m_OnClick) {
                    m_OnClick();
                }

                m_IsPressed = true;
            }
        }
        else {
            m_IsPressed = false;
        }
    }
    void Draw() {
        if (!m_Visible) return;
        if (m_HasClipRect) {
            // 因為 Button 繼承自 GameObject，可以直接呼叫引擎底層的 DrawRect
            DrawRect(m_ClipX, m_ClipY, m_ClipW, m_ClipH);
        } else {
            // 沒設定的話，就正常畫出整張圖
            GameObject::Draw();
        }
        if (m_TextGO) {
            m_TextGO->Draw(); // 🚀 畫出完美的 UIText
        }

    }
    void SetScale(float x, float y) {
        m_Transform.scale = { x, y };
    }
    void SetClipRect(int x, int y, int w, int h) {

        m_HasClipRect = true;

        m_ClipX = x;
        m_ClipY = y;
        m_ClipW = w;
        m_ClipH = h;

        // 🚀 clip 區域的原始尺寸
        float clipWidth = static_cast<float>(w);
        float clipHeight = static_cast<float>(h);

        // 🚀 防呆
        if (clipWidth <= 0 || clipHeight <= 0)
            return;

        // 🚀 分別計算 X/Y 縮放
        float scaleX = m_Width / clipWidth;
        float scaleY = m_Height / clipHeight;

        // 🚀 不強制等比
        m_Transform.scale = { scaleX, scaleY };
    }
private:
    std::shared_ptr<UIText> m_TextGO; // 🚀 型別換成你的 UIText
    std::string m_CurrentContent;
    std::function<void()> m_OnClick;
    bool m_IsPressed;

    float m_Width;
    float m_Height;
    bool m_HasClipRect = false;
    int m_ClipX = 0;
    int m_ClipY = 0;
    int m_ClipW = 0;
    int m_ClipH = 0;
};

#endif