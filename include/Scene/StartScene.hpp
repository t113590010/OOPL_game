#ifndef STARTSCENE_HPP
#define STARTSCENE_HPP

#include "Util/GameObject.hpp"
#include "System/Button.hpp"
#include <functional>
#include <memory>

class StartScene {
public:
    StartScene();

    void Update();
    void Draw();

    // 留給外面綁定的接口：按下去之後要觸發什麼事（例如切換到選關畫面）
    void SetOnStartGame(std::function<void()> callback);

private:
    Util::GameObject m_Background;           // 背景圖
    std::shared_ptr<Button> m_StartBtn;      // 開始按鈕
    std::function<void()> m_OnStartGame;     // 事件推播器
};

#endif