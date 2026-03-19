#include "Entity/Cats/LongLegCat.hpp"
#include "Entity/UnitFactory.hpp"
LongLegCat::LongLegCat(const Vector2& pos)
    : Entity(
        pos,
        UnitData::Get(UnitID::LONG_LEG_CAT).hp,           // 血量
        UnitData::Get(UnitID::LONG_LEG_CAT).speed,        // 跑速
        UnitData::Get(UnitID::LONG_LEG_CAT).damage,       // 傷害
        UnitData::Get(UnitID::LONG_LEG_CAT).attackRange,  // 攻擊距離
        UnitData::Get(UnitID::LONG_LEG_CAT).attackCd,     // 攻擊冷卻
        UnitData::Get(UnitID::LONG_LEG_CAT).cost,         // 價格
        UnitData::Get(UnitID::LONG_LEG_CAT).spawnCd,      // 產兵冷卻
        UnitData::Get(UnitID::LONG_LEG_CAT).rank,         // 階級
        UnitData::Get(UnitID::LONG_LEG_CAT).imgPath ,      // 圖片路徑
        UnitData::Get(UnitID::LONG_LEG_CAT).kb   // 擊退

    ) {
    SetAoE(UnitData::Get(UnitID::LONG_LEG_CAT).isAoE);

    m_IsPlayerTeam = true;
    SetSize(50,100);
}
void LongLegCat::Update(float dt) {
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
void LongLegCat::InitAnimation(const std::vector<SpriteFrame>& allFrames) {
    if (allFrames.size() < 11) return;

    std::vector<AnimFrame> walk, attack, idle;

    // 1. 走路 (0~5)
    for(int i = 0; i <= 5; i++) {
        AnimFrame frame;
        frame.parts.push_back({allFrames[i], 0.0f, 0.0f});
        walk.push_back(frame);
    }

    // 2. IDLE (冷卻時用第 0 幀站立)
    AnimFrame idleFrame;
    idleFrame.parts.push_back({allFrames[0], 0.0f, 0.0f});
    idle.push_back(idleFrame);

    // 3. 攻擊順序：6 -> 7 -> 8(+10) -> 9
    // 💡 解決「回溯」：將寬素材向左推，讓貓身(根部)對齊座標。
    // 因為寬度從約 60 變成 171，多出約 110 像素。往左推一半(約 -55)能讓中心對齊。
    float attackOffsetX = -55.0f;

    // [幀 6]
    AnimFrame f6; f6.parts.push_back({allFrames[6], attackOffsetX, 0.0f});
    attack.push_back(f6);

    // [幀 7]
    AnimFrame f7; f7.parts.push_back({allFrames[7], attackOffsetX, 0.0f});
    attack.push_back(f7);

    // [幀 8] 🚀 組合拼接 8(大腿) + 10(腳尖)
    AnimFrame f8;
    f8.parts.push_back({allFrames[10], attackOffsetX, 0.0f});   // 腳尖
    f8.parts.push_back({allFrames[8], attackOffsetX, 69.0f});    // 大腿向上推 69
    attack.push_back(f8);

    // [幀 9]
    AnimFrame f9; f9.parts.push_back({allFrames[9], attackOffsetX, 0.0f});
    attack.push_back(f9);

    SetWalkFrames(walk);
    SetAttackFrames(attack);
    SetIdleFrames(idle); // 💡 設定 IDLE 影格

    // 擊退與死亡
    SetKnockbackFrames(idle);
    SetDeathFrames(idle);

    SetAnimFPS(15.0f);
}