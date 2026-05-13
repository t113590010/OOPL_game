#include "Entity/Cats/Peashooter.hpp"

Peashooter::Peashooter(const Vector2& pos)
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
    SetScale(0.6f);
    // 🚀 載入豌豆圖片並設定給渲染器
    SetProjectileUnit(true);
    m_PeaImage = std::make_shared<Util::Image>(RESOURCE_DIR "/img/ProjectilePea.png");
    m_PeaRenderer.SetDrawable(m_PeaImage);
    m_PeaRenderer.m_Transform.scale = {2.0f, 2.0f};
    // m_AttackSoundPath = RESOURCE_DIR "/music/what-da-dog-doin.mp3"; // 換成狗的攻擊音效
    // m_HitSoundPath    = RESOURCE_DIR "/music/bibilabu.mp3";   // 換成狗的受傷音效
    // m_DeadSoundPath   = RESOURCE_DIR "/music/gugugaga.mp3";    // 換成狗的死亡音效
    // m_AttackSoundFrame = 1;
}
void Peashooter::Update(float dt) {
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
        if (!m_HasFired) {
            Pea newPea;
            // 設定發射點：從本體中心稍微往左(或右)和往上推一點，對齊嘴巴
            // (依據你的遊戲方向，如果貓是向左走，就是 - 偏移量)
            newPea.position = {m_Position.x, m_Position.y + 20.0f};
            m_Peas.push_back(newPea);
            m_HasFired = true;
        }
    } else if (m_AttackTimer > 0) {
        // 優先級 2：攻擊動畫已經播完了，但技能還在冷卻中。
        // 這時候才切換成發呆 (IDLE) 狀態。
        SetState(EntityState::IDLE); // 建議使用 SetState 確保動畫歸零
        m_IsMoving = false;
        m_HasFired = false; // 🚀 重置發射開關

    } else {
        // 優先級 3：非攻擊狀態，且冷卻已完畢。
        // 恢復正常前進！
        SetState(EntityState::WALK);
        m_IsMoving = true;
        m_HasFired = false; // 🚀 重置發射開關
    }

    // 移動控制
    if (m_IsMoving && m_CurrentState == EntityState::WALK) {
        m_Position.x -= m_Speed * dt;
    }

    for (auto& pea : m_Peas) {
        if (pea.active) {
            // 判斷方向：因為你的本體是 -= m_Speed，代表玩家是往左攻，所以豌豆也要往左飛 (-)
            float peaDir = m_IsPlayerTeam ? -1.0f : 1.0f;
            pea.position.x += peaDir * pea.speed * dt;

            // 如果飛離本體太遠 (例如 1200 像素)，就標記為失效，避免無限消耗記憶體
            if (std::abs(pea.position.x - m_Position.x) > 1200.0f) {
                pea.active = false;
            }
        }
    }
    m_Peas.erase(std::remove_if(m_Peas.begin(), m_Peas.end(),
        [](const Pea& p) { return !p.active; }), m_Peas.end());
}

// ==========================================
// 💡 基本貓：單圖包裝進 AnimFrame 系統
// ==========================================
void Peashooter::InitAnimation(const std::vector<SpriteFrame>& allFrames) {
    // 確保有足夠的圖 (0~5，共 6 張)
    if (allFrames.size() < 6) return;
    std::cout<<allFrames.size();
    std::vector<AnimFrame> walk, attack,idle;
    float offsetY = -30.0f;
    int walkcount = 10; // Index 0~8站立 從9~
    int atkcount =12;  // Index 3, 4, 5


    for(int i = 6; i <=walkcount  ; i++) {

        AnimFrame frame;
        // 基本貓不用拼圖，直接把單張圖塞進去，偏移量給 0

        frame.parts.push_back({allFrames[i], 0.0f, offsetY});


        walk.push_back(frame);
    }

    // 2. IDLE (冷卻時用第 0 幀站立)
    AnimFrame idleFrame;
    idleFrame.parts.push_back({allFrames[0], 0.0f, offsetY});


    idle.push_back(idleFrame);

    AnimFrame frame;
    frame.parts.push_back({allFrames[11], 0.0f, offsetY});
    attack.push_back(frame);




    // 填入實體
    SetWalkFrames(walk);
    SetAttackFrames(attack);
    SetIdleFrames(idle);
    // 處理擊退與死亡 (用明確宣告，避免 C++ 型別推斷錯誤)
    AnimFrame kbFrame;
    kbFrame.parts.push_back({allFrames[0], 0.0f, offsetY});

    std::vector<AnimFrame> kbList;
    kbList.push_back(kbFrame);

    SetKnockbackFrames(kbList);
    SetDeathFrames(kbList);

    SetAnimFPS(8.0f); // 基本貓可以走快一點，12 FPS 差不多
}
// ==========================================
// 🚀 覆寫渲染邏輯：畫完射手後，順便畫出所有豌豆
// ==========================================
void Peashooter::Draw(float cameraX) {
    // 1. 呼叫老爸的 Draw，先畫出豌豆射手本體
    Entity::Draw(cameraX);

    // 2. 把正在飛的豌豆畫出來
    if (m_PeaImage && !m_Peas.empty()) {
        for (const auto& pea : m_Peas) {
            if (pea.active) {
                // 設定豌豆在畫面上的正確位置
                m_PeaRenderer.m_Transform.translation = {
                    pea.position.x - cameraX,
                    pea.position.y
                };

                // 💡 如果你的豌豆原圖太大，可以在這裡調整縮放 (例如 0.4f)
                m_PeaRenderer.m_Transform.scale = {1.0f, 1.0f};

                m_PeaRenderer.Draw();
            }
        }
    }
}