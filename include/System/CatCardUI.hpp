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
    CatCardUI(UnitID id, float pos_ratio_x, float pos_ratio_y);

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
private:
    UnitID m_UnitID;
    bool m_IsVisible = true; // 新增這個變數
    float m_Width = 260.0f;
    float m_Height = 180.0f;

    // 紀錄卡片剛生出來時的「初始絕對基準點」，滑動時以此推算
    glm::vec2 m_BaseCenter;

    std::shared_ptr<Button> m_BackgroundBtn;
    std::shared_ptr<Util::GameObject> m_CatIcon;
    std::shared_ptr<Util::GameObject> m_NameImage;
    std::shared_ptr<NumberSystem> m_LevelNumber;
    std::shared_ptr<NumberSystem> m_CostNumber;
    std::shared_ptr<Button> m_SwapBtn;

    // 內部相對偏移量 (像素)
    const glm::vec2 OFFSET_ICON  = { 0.0f,  25.0f };
    const glm::vec2 OFFSET_NAME  = {   0.0f,  65.0f };
    const glm::vec2 OFFSET_LEVEL = {  65.0f, -10.0f };
    const glm::vec2 OFFSET_COST  = { 40.0f, -85.0f };
    const glm::vec2 OFFSET_SWAP  = { 130.0f, -100.0f };
    glm::vec2 m_BgBaseScale;
    glm::vec2 m_SwapBaseScale;
    float CatIconScaleX =2.8;
    float CatIconScaleY =2.8;

};

#endif