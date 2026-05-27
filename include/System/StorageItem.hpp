#ifndef STORAGE_ITEM_HPP
#define STORAGE_ITEM_HPP

#include "Util/GameObject.hpp"
#include "Core/Context.hpp"
#include "Button.hpp"
#include "Entity/UnitData.hpp"
#include "Entity/Entity.hpp"
#include <memory>
#include <string>
#include <cmath>
#include <algorithm>
#include <filesystem>
#include <cstdlib>
#include <functional>
std::vector<SpriteFrame> ParseImgCut(const std::string& filepath);

class StorageItem {
public: // 👈 注意這裡！下面的都是公開的！
    StorageItem(float pos_ratio_x, float pos_ratio_y, UnitID cat_id)
        : m_CatID(cat_id), m_IsSelected(false) {

        auto context = Core::Context::GetInstance();
        float halfW = context->GetWindowWidth() / 2.0f;
        float halfH = context->GetWindowHeight() / 2.0f;

        m_CurrentX = halfW * pos_ratio_x;
        m_CurrentY = halfH * pos_ratio_y;

        m_VX = ((rand() % 200) / 100.0f - 1.0f) * 1.5f;
        m_VY = ((rand() % 200) / 100.0f - 1.0f) * 1.5f;
        if (std::abs(m_VX) < 0.2f) m_VX = 0.8f;
        if (std::abs(m_VY) < 0.2f) m_VY = -0.8f;

        m_Rotation = 0.0f;
        m_RotationSpeed = ((rand() % 100) / 100.0f - 0.5f) * 0.05f;

        auto& stats = UnitData::Get(cat_id);
        if (!stats.imgPath.empty()) {
            auto tempImg = std::make_shared<Util::Image>(stats.imgPath);
            m_SheetWidth = tempImg->GetSize().x;
            m_SheetHeight = tempImg->GetSize().y;

            std::string path = stats.imgPath;
            size_t lastSlash = path.find_last_of("/\\");
            std::string filename = path.substr(lastSlash + 1);
            size_t dotPos = filename.find_last_of(".");
            if (dotPos != std::string::npos) filename = filename.substr(0, dotPos);
            std::string baseDir = path.substr(0, path.find_last_of("/\\"));
            baseDir = baseDir.substr(0, baseDir.find_last_of("/\\"));
            std::string imgcutPath = baseDir + "/imgcut/" + filename + ".imgcut";

            if (std::filesystem::exists(imgcutPath)) {
                std::vector<SpriteFrame> allFrames = ParseImgCut(imgcutPath);

                // ========================================================
                // 🚀 巨人貓特判：將身體與腳當作整體，取最高與最低點算出真正的旋轉中心
                // ========================================================
               if (cat_id == UnitID::GaintCat && allFrames.size() >= 2) {
                    m_IsGiantCat = true;
                    SpriteFrame frame0 = allFrames[0];
                    SpriteFrame frame1 = allFrames[1];

                    float maxSlotSize = 100.0f;
                    m_ScaleFactor = maxSlotSize / std::max(frame0.w, frame0.h);

                    m_FinalW = frame0.w * m_ScaleFactor;
                    m_FinalH = frame0.h * m_ScaleFactor;
                    m_LegsFinalW = frame1.w * m_ScaleFactor;
                    m_LegsFinalH = frame1.h * m_ScaleFactor;

                    // 依照你的邏輯：計算最高 Y 與最低 Y (以身體中心點 0,0 為基準)
                    float maxY = m_FinalH / 2.0f; // 身體最頂端

                    // 🚀 修正這裡：把腳本身高度的一半加回來往上提，這樣腿才不會掉下去！
                    float legCenterY = -(m_FinalH / 2.0f) - (15.0f * m_ScaleFactor) + (m_LegsFinalH / 2.0f);

                    float minY = legCenterY - (m_LegsFinalH / 2.0f); // 腳的最底端

                    // 算出整體的真正中心點 Y
                    float overallCenterY = (maxY + minY) / 2.0f;

                    // 算出「身體」與「腳」相對於這個新中心的 Local 座標
                    m_BodyLocalY = 0.0f - overallCenterY;
                    m_LegLocalX = 3.0f * m_ScaleFactor; // 腳偏右 3 offset
                    m_LegLocalY = legCenterY - overallCenterY;

                    // 創建漂浮身體
                    m_CatButton = std::make_shared<Button>(pos_ratio_x, pos_ratio_y, m_FinalW, m_FinalH, stats.imgPath, " ", 30, Util::Color(255,255,255,255));
                    m_CatButton->SetClipRect(frame0.x, frame0.y, frame0.w, frame0.h);
                    m_CatButton->SetScale(m_ScaleFactor, m_ScaleFactor);
                    m_CatButton->SetZIndex(33);

                    // 創建漂浮腳腳
                    m_CatLegsButton = std::make_shared<Button>(pos_ratio_x, pos_ratio_y, m_LegsFinalW, m_LegsFinalH, stats.imgPath, " ", 30, Util::Color(255,255,255,255));
                    m_CatLegsButton->SetClipRect(frame1.x, frame1.y, frame1.w, frame1.h);
                    m_CatLegsButton->SetScale(m_ScaleFactor, m_ScaleFactor);
                    m_CatLegsButton->SetZIndex(32);

                    m_CatButton->SetOnClick([this]() { if (m_OnSelectCallback) m_OnSelectCallback(this); });

                    // 彈窗專用的 Popup 按鈕 (維持原狀)
                    float m_ScaleFactorBig = 100.0f / std::max(frame0.w, frame0.h);
                    m_PopupCatButton = std::make_shared<Button>(0.0f, 0.4f, frame0.w * m_ScaleFactorBig, frame0.h * m_ScaleFactorBig, stats.imgPath, " ", 30, Util::Color(255,255,255,255));
                    m_PopupCatButton->SetClipRect(frame0.x, frame0.y, frame0.w, frame0.h);
                    m_PopupCatButton->SetScale((frame0.w * m_ScaleFactorBig * 2.5f) / m_SheetWidth, (frame0.h * m_ScaleFactorBig * 2.5f) / m_SheetHeight);
                    m_PopupCatButton->SetZIndex(80);
                }
                // ========================================================
                // 🐟 其他貓咪 (完全沒動你原本的邏輯)
                // ========================================================
                else if (!allFrames.empty()) {
                    SpriteFrame frame0 = allFrames[0];

                    float maxSlotSize = 100.0f;
                    float ratio = static_cast<float>(frame0.w) / static_cast<float>(frame0.h);
                    if (ratio > 0.8f && ratio < 1.2f) {
                        maxSlotSize=60.0f;
                    }
                    m_ScaleFactor = maxSlotSize / std::max(frame0.w, frame0.h);
                    m_FinalW = frame0.w * m_ScaleFactor;
                    m_FinalH = frame0.h * m_ScaleFactor;
                    float showCat = 100.0f;
                    float m_ScaleFactorBig = showCat / std::max(frame0.w, frame0.h);
                    float m_FinalBigW = frame0.w * m_ScaleFactorBig;
                    float m_FinalBigH = frame0.h * m_ScaleFactorBig;

                    m_CatButton = std::make_shared<Button>(
                        pos_ratio_x, pos_ratio_y,
                        m_FinalW, m_FinalH,
                        stats.imgPath, " ", 30, Util::Color(255,255,255,255)
                    );

                    m_CatButton->SetClipRect(frame0.x, frame0.y, frame0.w, frame0.h-3);
                    m_CatButton->SetScale(m_ScaleFactor, m_ScaleFactor);
                    m_CatButton->SetZIndex(32);

                    m_CatButton->SetOnClick([this]() {
                        if (m_OnSelectCallback) {
                            m_OnSelectCallback(this);
                        }
                    });

                    m_PopupCatButton = std::make_shared<Button>(
                        0.0f, 0.4f,
                        m_FinalBigW, m_FinalBigH,
                        stats.imgPath, " ", 30, Util::Color(255,255,255,255)
                    );
                    m_PopupCatButton->SetClipRect(frame0.x, frame0.y, frame0.w, frame0.h);
                    m_PopupCatButton->SetScale(
                            (m_FinalBigW * 2.5f) / m_SheetWidth,
                            (m_FinalBigH * 2.5f) / m_SheetHeight
                        );
                    m_PopupCatButton->SetZIndex(80);
                }
            }
        }
    }

