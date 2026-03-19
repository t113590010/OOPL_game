#ifndef APP_HPP
#define APP_HPP

#include "pch.hpp" // IWYU pragma: export
#include "Scene/GameScene.hpp"
#include <memory>
#include "Util/BGM.hpp"
class App {
public:
    enum class State {
        START,
        UPDATE,
        END,
    };

    State GetCurrentState() const { return m_CurrentState; }
    void SetCurrentState(State s){  m_CurrentState=s; }

    void Start();

    void Update();

    void End(); // NOLINT(readability-convert-member-functions-to-static)

private:
    void ValidTask();


private:
    State m_CurrentState = State::START;
    std::shared_ptr<GameScene> m_GameScene;
    std::shared_ptr<Util::BGM> m_BattleBGM;
};

#endif
