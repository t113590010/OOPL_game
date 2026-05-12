#include "Entity/Cats/GaintCat.hpp"

GaintCat::GaintCat(const Vector2& pos)
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
}
void GaintCat::Update(float dt) {
    UpdateCooldown(dt);

    // 底層會負責在 ATTACK 播完最後一幀時，自動將狀態切換回 WALK
    UpdateAnimation(dt);

    if (UpdateKnockback(dt)) {
        return;
    }

    // ==========================================
    // 💡 邏輯修正：嚴格的狀態機優先級別
    // ==========================================
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

    // 移動控制
    if (m_IsMoving && m_CurrentState == EntityState::WALK) {
        m_Position.x -= m_Speed * dt;
    }
}

// ==========================================
// 💡 基本貓：單圖包裝進 AnimFrame 系統
// ==========================================
void GaintCat::InitAnimation(const std::vector<SpriteFrame>& allFrames) {
    // 巨神貓的切圖有 17 張 (0~16)，確保陣列夠大避免閃退
    if (allFrames.size() < 17) return;

    std::vector<AnimFrame> walk, attack, idle, kbList;

    // 💡 座標設定：
    // 巨神貓的腳高度大約是 63。如果你的 Y 軸向上是正的，
    // 我們就把上半身「往上推 60」，讓身體剛好坐在腳上。
    float bodyY = 15.0f;
    float bodyX = -3.0f;

    // 攻擊時，身體的寬度從 67 暴增到 141 (往前揍)。
    // 為了讓他的「背部/重心」留在原地，我們把攻擊的大圖往左推。
    float atkOffsetX = -0.0f;
    float atkbodyY = 0.0f;
    // =====================================
    // 1. 走路 (Walk) - 組合身體(0) + 不同的腳(1~4)
    // =====================================
    for (int i = 1; i <= 4; i++) {
        AnimFrame frame;
        if (i==1) {
            frame.parts.push_back({allFrames[0], bodyX, bodyY});      // 上層：身體
        }
        else if (i==2 ) {
            frame.parts.push_back({allFrames[0], bodyX, bodyY-3.0f});      // 上層：身體

        }else if (i==3) {
            frame.parts.push_back({allFrames[0], bodyX, bodyY-2.0f});      // 上層：身體
        }else if (i==4) {
            frame.parts.push_back({allFrames[0], bodyX, bodyY-2.0f});      // 上層：身體

        }
        frame.parts.push_back({allFrames[i], 0.0f, 0.0f});       // 底層：腳
        walk.push_back(frame);
    }

    // =====================================
    // 2. 待機 (Idle) - 身體(0) + 站立腳(1)
    // =====================================
    AnimFrame idleFrame;
    idleFrame.parts.push_back({allFrames[0], bodyX, bodyY});      // 上層：身體
    idleFrame.parts.push_back({allFrames[1], 0.0f, 0.0f});

    idle.push_back(idleFrame);

    // =====================================
    // 3. 攻擊 (Attack) - 重拳連擊！
    // =====================================
    // [幀 1] 蓄力：普通腳(1) + 蓄力身體(7)
    AnimFrame atk1;
    // atk1.parts.push_back({allFrames[1], 0.0f, 0.0f});
    atk1.parts.push_back({allFrames[7], atkOffsetX, atkbodyY});
    attack.push_back(atk1);

    // [幀 2] 揮拳：普通腳(1) + 揮拳身體(8)
    AnimFrame atk2;
    // atk2.parts.push_back({allFrames[1], 0.0f, 0.0f});
    atk2.parts.push_back({allFrames[8], atkOffsetX, atkbodyY});
    attack.push_back(atk2);

    // [幀 3] 揮拳定格：專屬腳(10) + 定格身體(9)
    AnimFrame atk3;
    // atk3.parts.push_back({allFrames[10], 0.0f, 0.0f});
    atk3.parts.push_back({allFrames[9], atkOffsetX, atkbodyY});
    attack.push_back(atk3);

    // [幀 4] 收招：專屬腳(11) + 定格身體(9)
    AnimFrame atk4;
    // atk4.parts.push_back({allFrames[11], 0.0f, 0.0f});
    atk4.parts.push_back({allFrames[9], atkOffsetX, atkbodyY});
    attack.push_back(atk4);

    // =====================================
    // 4. 擊退與死亡 (Knockback / Death)
    // =====================================
    AnimFrame kbFrame;
    // 使用 Index 13 的腳 (通常是向後仰的腳) 加上身體
    kbFrame.parts.push_back({allFrames[13], 0.0f, 0.0f});
    // kbFrame.parts.push_back({allFrames[0], 0.0f, bodyY});
    kbList.push_back(kbFrame);

    // 填入系統
    SetWalkFrames(walk);
    SetAttackFrames(attack);
    SetIdleFrames(idle);
    SetKnockbackFrames(kbList);
    SetDeathFrames(kbList);

    // 💡 巨神貓體型龐大，動作比較沉重，建議 FPS 設低一點 (8 ~ 10) 更有重量感！
    SetAnimFPS(8.0f);
}