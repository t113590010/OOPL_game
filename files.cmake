# files.cmake

set(SRC_FILES
        "App.cpp"
        "Scene/StartScene.cpp"
        "Scene/GameScene.cpp"
        "Entity/Cats/Cat.cpp"
        "Entity/Enemies/Enemy.cpp"
        "Entity/Enemies/GAY.cpp"

        "Entity/Cats/LongLegCat.cpp"
        "Entity/Base.cpp"
        "Entity/UnitFactory.cpp"
        "Entity/UnitData.cpp"

        "System/SpawnSystem.cpp"
        "System/BattleSystem.cpp"
        "System/CollisionSystem.cpp"
        "System/UISystem.cpp"

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
        "Entity/Enemies/GAY.hpp"

        "Entity/Cats/LongLegCat.hpp"
        "Entity/Base.hpp"
        "Entity/UnitID.hpp"
        "Entity/UnitFactory.hpp"
        "Entity/UnitData.hpp"
        "System/SpawnSystem.hpp"
        "System/BattleSystem.hpp"
        "System/Background.hpp"

        "System/CollisionSystem.hpp"
        "System/UIText.hpp"
        "System/WorldText.hpp"
        "System/UISystem.hpp"

)
set(TEST_FILES
)
