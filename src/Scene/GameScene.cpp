#include "Scene/GameScene.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "GameConfig.hpp"
#include <iostream>
GameScene::GameScene() {
    m_CameraX = GameConfig::CAMERA_MIN_X;
    // 玩家基地放在左下角 (X 為負數，Y 為負數)
    m_PlayerBase = std::make_shared<Base>(Vector2{GameConfig::PLAYER_BASE_X, GameConfig::BASE_Y}, GameConfig::BASE_HP);
    m_PlayerBase->SetImage(RESOURCE_DIR "/img/catBase.png");

    // 敵人基地放在右下角 (X 為正數，Y 為負數)
    m_EnemyBase = std::make_shared<Base>(Vector2{GameConfig::ENEMY_BASE_X, GameConfig::BASE_Y}, GameConfig::BASE_HP);
    m_EnemyBase->SetImage(RESOURCE_DIR "/img/enemyBase.png");
    m_EnemyBase->SetTeam(false);

    m_Entities.push_back(m_PlayerBase);
    m_Entities.push_back(m_EnemyBase);
    m_Background = std::make_shared<Background>(RESOURCE_DIR "/img/bg.png");
    m_MoneyText = std::make_shared<UIText>(
         0.8f, 0.8f,
         "MONEY: 0 / " + std::to_string(GameConfig::MAX_MONEY_LEVEL_1),
         30,
         Util::Color(255, 255, 0, 255)
     );

    m_BaseNameText = std::make_shared<WorldText>(
        GameConfig::PLAYER_BASE_X,
        GameConfig::BASE_Y + 200.0f,
        " "
    );
    m_EnemyBaseText = std::make_shared<WorldText>(
          GameConfig::ENEMY_BASE_X,          // 使用敵方基地的 X 座標
          GameConfig::BASE_Y + 200.0f,       // 一樣的高度
          " "
      );
}
void GameScene::Update(float dt) {
    // 讓錢隨著時間慢慢增加
    m_CurrentMoney += GameConfig::MONEY_GROWTH_SPEED * dt;
    // 限制錢包上限
    if (m_CurrentMoney > GameConfig::MAX_MONEY_LEVEL_1) {
        m_CurrentMoney = GameConfig::MAX_MONEY_LEVEL_1;
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::RIGHT) || Util::Input::IsKeyPressed(Util::Keycode::D)) {
        m_CameraX += m_CameraSpeed * dt;
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::LEFT) || Util::Input::IsKeyPressed(Util::Keycode::A)) {
        m_CameraX -= m_CameraSpeed * dt;
    }

    // 防呆機制 (Clamp)：限制攝影機不能看超過左邊基地，也不能看超過右邊基地
    // 假設戰場最左邊是 -600，最右邊是 600
    if (m_CameraX < GameConfig::CAMERA_MIN_X) m_CameraX = GameConfig::CAMERA_MIN_X;
    if (m_CameraX > GameConfig::CAMERA_MAX_X) m_CameraX = GameConfig::CAMERA_MAX_X;


    // 生成敵人
    m_EnemySpawnTimer += dt;
    if (m_EnemySpawnTimer > 2.0f) { // 每 2 秒生成一隻敵人
        SpawnEnemy();
        m_EnemySpawnTimer = 0.0f;
        std::cout << "Enemy spawned!\n";
    }

    // 按鍵生成貓
    if (Util::Input::IsKeyDown(Util::Keycode::SPACE)) {
        SpawnCat();
        std::cout << "CAT spawned!\n";
    }

    // 更新所有 Entity
    for (auto& entity : m_Entities) {
        entity->Update(dt);
    }

    // 碰撞檢測
    CheckCollisions();

    // 移除死亡單位
    RemoveDeadEntities();

    // 簡單印出基地血量
    std::cout << "Player Base HP: " << m_PlayerBase->GetHP()
              << " | Enemy Base HP: " << m_EnemyBase->GetHP() << "\r";
    if (m_MoneyText) {
        std::string moneyStr = "MONEY: " + std::to_string((int)m_CurrentMoney) +
                               " / " + std::to_string(GameConfig::MAX_MONEY_LEVEL_1);
        m_MoneyText->UpdateText(moneyStr);
    }
    if (m_BaseNameText) {
        std::string hpString =  std::to_string(m_PlayerBase->GetHP());
        m_BaseNameText->UpdateText(hpString);
    }
    if (m_EnemyBaseText) {
        std::string enemyHpString = std::to_string(m_EnemyBase->GetHP());
        m_EnemyBaseText->UpdateText(enemyHpString);
    }
}

