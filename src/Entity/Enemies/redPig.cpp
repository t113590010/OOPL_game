#include "Entity/Enemies/redPig.hpp"
#include "Entity/UnitData.hpp"

redPig::redPig(const Vector2& pos)
: Entity(
   pos,
   UnitData::Get(MY_ID).hp,           // 血量
   UnitData::Get(MY_ID).speed,        // 跑速
   UnitData::Get(MY_ID).damage,       // 傷害
   UnitData::Get(MY_ID).attackRange,  // 攻擊距離
   UnitData::Get(MY_ID).attackCd,     // 攻擊冷卻
   UnitData::Get(MY_ID).cost,         // 價格
   UnitData::Get(MY_ID).spawnCd,      // 產兵冷卻
   UnitData::Get(MY_ID).rank,         // 階級
   UnitData::Get(MY_ID).imgPath,      // 圖片路徑
   UnitData::Get(MY_ID).kb            // 擊退次數
) {
    m_IsPlayerTeam = true;
    SetAoE(UnitData::Get(MY_ID).isAoE);
    // 設定敵人的碰撞/顯示尺寸，符合裁切圖的 50x56
    SetSize(50, 56);
}

void redPig::Update(float dt) {
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
void redPig::InitAnimation(const std::vector<SpriteFrame>& allFrames) {
    if (allFrames.size() < 5) return; // 至少要到 index 4

    std::vector<AnimFrame> walk, attack, kb;

    // 1. 走路：使用 index 0, 1 循環 (蠕動或漂浮感)
    for(int i = 0; i <=2; i++) {
        AnimFrame frame;
        frame.parts.push_back({allFrames[i], 0.0f, 0.0f});
        walk.push_back(frame);
    }

    // 2. 攻擊：使用 index 1, 2, 3 (咬下去的動作)
    for(int i = 3; i <= 10; i++) {
        AnimFrame frame;
        // int handleY;
        // if (i < 7) {
        //     handleY = i;
        // } else {
        //     handleY = 6 - i;
        // }
        float offsetY = 40.0f;
        if (4<i && i <7) {
       frame.parts.push_back({allFrames[i], 0.0f, offsetY});
        }else {
       frame.parts.push_back({allFrames[i], 0.0f, 0.0f});

        }

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