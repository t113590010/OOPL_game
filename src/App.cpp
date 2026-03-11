    #include "App.hpp"

    #include "Util/Image.hpp"
    #include "Util/Input.hpp"
    #include "Util/Keycode.hpp"
    #include "Util/Logger.hpp"
    #include "Core/Context.hpp"

    void App::Start() {
        LOG_TRACE("Start");
        m_CurrentState = State::UPDATE;
    }

    void App::Update() {
        // auto context = Core::Context::GetInstance();
        //TODO: do your things here and delete this line <3
        
        /*
         * Do not touch the code below as they serve the purpose for
         * closing the window.
         */
        float dt = 0.016f; // 假設 60 FPS
        m_GameScene.Update(dt);

        m_GameScene.Draw();

        if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) ||
            Util::Input::IfExit()) {
            m_CurrentState = State::END;
        }
    }

    void App::End() { // NOLINT(this method will mutate members in the future)
        LOG_TRACE("End");
    }
