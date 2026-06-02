#ifndef STAGE_CARD_UI_HPP
#define STAGE_CARD_UI_HPP

#include <memory>
#include <functional>

#include "Button.hpp"
#include "Util/GameObject.hpp"
#include "Util/Image.hpp"

class StageCardUI {
public:

    StageCardUI(
        int stageID,
        const std::string& stageName,
        float pos_ratio_x,
        float pos_ratio_y
    );

    void Update();

    void Draw();

    void ApplyScroll(float scrollX);

    void ApplyTransform(
        float finalX,
        float scale
    );

    void SetOnClick(
        std::function<void(int)> callback
    );

    void SetVisible(bool visible) {
        m_IsVisible = visible;
    }

    bool IsVisible() const {
        return m_IsVisible;
    }

    int GetStageID() const {
        return m_StageID;
    }

    float GetBaseX() const {
        return m_BaseCenter.x;
    }

private:

    // =========================
    // Stage Info
    // =========================

    int m_StageID;

    std::string m_StageName;

    // =========================
    // State
    // =========================

    bool m_IsVisible = true;

    bool m_IsSelected = false;

    // =========================
    // Layout
    // =========================

    float m_Width = 320.0f;

    float m_Height = 120.0f;

    glm::vec2 m_BaseCenter;

    // =========================
    // UI Objects
    // =========================

    std::shared_ptr<Button>
        m_BackgroundBtn;

    std::shared_ptr<Util::GameObject>
        m_StageImage;

    std::shared_ptr<Button>
        m_StageNameText;

    // =========================
    // Callback
    // =========================

    std::function<void(int)>
        m_OnClick;

    // =========================
    // Offsets
    // =========================

    const glm::vec2 OFFSET_IMAGE =
        { 0.0f, 20.0f };

    const glm::vec2 OFFSET_NAME =
        { 0.0f, -55.0f };

    // =========================
    // Scale Cache
    // =========================

    glm::vec2 m_BgBaseScale;

    glm::vec2 m_ImageBaseScale;
};

#endif