void GameScene::SpawnCat() {

    // 檢查錢夠不夠
    if (m_CurrentMoney >= GameConfig::CAT_COST) {
        m_CurrentMoney -= GameConfig::CAT_COST; // 扣錢

        float spawnX = m_PlayerBase->GetPosition().x + GameConfig::SPAWN_OFFSET_X;
        auto cat = std::make_shared<Cat>(Vector2{spawnX, GameConfig::BASE_Y});
        m_Entities.push_back(cat);

        std::cout << "\n[System] Spawned Cat! Money left: " << (int)m_CurrentMoney << std::endl;
    } else {
        std::cout << "\n[System] Not enough money! Need: " << GameConfig::CAT_COST << std::endl;
    }

}

void GameScene::SpawnEnemy() {
    float spawnX = m_EnemyBase->GetPosition().x - GameConfig::SPAWN_OFFSET_X;
    auto enemy = std::make_shared<Enemy>(Vector2{spawnX, GameConfig::BASE_Y});
    m_Entities.push_back(enemy);
}

void GameScene::CheckCollisions() {
    // 1. 每一幀開始時，先預設所有人都可以繼續往前走
    for (auto& e : m_Entities) {
        e->SetMoving(true);
    }

    // 2. 開始巡視戰場
    for (auto& entityA : m_Entities) {
        if (!entityA->IsAlive()) continue;

        for (auto& entityB : m_Entities) {
            if (entityA == entityB || !entityB->IsAlive()) continue;

            // 同陣營不打架
            if (entityA->IsPlayerTeam() == entityB->IsPlayerTeam()) continue;

            // 檢查距離 (這裡的 100.0f 是攻擊範圍，因為圖片是 100x100，半徑設大一點才不會疊在一起)
            if (entityA->CheckCollision(entityB, entityA->GetAttackRange())) {

                // 【核心邏輯 1】發現攻擊範圍內有敵人，馬上煞車！
                entityA->SetMoving(false);

                // 【核心邏輯 2】如果武器冷卻好了，就揍他！
                if (entityA->CanAttack()) {

                    // 👇 改成這樣：拿 A 的攻擊力，去扣 B 的血！
                    entityB->TakeDamage(entityA->GetDamage());

                    entityA->ResetAttackTimer();
                    std::cout << (entityA->IsPlayerTeam() ? "Cat" : "Enemy")
                              << " attacked! Target HP left: " << entityB->GetHP() << "\n";
                }

                // 每次只鎖定面前的「第一個」敵人攻擊，打完就跳出內層迴圈
                break;
            }
        }
    }
}
void GameScene::RemoveDeadEntities() {
    m_Entities.erase(
        std::remove_if(m_Entities.begin(), m_Entities.end(),
                       [](const std::shared_ptr<Entity>& e) { return !e->IsAlive(); }),
        m_Entities.end());
}

void GameScene::Draw() {
    if (m_Background) {
        m_Background->Draw(m_CameraX);
    }
    for (auto& entity : m_Entities) {
        // 把 m_CameraX 交給實體，讓它自己算該畫在哪裡
        entity->Draw(m_CameraX);

    }
    if (m_BaseNameText) m_BaseNameText->Draw(m_CameraX);
    if (m_EnemyBaseText) m_EnemyBaseText->Draw(m_CameraX);
    if (m_MoneyText) {
        m_MoneyText->Draw(); // 👈 只要這短短一行就夠了！
    }
}
