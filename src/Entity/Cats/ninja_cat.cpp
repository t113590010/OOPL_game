#include "Entity/Cats/ninja_cat.hpp"
#include "Entity/UnitFactory.hpp"
ninja_cat::ninja_cat(const Vector2& pos)
    : Entity(
        pos,
        UnitData::Get(UnitID::ninja_cat).hp,           // 血量
        UnitData::Get(UnitID::ninja_cat).speed,        // 跑速
        UnitData::Get(UnitID::ninja_cat).damage,       // 傷害
        UnitData::Get(UnitID::ninja_cat).attackRange,  // 攻擊距離
        UnitData::Get(UnitID::ninja_cat).attackCd,     // 攻擊冷卻
        UnitData::Get(UnitID::ninja_cat).cost,         // 價格
        UnitData::Get(UnitID::ninja_cat).spawnCd,      // 產兵冷卻
        UnitData::Get(UnitID::ninja_cat).rank,         // 階級
        UnitData::Get(UnitID::ninja_cat).imgPath ,      // 圖片路徑
        UnitData::Get(UnitID::ninja_cat).kb   // 擊退

    ) {
    SetAoE(UnitData::Get(UnitID::ninja_cat).isAoE);

    m_IsPlayerTeam = true;
    SetSize(50,100);
}
void ninja_cat::Update(float dt) {
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
void ninja_cat::InitAnimation(const std::vector<SpriteFrame>& allFrames) {
    if (allFrames.size() < 18) return;

    std::vector<AnimFrame> walk, attack, kb;

    // 1. 走路：使用 N1, N2, N3, N4
    int walkIndices[] = {2, 3, 4, 6};
    for(int i : walkIndices) {
        AnimFrame frame;
        frame.parts.push_back({allFrames[i], 0.0f, 0.0f});
        walk.push_back(frame);
    }

    // 2. 攻擊：從 Index 9 開始到 12 (張大嘴與準備動作)
    for(int i = 9; i <= 12; i++) {
        AnimFrame frame;
        frame.parts.push_back({allFrames[i], 0.0f, 0.0f});
        attack.push_back(frame);
    }
    // 攻擊最後一幀補上 Index 17 (動作收尾)
    AnimFrame atkEnd;
    atkEnd.parts.push_back({allFrames[17], 0.0f, 0.0f});
    attack.push_back(atkEnd);

    // 3. 擊退/死亡：通常用最歪的那張，這裡假設是 Index 13
    AnimFrame kbFrame;
    kbFrame.parts.push_back({allFrames[13], 0.0f, 0.0f});
    kb.push_back(kbFrame);

    SetWalkFrames(walk);
    SetAttackFrames(attack);
    SetKnockbackFrames(kb);
    SetDeathFrames(kb);

    SetAnimFPS(10.0f);
}