#ifndef APP_HPP
#define APP_HPP

#include "pch.hpp" // IWYU pragma: export
#include "Scene/GameScene.hpp"
#include <memory>
#include "Util/BGM.hpp"
#include "HomeScene.hpp"
#include "StartScene.hpp"
#include "StorageScene.hpp"
#include "RareGachaScene.hpp"
#include "NormalGachaScene.hpp"
#include "LevelUpgradeScene.hpp"
#include "DeckScene.hpp"


class App {
public:
    enum class State {
        START,
        TITLE,
        LEVEL_SELECT,
        HOME,    // 👈 取代了 UPDATE
        BATTLE,  // 👈 取代了 UPDATE,
        DECK,
        STORAGE,       // 👈 新增：冰箱狀態
        RARE_GACHA,    // 👈 新增：稀有轉蛋狀態
        NORMAL_GACHA,  // 👈 新增：貓咪轉蛋狀態
        LEVEL_UPGRADE,
        END
    };
    State GetCurrentState() const { return m_CurrentState; }
    void SetCurrentState(State s){  m_CurrentState=s; }

    void Start();

    void Update();
    void UpdateLevelSelectScene();
    void End(); // NOLINT(readability-convert-member-functions-to-static)

private:
    void ValidTask();
    void LoadStartScene();
    void StartHomeScene();
   

     void StartBattleScene(int stageIdx);
    void StartStorageScene();
    void StartRareGachaScene();
    void StartNormalGachaScene();
    void StartLevelUpgradeScene();
    void StartDeckScene();

private:
    State m_CurrentState = State::START;
    std::shared_ptr<StartScene> m_StartScene;
    std::shared_ptr<HomeScene> m_HomeScene;
    std::shared_ptr<GameScene> m_GameScene;
    std::shared_ptr<Util::BGM> m_BattleBGM;
    std::shared_ptr<Util::BGM> m_MenuBGM;
    std::shared_ptr<LevelUpgradeScene> m_LevelUpgradeScene;
    std::shared_ptr<DeckScene> m_DeckScene;
    std::shared_ptr<StorageScene> m_StorageScene;
    std::shared_ptr<RareGachaScene> m_RareGachaScene;
    std::shared_ptr<NormalGachaScene> m_NormalGachaScene;
    bool m_PendingQuit = false;
};

#endif
