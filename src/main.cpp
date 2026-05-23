#include "App.hpp"

#include "Core/Context.hpp"

int main(int, char**) {
    auto context = Core::Context::GetInstance();
    App app;

    while (!context->GetExit()) {
        switch (app.GetCurrentState()) {
            case App::State::START:
                app.Start();
                break;
            case App::State::TITLE:
            case App::State::HOME:
            case App::State::BATTLE:
            case App::State::STORAGE:       // 👈 補上冰箱
            case App::State::RARE_GACHA:    // 👈 補上稀有轉蛋
            case App::State::NORMAL_GACHA:  // 👈 補上一般轉蛋
            case App::State::LEVEL_UPGRADE:  // 👈 補上升級
            case App::State::DECK:  // 👈 補上隊伍編成

                app.Update();
                break;
            case App::State::LEVEL_SELECT:
                app.UpdateLevelSelectScene();
                break;
            case App::State::END:
                app.End();
                context->SetExit(true);
                break;
        }
        context->Update();
    }
    return 0;
}
