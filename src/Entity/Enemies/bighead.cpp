#include "Entity/Enemies/bighead.hpp"
#include "Entity/UnitData.hpp"

bighead::bighead(const Vector2& pos)
    : Entity(
        pos,
        UnitData::Get(UnitID::bighead).hp,           // 150
        UnitData::Get(UnitID::bighead).speed,        // -40.0f (敵人向左走，速度應為負)
        UnitData::Get(UnitID::bighead).damage,       // 15
        UnitData::Get(UnitID::bighead).attackRange,  // 60.0f
        UnitData::Get(UnitID::bighead).attackCd,     // 1.5f
        UnitData::Get(UnitID::bighead).cost,         // 0
        UnitData::Get(UnitID::bighead).spawnCd,      // 0.0f
        UnitData::Get(UnitID::bighead).rank,         // 1
        UnitData::Get(UnitID::bighead).imgPath,      // 圖片路徑
        UnitData::Get(UnitID::bighead).kb            // 擊退次數
    ) {
    SetAoE(UnitData::Get(UnitID::bighead).isAoE);

    m_IsPlayerTeam = false; // 💡 敵人記得設為 false

    // 設定敵人的碰撞/顯示尺寸，符合裁切圖的 50x56
    SetSize(50, 56);
}

void bighead::Update(float dt) {
    UpdateCooldown(dt);
    UpdateAnimation(dt);

    if (UpdateKnockback(dt)) {
        return;
    }
    if (m_CurrentState == EntityState::ATTACK) {
        // 優先級 1：正在攻擊中！
        // 絕對不要改變狀態，讓動畫好好播完，並且定在原地不准走。
        m_IsMoving = false;

    } else if (m_AttackTimer > 0) {
        // 優先級 2：攻擊動畫已經播完了，但技能還在冷卻中。
        // 這時候才切換成發呆 (IDLE) 狀態。
        SetState(EntityState::IDLE); // 建議使用 SetState 確保動畫歸零
        m_IsMoving = false;

    } else {
        // 優先級 3：非攻擊狀態，且冷卻已完畢。
        // 恢復正常前進！
        SetState(EntityState::WALK);
        m_IsMoving = true;
    }

    // 敵人移動邏輯： m_Speed 為負值，故向左移動
    if (m_IsMoving && m_CurrentState == EntityState::WALK) {
        m_Position.x += m_Speed * dt;
    }
}

// ==========================================
// 🚀 基礎敵人 (Dog)：單圖包裝進 AnimFrame 系統
// ==========================================
void bighead::InitAnimation(const std::vector<SpriteFrame>& allFrames) {
    if (allFrames.size() < 5) return; // 至少要到 index 4

    std::vector<AnimFrame> walk, attack, kb;

    // 1. 走路：使用 index 0, 1 循環 (蠕動或漂浮感)
    for(int i = 0; i <=0; i++) {
        AnimFrame frame;
        frame.parts.push_back({allFrames[i], 0.0f, 0.0f});
        walk.push_back(frame);
    }

    // 2. 攻擊：使用 index 1, 2, 3 (咬下去的動作)
    for(int i = 1; i <= 3; i++) {
        AnimFrame frame;
        frame.parts.push_back({allFrames[i], 0.0f, 0.0f});
        attack.push_back(frame);
    }

    // 3. 擊退/死亡：使用 index 4 (受傷表情)
    AnimFrame frame4;
    frame4.parts.push_back({allFrames[4], 0.0f, 0.0f});
    kb.push_back(frame4);

    // 💡 IDLE 狀態
    AnimFrame idleFrame;
    idleFrame.parts.push_back({allFrames[0], 0.0f, 0.0f});
    std::vector<AnimFrame> idle = {idleFrame};

    SetWalkFrames(walk);
    SetAttackFrames(attack);
    SetKnockbackFrames(kb);
    SetDeathFrames(kb);
    SetIdleFrames(idle);

    SetAnimFPS(8.0f); // Boss 動作通常比較沉重，FPS 調低一點
}