    void DrawForPopup() {
        if (m_PopupCatButton) {
            m_PopupCatButton->Draw();
        }
    }
    void SetOnSelectCallback(std::function<void(StorageItem*)> cb) {
        m_OnSelectCallback = cb;
    }

    void Update() {
        if (m_CatButton) m_CatButton->Update();

        auto context = Core::Context::GetInstance();
        float halfW = context->GetWindowWidth() / 2.0f;
        float halfH = context->GetWindowHeight() / 2.0f;

        if (m_IsSelected) {
            if (m_IsGiantCat && m_CatLegsButton) {
                // 點擊時靜止，直接加上 Local 座標偏移
                m_CatButton->m_Transform.translation = { m_CurrentX, m_CurrentY + 15.0f + m_BodyLocalY };
                m_CatButton->m_Transform.rotation = 0.0f;

                m_CatLegsButton->m_Transform.translation = { m_CurrentX + m_LegLocalX, m_CurrentY + 15.0f + m_LegLocalY };
                m_CatLegsButton->m_Transform.rotation = 0.0f;
            }
            else if (m_CatButton) {
                m_CatButton->m_Transform.translation = { m_CurrentX, m_CurrentY + 15.0f };
                m_CatButton->m_Transform.rotation = 0.0f;
            }
        }
        else {
            m_CurrentX += m_VX;
            m_CurrentY += m_VY;
            m_Rotation += m_RotationSpeed;

            float paddingX = m_FinalW / 2.0f + 100.0f;
            float paddingY = m_FinalH / 2.0f + 50.0f;

            if (m_CurrentX > (halfW - paddingX)) { m_CurrentX = halfW - paddingX; m_VX = -std::abs(m_VX); }
            else if (m_CurrentX < (-halfW + paddingX)) { m_CurrentX = -halfW + paddingX; m_VX = std::abs(m_VX); }
            if (m_CurrentY > (halfH - paddingY)) { m_CurrentY = halfH - paddingY; m_VY = -std::abs(m_VY); }
            else if (m_CurrentY < (-halfH + paddingY)) { m_CurrentY = -halfH + paddingY; m_VY = std::abs(m_VY); }

            if (m_IsGiantCat && m_CatLegsButton) {
                // 🚀 套用二維旋轉矩陣，讓身體跟腳繞著共同的中心旋轉
                float cosR = std::cos(m_Rotation);
                float sinR = std::sin(m_Rotation);

                // 身體旋轉
                m_CatButton->m_Transform.translation = {
                    m_CurrentX + (0.0f * cosR - m_BodyLocalY * sinR),
                    m_CurrentY + (0.0f * sinR + m_BodyLocalY * cosR)
                };
                m_CatButton->m_Transform.rotation = m_Rotation;

                // 腳旋轉
                m_CatLegsButton->m_Transform.translation = {
                    m_CurrentX + (m_LegLocalX * cosR - m_LegLocalY * sinR),
                    m_CurrentY + (m_LegLocalX * sinR + m_LegLocalY * cosR)
                };
                m_CatLegsButton->m_Transform.rotation = m_Rotation;
            }
            else if (m_CatButton) {
                m_CatButton->m_Transform.translation = { m_CurrentX, m_CurrentY };
                m_CatButton->m_Transform.rotation = m_Rotation;
            }
        }
    }

