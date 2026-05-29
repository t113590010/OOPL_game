#ifndef STAGE_SELECT_UI_HPP
#define STAGE_SELECT_UI_HPP

#include <vector>
#include <memory>
#include <functional>

#include "StageCardUI.hpp"

class StageSelectUI {
public:

    StageSelectUI();

    void Update();

    void Draw();

    void LoadStages(
        const std::vector<int>& stageList
    );

    void SetOnStageSelected(
        std::function<void(int)> callback
    );

    int GetCenterStageID() const;

    float GetScrollX() const {
        return m_ScrollX;
    }

    void SetScrollX(float x) {
        m_ScrollX = x;
        m_TargetScrollX = x;
    }

private:

    // =========================
    // 關卡卡片
    // =========================

    std::vector<
        std::shared_ptr<StageCardUI>
    > m_Cards;

    std::function<void(int)>
        m_OnStageSelected;

    // =========================
    // 滑動控制
    // =========================

    bool m_IsDragging = false;

    float m_LastMouseX = 0.0f;

    float m_ScrollX = 0.0f;

    float m_TargetScrollX = 0.0f;

    // =========================
    // 排版
    // =========================

    float m_BaseYRatio = 0.0f;

    float m_Spacing = 350.0f;

    int m_CurrentCenterIndex = 0;
};

#endif