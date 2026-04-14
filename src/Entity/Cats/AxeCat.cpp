#include "Entity/Cats/AxeCat.hpp"
#include "Entity/UnitFactory.hpp"
AxeCat::AxeCat(const Vector2& pos)
    : Entity(
        pos,
        UnitData::Get(UnitID::AXE_CAT).hp,           // 血量
        UnitData::Get(UnitID::AXE_CAT).speed,        // 跑速
        UnitData::Get(UnitID::AXE_CAT).damage,       // 傷害
        UnitData::Get(UnitID::AXE_CAT).attackRange,  // 攻擊距離
        UnitData::Get(UnitID::AXE_CAT).attackCd,     // 攻擊冷卻
        UnitData::Get(UnitID::AXE_CAT).cost,         // 價格
        UnitData::Get(UnitID::AXE_CAT).spawnCd,      // 產兵冷卻
        UnitData::Get(UnitID::AXE_CAT).rank,         // 階級
        UnitData::Get(UnitID::AXE_CAT).imgPath ,      // 圖片路徑
        UnitData::Get(UnitID::AXE_CAT).kb   // 擊退

    ) {
    SetAoE(UnitData::Get(UnitID::AXE_CAT).isAoE);

    m_IsPlayerTeam = true;
    SetSize(80,130);
}

void AxeCat::Update(float dt) {
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
void AxeCat::InitAnimation(const std::vector<SpriteFrame>& allFrames) {
    // 💡 安全檢查：確保有抓到 index 6 (共 7 張以上)
    if (allFrames.size() < 7) return;

    std::vector<AnimFrame> walk, attack, kb;

    // 💡 資料分析：Dog 的裁切皆為 50x56，無需偏移 (0,0)

    // 1. 走路：依照素材順序 0, 1, 2
    for(int i = 0; i <=2; i++) {
        AnimFrame frame;
        // 將單張裁切包裝進零件，Offset 皆為 0
        frame.parts.push_back({allFrames[i], 0.0f, 0.0f});
        walk.push_back(frame);
    }

    // 2. 攻擊：依照素材順序 3, 4, 5
    for(int i =3; i <= 7; i++) {
        AnimFrame frame;
        frame.parts.push_back({allFrames[i], 0.0f, 0.0f});
        attack.push_back(frame);
    }

    // 3. 擊退/死亡：使用第 6 張
    AnimFrame frame6;
    frame6.parts.push_back({allFrames[7], 0.0f, 0.0f});
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