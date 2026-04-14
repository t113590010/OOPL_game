#ifndef HOME_SCENE_HPP
#define HOME_SCENE_HPP

#include "Util/GameObject.hpp"
#include "System/Button.hpp"
#include <functional>
#include <memory>

class HomeScene : public Util::GameObject {
public:
    HomeScene(); // 建構子宣告

    // 🚀 給 App.cpp 對接用的接口宣告
    void SetOnStartBtnClick(std::function<void()> callback);

    void Update() ;
    void Draw() ;

private:
    std::shared_ptr<Button> m_StartBtn;
    std::function<void()> m_OnStart;
};

#endif