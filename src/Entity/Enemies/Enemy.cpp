#include "Entity/Enemies/Enemy.hpp"
#include "Entity/UnitData.hpp"

Enemy::Enemy(const Vector2& pos)
    : Entity(
        pos,
        UnitData::Get(UnitID::BASIC_ENEMY).hp,           // 150
        UnitData::Get(UnitID::BASIC_ENEMY).speed,        // -40.0f (敵人向左走，速度應為負)
        UnitData::Get(UnitID::BASIC_ENEMY).damage,       // 15
        UnitData::Get(UnitID::BASIC_ENEMY).attackRange,  // 60.0f
        UnitData::Get(UnitID::BASIC_ENEMY).attackCd,     // 1.5f
        UnitData::Get(UnitID::BASIC_ENEMY).cost,         // 0
        UnitData::Get(UnitID::BASIC_ENEMY).spawnCd,      // 0.0f
        UnitData::Get(UnitID::BASIC_ENEMY).rank,         // 1
        UnitData::Get(UnitID::BASIC_ENEMY).imgPath,      // 圖片路徑
        UnitData::Get(UnitID::BASIC_ENEMY).kb            // 擊退次數
    ) {
    SetAoE(UnitData::Get(UnitID::BASIC_ENEMY).isAoE);

    m_IsPlayerTeam = false; // 💡 敵人記得設為 false

    // 設定敵人的碰撞/顯示尺寸，符合裁切圖的 50x56
    SetSize(50, 56);
}

void Enemy::Update(float dt) {
    UpdateCooldown(dt);
    UpdateAnimation(dt);

    if (UpdateKnockback(dt)) {
        return;
    }

    // 敵人移動邏輯： m_Speed 為負值，故向左移動
    if (m_IsMoving && m_CurrentState == EntityState::WALK) {
        m_Position.x += m_Speed * dt;
    }
}

// ==========================================
// 🚀 基礎敵人 (Dog)：單圖包裝進 AnimFrame 系統
// ==========================================
void Enemy::InitAnimation(const std::vector<SpriteFrame>& allFrames) {
    // 💡 安全檢查：確保有抓到 index 6 (共 7 張以上)
    if (allFrames.size() < 7) return;

    std::vector<AnimFrame> walk, attack, kb;

    // 💡 資料分析：Dog 的裁切皆為 50x56，無需偏移 (0,0)

    // 1. 走路：依照素材順序 0, 1, 2
    for(int i = 0; i <= 2; i++) {
        AnimFrame frame;
        // 將單張裁切包裝進零件，Offset 皆為 0
        frame.parts.push_back({allFrames[i], 0.0f, 0.0f});
        walk.push_back(frame);
    }

    // 2. 攻擊：依照素材順序 3, 4, 5
    for(int i = 3; i <= 5; i++) {
        AnimFrame frame;
        frame.parts.push_back({allFrames[i], 0.0f, 0.0f});
        attack.push_back(frame);
    }

    // 3. 擊退/死亡：使用第 6 張
    AnimFrame frame6;
    frame6.parts.push_back({allFrames[6], 0.0f, 0.0f});
    kb.push_back(frame6);

    // 💡 IDLE 狀態 (敵人通常不需要冷卻 IDLE，但為了架構完整填入站立圖)
    AnimFrame idleFrame;
    idleFrame.parts.push_back({allFrames[0], 0.0f, 0.0f});
    std::vector<AnimFrame> idle = {idleFrame};

    // 填入實體動畫狀態機
    SetWalkFrames(walk);
    SetAttackFrames(attack);
    SetKnockbackFrames(kb); // 💡 使用專用的擊退圖
    SetDeathFrames(kb);     // 死亡也用同一張
    SetIdleFrames(idle);

    // 敵人動作通常較快，設為 12 FPS
    SetAnimFPS(12.0f);
}