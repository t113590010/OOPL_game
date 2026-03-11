# files.cmake

set(SRC_FILES
        "App.cpp"
        "Scene/StartScene.cpp"
        "Scene/GameScene.cpp"
        "Entity/Cats/Cat.cpp"
        "Entity/Enemies/Enemy.cpp"
        "Entity/Base.cpp"
        "System/SpawnSystem.cpp"
        "System/BattleSystem.cpp"
        "System/CollisionSystem.cpp"
)

set(INCLUDE_FILES
        "App.hpp"
        "GameConfig.hpp"

        "Scene/Scene.hpp"
        "Scene/StartScene.hpp"
        "Scene/GameScene.hpp"
        "Entity/Entity.hpp"
        "Entity/Cats/Cat.hpp"
        "Entity/Enemies/Enemy.hpp"
        "Entity/Base.hpp"
        "System/SpawnSystem.hpp"
        "System/BattleSystem.hpp"
        "System/Background.hpp"

        "System/CollisionSystem.hpp"
        "System/UIText.hpp"
        "System/WorldText.hpp"

)
set(TEST_FILES
)
