#ifndef NUMBERSYSTEM_HPP
#define NUMBERSYSTEM_HPP

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "Core/Context.hpp"
#include <string>
#include <memory>
#include <vector>

class NumberSystem : public Util::GameObject {
public:
    // 💡 定義你的字體種類
    enum class FontType {
        YELLOW_BIG,   // 原本的黃色大字
        WHITE_SMALL   // 你新發現的白色字體 (儲藏庫用)
    };


    // 💡 建構子：最後面多加一個 font_type，預設給 YELLOW_BIG 就不會弄壞原本的扭蛋機
    NumberSystem(float pos_ratio_x, float pos_ratio_y, float char_width, float char_height, const std::string& img_path, FontType font_type = FontType::YELLOW_BIG)
        : GameObject(std::make_unique<Util::Image>(img_path), 50),
          m_FontType(font_type) { // 👈 存下字體類型

        auto context = Core::Context::GetInstance();
        float halfW = context->GetWindowWidth() / 2.0f;
        float halfH = context->GetWindowHeight() / 2.0f;

        m_StartX = halfW * pos_ratio_x;
        m_StartY = halfH * pos_ratio_y;
        m_CharWidth = char_width;
        m_CharHeight = char_height;
        m_CurrentValue = 0;
    }

    // 💡 更新你要顯示的數字
    void SetValue(int newValue) {
        m_CurrentValue = newValue;
    }

    // 💡 繪製函數：拆解數字並連續繪製
    void Draw() {
        if (!m_Visible) return;

        auto img = std::dynamic_pointer_cast<Util::Image>(m_Drawable);
        if (!img) return;
        glm::vec2 sheetSize = img->GetSize();

        std::string numStr = std::to_string(m_CurrentValue);

        float currentX = m_StartX;

        // 4. 迴圈：由右至左畫數字
        for (size_t i = 0; i < numStr.length(); ++i) {
            // 反向讀取：i=0 讀最後一個字，i=1 讀倒數第二個字
            char c = numStr[numStr.length() - 1 - i];
            int digit = c - '0';

            float clipX = 0.0f, clipY = 0.0f, clipW = 50.0f, clipH = 50.0f;
            GetClipInfo(digit, clipX, clipY, clipW, clipH);

            m_Transform.translation = { currentX, m_StartY };
            m_Transform.scale = {
                (clipW / sheetSize.x) * (m_CharWidth / clipW),
                (clipH / sheetSize.y) * (m_CharHeight / clipH)
            };

            DrawRect(clipX, clipY, clipW, clipH);

            // ==========================================
            // 🚀 字距微調系統 (Kerning)
            // ==========================================
            // 預設每個數字往左推的距離 (你可以調這個基礎值，例如 0.9f 讓字距更緊湊)
            float step = m_CharWidth * 0.91f;

            // 如果「下一個準備要畫的數字」(也就是目前的左邊那個數字) 是特定的瘦子
            // 我們就讓它不要退那麼多，這樣就能完美貼齊！
            if (i + 1 < numStr.length()) {
                char nextChar = numStr[numStr.length() - 1 - (i + 1)];
                if (nextChar == '1') {
                    // 因為 1 比較瘦，往左退少一點 (0.7f 倍)
                    step = m_CharWidth * 0.87f;
                }
                // 你還可以繼續加，例如：
                // else if (nextChar == '7') { step = m_CharWidth * 0.85f; }
            }

            // 把 X 座標往「左」推，準備畫下一個數字
            currentX -= step;
        }
    }
private:
    float m_StartX;
    float m_StartY;
    float m_CharWidth;
    float m_CharHeight;
    int m_CurrentValue;
    FontType m_FontType;
    // 💡 這個私有函數負責「查字典」：給你數字 0~9，回傳切圖座標
    void GetClipInfo(int digit, float& x, float& y, float& w, float& h) {
        if (m_FontType == FontType::YELLOW_BIG) {
            // ==========================================
            // 🟡 原本的黃色大數字邏輯
            // ==========================================
            w = 32.0f;
            h = 44.0f;
            float startX = 0.0f;
            float startY = 0.0f;

            if (digit >= 8) {
                x = startX + ((digit - 8) * w);
                y = startY + h;
            } else {
                x = startX + (digit * w);
                y = startY;
            }
        }
        else if (m_FontType == FontType::WHITE_SMALL) {
            // ==========================================
            // ⚪ 新增的白色數字邏輯 (等待你填入數據)
            // ==========================================
            w = 28.0f; // TODO: 填入白色數字的寬度
            h = 335.0f-295.0f; // TODO: 填入白色數字的高度
            float startX = 0.0f; // TODO: 白色數字 '0' 的 X 座標
            float startY = 295.0f; // TODO: 白色數字 '0' 的 Y 座標

            // 🚀 套用你量出來的排列規則公式
            x = startX + (digit * w);
            y = startY;
        }

    }
};

#endif