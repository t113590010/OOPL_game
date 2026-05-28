#ifndef CAT_CARD_UI_HPP
#define CAT_CARD_UI_HPP

#include <memory>
#include <functional>
#include "Button.hpp"
#include "Entity/UnitData.hpp"
#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "NumberSystem.hpp"

class CatCardUI {
public:
    // 🚀 回歸你的系統：吃螢幕比例
    CatCardUI(UnitID id, float pos_ratio_x, float pos_ratio_y, bool isUpgradeMode = false);

    void Update();
    void Draw();

    // 🚀 滑動時只需傳入偏移量 (ScrollX)，不破壞原始排版
    void ApplyScroll(float scrollX);

    void SetOnClick(std::function<void(UnitID)> callback);
    void SetOnSwapClick(std::function<void(UnitID)> callback);
    void ApplyTransform(float finalX, float scale);
    float GetBaseX() const { return m_BaseCenter.x; }
    // 🚀 新增：控制是否顯示與更新 (Culling)
    void SetVisible(bool visible) { m_IsVisible = visible; }
    bool IsVisible() const { return m_IsVisible; }
    UnitID GetUnitID() const { return m_UnitID; }

private:
    UnitID m_UnitID;
    bool m_IsVisible = true; // 新增這個變數
    float m_Width = 260.0f;
    float m_Height = 180.0f;
    bool m_IsSelected = false; // 是否在正中間 (Scale == 1.0)
    bool m_IsMaxLevel = false; // 等級是否 >= 20
    bool m_IsUpgradeMode;
    // 紀錄卡片剛生出來時的「初始絕對基準點」，滑動時以此推算
    glm::vec2 m_BaseCenter;

    std::shared_ptr<Button> m_BackgroundBtn;
    std::shared_ptr<Util::GameObject> m_CatIcon;
    std::shared_ptr<Util::GameObject> m_NameImage;
    std::shared_ptr<NumberSystem> m_LevelNumber;
    std::shared_ptr<NumberSystem> m_ExtraLevelNumber;
    std::shared_ptr<NumberSystem> m_lvlMaxIcon;

    std::shared_ptr<NumberSystem> m_CostNumber;

    std::shared_ptr<Button> m_SwapBtn;
    std::shared_ptr<Button> lvl_Icon;
    std::shared_ptr<Button> m_CustomCatImg;
    std::shared_ptr<Button> m_CostXPSlot;
    std::shared_ptr<Button> m_CostBg;
    glm::vec2 m_CostBgOffset = {-19.0f, -73.0f}; // 🚀 新增：遮罩的位置微調
    float m_CostBgScale = 1.0f;             // 🚀 新增：遮罩的大小的微調
    glm::vec2 m_CostBgBaseScale;
    glm::vec2 m_CostXpBaseScale; // 記住底框的原始縮放比例

    // 內部相對偏移量 (像素)
    const glm::vec2 OFFSET_ICON  = { 0.0f,  25.0f };
    const glm::vec2 OFFSET_NAME  = {   0.0f,  65.0f };
    const glm::vec2 OFFSET_LEVEL = {  100.0f, -2.0f };
    const glm::vec2 OFFSET_COST  = { 40.0f, -85.0f };
    const glm::vec2 OFFSET_SWAP  = { 130.0f, -100.0f };

    const glm::vec2 OFFSET_EXTRA_LEVEL = { 100.0f, -20.0f }; // "+90" 的位置 (在主等級的下方)
    const glm::vec2 OFFSET_LVL_ICON    = { 500.0f, -10.0f }; // "等級" 兩個綠字的位置 (在主等級的左邊)
    const glm::vec2 OFFSET_MAX_ICON    = { 35.0f,  17.0f }; // "MAX" 綠字的位置 (在等級的上方)
    glm::vec2 m_BgBaseScale;
    glm::vec2 m_SwapBaseScale;
    float CatIconScaleX =2.8;
    float CatIconScaleY =2.8;
    float LvlIconScaleX =0.07;
    float LvlIconScaleY =0.07;
    glm::vec2 m_CustomBaseScale = { 1.0f, 1.0f };
    glm::vec2 m_CustomOffset = { 0.0f, 0.0f };
    const glm::vec2 OFFSET_COST_XP_BG = { 30.0f, -55.0f };
    glm::vec2 m_CostOffset;
    float m_CostBaseScale = 1.0f;
};

#endif