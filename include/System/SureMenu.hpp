#ifndef SURE_MENU_HPP
#define SURE_MENU_HPP

#include "Util/Image.hpp"
#include "Util/GameObject.hpp"
#include "System/Button.hpp"
#include <functional>
#include <memory>

class SureMenu : public Util::GameObject {
public:
    SureMenu();

    // 給外面呼叫的接口：按「是」或「否」要做什麼事
    void SetOnConfirm(std::function<void()> callback);
    void SetOnCancel(std::function<void()> callback);

    void Update();
    void Draw();

private:
    std::shared_ptr<Button> m_YesBtn;
    std::shared_ptr<Button> m_NoBtn;

    std::function<void()> m_OnConfirm;
    std::function<void()> m_OnCancel;
    Util::GameObject m_Background; // 暫停時的半透明黑底

};

#endif