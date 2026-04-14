# files.cmake

set(SRC_FILES
        "App.cpp"
        "Scene/StartScene.cpp"
        "Scene/HomeScene.cpp"
        "Scene/GameScene.cpp"
        "Entity/Cats/Cat.cpp"
        "Entity/Cats/AxeCat.cpp"
        "Entity/Cats/ninja_cat.cpp"

        "Entity/Enemies/Enemy.cpp"
        "Entity/Enemies/GAY.cpp"
        "Entity/Enemies/Snack.cpp"
        "Entity/Enemies/p3.cpp"
        "Entity/Enemies/bighead.cpp"


        "Entity/Cats/LongLegCat.cpp"
        "Entity/Base.cpp"
        "Entity/UnitFactory.cpp"
        "Entity/UnitData.cpp"

        "System/SpawnSystem.cpp"
        "System/PauseMenu.cpp"
        "System/BattleSystem.cpp"
        "System/CollisionSystem.cpp"
        "System/UISystem.cpp"

)

set(INCLUDE_FILES
        "App.hpp"
        "GameConfig.hpp"

        "Scene/HomeScene.hpp"
        "Scene/StartScene.hpp"
        "Scene/GameScene.hpp"
        "Entity/Entity.hpp"
        "Entity/Cats/Cat.hpp"
        "Entity/Cats/AxeCat.hpp"
        "Entity/Cats/ninja_cat.hpp"

        "Entity/Enemies/Enemy.hpp"
        "Entity/Enemies/GAY.hpp"
        "Entity/Enemies/Snack.hpp"
        "Entity/Enemies/p3.hpp"
        "Entity/Enemies/bighead.hpp"


        "Entity/Cats/LongLegCat.hpp"
        "Entity/Base.hpp"
        "Entity/UnitID.hpp"
        "Entity/UnitFactory.hpp"
        "Entity/UnitData.hpp"
        "System/SpawnSystem.hpp"
        "System/PauseMenu.hpp"
        "System/BattleSystem.hpp"
        "System/Background.hpp"

        "System/CollisionSystem.hpp"
        "System/Button.hpp"
        "System/UIText.hpp"
        "System/WorldText.hpp"
        "System/UISystem.hpp"

)
set(TEST_FILES
)
