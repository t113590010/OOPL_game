# files.cmake

set(SRC_FILES
        "App.cpp"

        "Scene/StartScene.cpp"
        "Scene/HomeScene.cpp"
        "Scene/GameScene.cpp"
        "Scene/StorageScene.cpp"
        "Scene/RareGachaScene.cpp"
        "Scene/NormalGachaScene.cpp"
        "Scene/LevelUpgradeScene.cpp"
        "Scene/DeckScene.cpp"
        "Scene/LevelSelectScene.cpp"

        "Entity/Cats/Cat.cpp"
        "Entity/Cats/AxeCat.cpp"
        "Entity/Cats/ninja_cat.cpp"
        "Entity/Cats/LongCat.cpp"
        "Entity/Cats/CowCat.cpp"
        "Entity/Cats/FlyCat.cpp"
        "Entity/Cats/FishCat.cpp"
        "Entity/Cats/DinoCat.cpp"
        "Entity/Cats/GaintCat.cpp"
        "Entity/Cats/DogDoin.cpp"
        "Entity/Cats/Peashooter.cpp"
        "Entity/Cats/Queen.cpp"



        "Entity/Enemies/Enemy.cpp"
        "Entity/Enemies/GAY.cpp"
        "Entity/Enemies/Snack.cpp"
        "Entity/Enemies/p3.cpp"
        "Entity/Enemies/bighead.cpp"
        "Entity/Enemies/redPig.cpp"
        "Entity/Enemies/hippo.cpp"


        "Entity/Cats/LongLegCat.cpp"
        "Entity/Base.cpp"
        "Entity/UnitFactory.cpp"
        "Entity/UnitData.cpp"

        "System/PlayerData.cpp"

        "System/SpawnSystem.cpp"
        "System/PauseMenu.cpp"
        "System/SureMenu.cpp"
        "System/DeckUI.cpp"
        "System/CatCardUI.cpp"
        "System/SlotUI.cpp"
        "System/StageCardUI.cpp"
        "System/StageSelectUI.cpp"

        "System/BattleSystem.cpp"
        "System/CollisionSystem.cpp"
        "System/UISystem.cpp"
        "System/LevelSystem.cpp"
        "System/DebugMenu.cpp"
        "System/DebugCheat.cpp"


)

set(INCLUDE_FILES
        "App.hpp"
        "GameConfig.hpp"

        "Scene/HomeScene.hpp"
        "Scene/StartScene.hpp"
        "Scene/GameScene.hpp"
        "Scene/StorageScene.hpp"
        "Scene/RareGachaScene.hpp"
        "Scene/NormalGachaScene.hpp"
        "Scene/LevelUpgradeScene.hpp"
        "Scene/DeckScene.hpp"
        "Scene/LevelSelectScene.hpp"

        "Entity/Entity.hpp"
        "Entity/Cats/Cat.hpp"
        "Entity/Cats/AxeCat.hpp"
        "Entity/Cats/ninja_cat.hpp"
        "Entity/Cats/LongCat.hpp"
        "Entity/Cats/CowCat.hpp"
        "Entity/Cats/FlyCat.hpp"
        "Entity/Cats/FishCat.hpp"
        "Entity/Cats/DinoCat.hpp"
        "Entity/Cats/GaintCat.hpp"
        "Entity/Cats/DogDoin.hpp"
        "Entity/Cats/Peashooter.hpp"
        "Entity/Cats/Queen.hpp"


        "Entity/Enemies/Enemy.hpp"
        "Entity/Enemies/GAY.hpp"
        "Entity/Enemies/Snack.hpp"
        "Entity/Enemies/p3.hpp"
        "Entity/Enemies/bighead.hpp"
        "Entity/Enemies/redPig.hpp"
        "Entity/Enemies/hippo.hpp"


        "Entity/Cats/LongLegCat.hpp"
        "Entity/Base.hpp"
        "Entity/UnitID.hpp"
        "Entity/UnitFactory.hpp"
        "Entity/UnitData.hpp"
        "System/SpawnSystem.hpp"
        "System/PauseMenu.hpp"
        "System/SureMenu.hpp"
        "System/BattleSystem.hpp"
        "System/Background.hpp"
        "System/DeckUI.hpp"
        "System/CatCardUI.hpp"
        "System/SlotUI.hpp"
        "System/StageCardUI.hpp"
        "System/StageSelectUI.hpp"

        "System/PlayerData.hpp"
        "System/CollisionSystem.hpp"
        "System/Button.hpp"
        "System/UIText.hpp"
        "System/WorldText.hpp"
        "System/UISystem.hpp"
        "System/LevelSystem.hpp"
        "System/DebugMenu.hpp"
        "System/DebugCheat.hpp"
)
set(TEST_FILES
)
