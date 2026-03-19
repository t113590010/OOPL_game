#include "Entity/Cats/Cat.hpp"
#include "Entity/UnitData.hpp" // 💡 引入神級資料庫

Cat::Cat(const Vector2& pos)
    : Entity(
        pos,
        UnitData::Get(UnitID::CAT).hp,           // 血量
        UnitData::Get(UnitID::CAT).speed,        // 跑速
        UnitData::Get(UnitID::CAT).damage,       // 傷害
        UnitData::Get(UnitID::CAT).attackRange,  // 攻擊距離
        UnitData::Get(UnitID::CAT).attackCd,     // 攻擊冷卻
        UnitData::Get(UnitID::CAT).cost,         // 價格
        UnitData::Get(UnitID::CAT).spawnCd,      // 產兵冷卻
        UnitData::Get(UnitID::CAT).rank,         // 階級
        UnitData::Get(UnitID::CAT).imgPath,      // 圖片路徑
        UnitData::Get(UnitID::CAT).kb            // 擊退次數
    ) {
    m_IsPlayerTeam = true;
    SetAoE(UnitData::Get(UnitID::CAT).isAoE);
}
void Cat::Update(float dt) {
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
void Cat::InitAnimation(const std::vector<SpriteFrame>& allFrames) {
    // 確保有足夠的圖 (0~5，共 6 張)
    if (allFrames.size() < 6) return;

    std::vector<AnimFrame> walk, attack,idle;

    int walkcount = 2; // Index 0, 1, 2
    int atkcount = 5;  // Index 3, 4, 5

    // 0~2 是走路 (3 張)
    for(int i = 0; i <= walkcount; i++) {
        AnimFrame frame;
        // 基本貓不用拼圖，直接把單張圖塞進去，偏移量給 0
        frame.parts.push_back({allFrames[i], 0.0f, 0.0f});
        walk.push_back(frame);
    }
    // 2. IDLE (冷卻時用第 0 幀站立)
    AnimFrame idleFrame;
    idleFrame.parts.push_back({allFrames[0], 0.0f, 0.0f});
    idle.push_back(idleFrame);
    // 3~5 是攻擊 (3 張) -> 💡 記得從 walkcount + 1 開始！
    for(int i = walkcount + 1; i <= atkcount; i++) {
        AnimFrame frame;
        frame.parts.push_back({allFrames[i], 0.0f, 0.0f});
        attack.push_back(frame);
    }

    // 填入實體
    SetWalkFrames(walk);
    SetAttackFrames(attack);
    SetIdleFrames(idle);
    // 處理擊退與死亡 (用明確宣告，避免 C++ 型別推斷錯誤)
    AnimFrame kbFrame;
    kbFrame.parts.push_back({allFrames[0], 0.0f, 0.0f});

    std::vector<AnimFrame> kbList;
    kbList.push_back(kbFrame);

    SetKnockbackFrames(kbList);
    SetDeathFrames(kbList);

    SetAnimFPS(12.0f); // 基本貓可以走快一點，12 FPS 差不多
}