    void Draw() {
        if (m_CatButton) {
            glm::vec2 physicsScale = m_CatButton->m_Transform.scale;

            if (m_SheetWidth > 0 && m_SheetHeight > 0) {
                float popScale = m_IsSelected ? 1.2f : 1.0f;
                m_CatButton->m_Transform.scale = {
                    (m_FinalW / m_SheetWidth) * popScale,
                    (m_FinalH / m_SheetHeight) * popScale
                };
            }

            m_CatButton->SetZIndex(m_IsSelected ? 35 : 33);
            m_CatButton->Draw();
            m_CatButton->m_Transform.scale = physicsScale;

            // 🚀 幫巨人貓畫出腳 (套用你原版的畫法與縮放公式)
            if (m_IsGiantCat && m_CatLegsButton && m_SheetWidth > 0 && m_SheetHeight > 0) {
                glm::vec2 legsPhysicsScale = m_CatLegsButton->m_Transform.scale;
                float popScale = m_IsSelected ? 1.2f : 1.0f;

                m_CatLegsButton->m_Transform.scale = {
                    (m_LegsFinalW / m_SheetWidth) * popScale,
                    (m_LegsFinalH / m_SheetHeight) * popScale
                };
                m_CatLegsButton->SetZIndex(m_IsSelected ? 34 : 32);
                m_CatLegsButton->Draw();
                m_CatLegsButton->m_Transform.scale = legsPhysicsScale;
            }
        }
    }

    // ==========================================
    // 🚀 IDE 請看這裡！它們確實存在而且是 public！
    // ==========================================
    bool IsSelected() const { return m_IsSelected; }
    void Deselect() { m_IsSelected = false; }
    UnitID GetCatID() const { return m_CatID; }

private: // 👈 這裡才是私有區塊
    std::shared_ptr<Button> m_CatButton;
    UnitID m_CatID;
    bool m_IsSelected;
    std::function<void(StorageItem*)> m_OnSelectCallback;
    std::shared_ptr<Button> m_PopupCatButton;
    float m_CurrentX, m_CurrentY;
    float m_VX, m_VY;
    float m_Rotation, m_RotationSpeed;
    float m_FinalW = 0.0f, m_FinalH = 0.0f;
    float m_ScaleFactor = 1.0f;
    float m_SheetWidth = 0.0f, m_SheetHeight = 0.0f;

    // 🚀 巨人貓專用變數
    std::shared_ptr<Button> m_CatLegsButton = nullptr;
    bool m_IsGiantCat = false;
    float m_LegsFinalW = 0.0f, m_LegsFinalH = 0.0f;
    float m_BodyLocalY = 0.0f, m_LegLocalX = 0.0f, m_LegLocalY = 0.0f;
};

#endif