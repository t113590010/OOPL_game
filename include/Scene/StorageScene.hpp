#ifndef STORAGE_SCENE_HPP
#define STORAGE_SCENE_HPP

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "Button.hpp"
#include <memory>
#include <functional>
#include "NumberSystem.hpp"

class StorageScene {
public:
    StorageScene();

    void Update();
    void Draw();

    // 綁定返回主畫面的事件
    void SetOnReturnBtnClick(std::function<void()> callback);
    void SetOnToUse(std::function<void()> callback);
    void SetOnToXp(std::function<void()> callback);

private:
    Util::GameObject m_Background;

    // 預留返回按鈕
    std::shared_ptr<Button> m_ReturnBtn;
    std::function<void()> m_OnReturn;
    std::shared_ptr<Button> m_ToUse;
    std::function<void()> m_OnToUse;
    std::shared_ptr<Button> m_ToXp;
    std::function<void()> m_OnToXp;

    std::shared_ptr<NumberSystem> m_XPNumber;
    std::shared_ptr<NumberSystem> m_CatFoodNumber; // 🥫 罐頭
    std::shared_ptr<NumberSystem> m_cat_count; // 🥫 罐頭

    int m_CatFood = 139;
    int m_XP = 3000;
    int cat_count =1234567890;

    // 這裡之後可以放你的貓咪網格、分頁按鈕等
    // std::vector<std::shared_ptr<Button>> m_CatSlots;
};

#endif