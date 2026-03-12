    #include "App.hpp"

    #include "Util/Image.hpp"
    #include "Util/Input.hpp"
    #include "Util/Keycode.hpp"
    #include "Util/Logger.hpp"
    #include "Entity/UnitID.hpp"
    #include "Core/Context.hpp"

    void App::Start() {
        LOG_TRACE("Start");
        m_CurrentState = State::UPDATE;
        std::vector<UnitID> globalPlayerDeck = {
            UnitID::CAT,
            UnitID::LONG_LEG_CAT,
            UnitID::NONE,
            UnitID::NONE,
            UnitID::NONE
        };

        // 💡 把全域資料「注入」給遊戲場景！GameScene 完全不需要知道這份名單是怎麼來的。
        m_GameScene = std::make_shared<GameScene>(globalPlayerDeck);
    }

    void App::Update() {
        // auto context = Core::Context::GetInstance();
        //TODO: do your things here and delete this line <3
        
        /*
         * Do not touch the code below as they serve the purpose for
         * closing the window.
         */
        float dt = 0.016f; // 假設 60 FPS
        if (m_GameScene) {
            m_GameScene->Update(dt);
            m_GameScene->Draw();
        }

        if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) ||
            Util::Input::IfExit()) {
            m_CurrentState = State::END;
        }
    }

    void App::End() { // NOLINT(this method will mutate members in the future)
        LOG_TRACE("End");
    }
