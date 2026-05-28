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

    NumberSystem(float pos_ratio_x, float pos_ratio_y, float char_width, float char_height, const std::string& img_path, FontType font_type = FontType::YELLOW_BIG)
        : GameObject(std::make_unique<Util::Image>(img_path), 50),
          m_FontType(font_type) {

        auto context = Core::Context::GetInstance();
        float halfW = context->GetWindowWidth() / 2.0f;
        float halfH = context->GetWindowHeight() / 2.0f;

        m_StartX = halfW * pos_ratio_x;
        m_StartY = halfH * pos_ratio_y;
        m_CharWidth = char_width;
        m_CharHeight = char_height;
        m_CurrentValueStr = "0"; // 🚀 改用字串儲存
    }

    void SetPosition(float absoluteX, float absoluteY) {
        m_StartX = absoluteX;
        m_StartY = absoluteY;
    }

    // 🚀 多載 1：相容原本的 int 輸入
    void SetValue(int newValue) {
        m_CurrentValueStr = std::to_string(newValue);
    }

    // 🚀 多載 2：支援字串輸入 (可以傳入 "+90", "/", "MAX")
    void SetValue(const std::string& newValue) {
        m_CurrentValueStr = newValue;
    }

    void SetScale(float scale) {
        m_Scale = scale;
    }

    // 💡 繪製函數：拆解字串並連續繪製
    void Draw() {
        if (!m_Visible) return;

        auto img = std::dynamic_pointer_cast<Util::Image>(m_Drawable);
        if (!img) return;
        glm::vec2 sheetSize = img->GetSize();

        std::string numStr = m_CurrentValueStr;

        // 🚀 攔截 MAX：如果字串是 MAX，我們把它當作單一特殊字元 'M' 處理，避免拆成三個字母
        if (numStr == "MAX" || numStr == "max") {
            numStr = "M";
        }

        float currentX = m_StartX;

        // 由右至左畫字元
        for (size_t i = 0; i < numStr.length(); ++i) {
            char c = numStr[numStr.length() - 1 - i];

            float clipX = 0.0f, clipY = 0.0f, clipW = 50.0f, clipH = 50.0f;

            // 🚀 改傳 char 給 GetClipInfo 去查字典
            GetClipInfo(c, clipX, clipY, clipW, clipH);

            m_Transform.translation = { currentX, m_StartY };
            m_Transform.scale = {
                (clipW / sheetSize.x) * ((m_CharWidth * m_Scale) / clipW),
                (clipH / sheetSize.y) * ((m_CharHeight * m_Scale) / clipH)
            };

            DrawRect(clipX, clipY, clipW, clipH);

            // 字距微調系統 (Kerning)
            float step = m_CharWidth * m_Scale * 0.91f;

            if (i + 1 < numStr.length()) {
                char nextChar = numStr[numStr.length() - 1 - (i + 1)];
                if (nextChar == '1') {
                    step = m_CharWidth * m_Scale * 0.87f;
                } else if (nextChar == '+') {
                    step = m_CharWidth * m_Scale * 0.80f;
                }
            }
            currentX -= step;
        }
    }
private:
    float m_Scale = 1.0f;
    float m_StartX;
    float m_StartY;
    float m_CharWidth;
    float m_CharHeight;
    std::string m_CurrentValueStr; // 🚀 統一儲存成字串
    FontType m_FontType;

    // 💡 查字典：現在改成吃 char 了！
    void GetClipInfo(char c, float& x, float& y, float& w, float& h) {
        if (m_FontType == FontType::YELLOW_BIG) {
            w = 32.0f;
            h = 44.0f;
            float startX = 0.0f;
            float startY = 0.0f;
            float offsetSpeX = 4.0f;
            // 🚀 數字 0~9 的處理
            if (c >= '0' && c <= '9') {
                int digit = c - '0';
                if (digit >= 8) {
                    x = startX + ((digit - 8) * w);
                    y = startY + h;
                } else {
                    x = startX + (digit * w);
                    y = startY;
                }
            }
            // 🚀 特殊符號處理 (你要填入實際的 xywh)
            else if (c == '+') {
                x = startX + ((14 - 8) * w)+offsetSpeX+2.0;
                y = startY + h;
            }
            else if (c == '/') {
                x = startX + ((10 - 8) * w);
                y = startY + h;
            }
            else if (c == 'M') {
                // 'M' 代表整個 MAX 圖示
                x = 448.0; y = 44.0f; // TODO: 填入 MAX 的座標
                w = 512.0f-x; h=71.0-y;        // MAX 通常比較寬，這裡 w 要改大一點
            }
            else if (c == '$') {//元
                x = startX + ((11 - 8) * w)+offsetSpeX;
                y = startY + h;
            }
        }
        else if (m_FontType == FontType::WHITE_SMALL) {
            w = 28.0f;
            h = 335.0f - 295.0f;
            float startX = 0.0f;
            float startY = 295.0f;

            if (c >= '0' && c <= '9') {
                int digit = c - '0';
                x = startX + (digit * w);
                y = startY;
            }
            // 🚀 白色字體的特殊符號也寫在這裡
            else if (c == '+') {
                // TODO
            }
        }
    }
};

#endif