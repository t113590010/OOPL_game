#include "Entity/Base.hpp"

Base::Base(const Vector2& pos, int hp)
    : Entity(pos, hp, 0.0f, 0, 0.0f, 0.0f, 0, 0.0f) {
    m_IsMoving = false;
    m_Size = {GameConfig::BASE_SIZE_X,GameConfig::BASE_SIZE_Y};
    m_Renderer.SetZIndex(-1);

    m_HitSoundPath = RESOURCE_DIR "/music/basegetatk.mp3";
    m_AttackCooldown = 999999.0f;
    m_AttackTimer = 999999.0f;

    m_BoomImage = std::make_shared<Util::Image>(RESOURCE_DIR "/img/Boom.png");
    m_BoomRenderer.SetDrawable(m_BoomImage);
    m_BoomRenderer.SetZIndex(60); // 讓特效顯示在貓咪和敵人前面

}

void Base::InitBoomAnimation(const std::vector<SpriteFrame>& frames) {
    m_BoomFrames = frames;
}

void Base::Update(float dt) {
    // 根據目前的階段執行不同邏輯
    switch (m_CannonPhase) {

        case CannonPhase::CHARGING:
            m_CannonTimer += dt;
            if (m_CannonTimer >= m_CannonCooldown) {
                m_CannonTimer = m_CannonCooldown;
                m_CannonPhase = CannonPhase::READY;
                // 🎵 充能完畢，播放提示音！
                Util::SFX(RESOURCE_DIR "/music/ChargeDone.mp3").Play();
            }
            break;

        case CannonPhase::READY:
            // 等待玩家呼叫 FireCannon()
            break;

        case CannonPhase::PRECAST:
            m_PrecastTimer -= dt;
            if (m_PrecastTimer <= 0.0f) {
                m_CannonPhase = CannonPhase::FIRING;
                m_FiringTimer = 0.0f; // 重置開火計時器
                m_DamageTrigger = true;
                m_HitTargets.clear();
            }
            break;

        case CannonPhase::FIRING:
            m_FiringTimer += dt;

            // 🚀 雷射波衝到最底大約 0.3 秒，加上最後一顆爆炸播完的時間
            // 給整個狀態維持 1.0 秒就非常完美了！
            if (m_FiringTimer >=m_CannonSec+1.0f) {
                m_CannonTimer = 0.0f;
                m_CannonPhase = CannonPhase::CHARGING;
            }
            break;


    }
}
void Base::SetImage(const std::string& imgPath) {
    m_Image = std::make_shared<Util::Image>(imgPath);
}

// UI 按鈕按下去時，呼叫這個函數！
void Base::FireCannon() {
    if (m_CannonPhase == CannonPhase::READY) {
        m_CannonPhase = CannonPhase::PRECAST;
        m_PrecastTimer = m_PrecastDuration;

        // 🎵 按下發射瞬間，播放蓄力/發射音效
        Util::SFX(RESOURCE_DIR "/music/laser.mp3").Play();
    }
}
void Base::Draw(float cameraX) {
    Entity::Draw(cameraX);

    if (m_BoomFrames.empty() || !m_BoomImage) return;

    float dir = m_IsPlayerTeam ? -1.0f : 1.0f;
    glm::vec2 imgSize = m_BoomImage->GetSize();
    float boomScale = 1.0f;

    // 📍 核心座標
    float mouthX = m_Position.x + (dir * 40.0f);
    float mouthY = m_Position.y + 100.0f;
    float floorY = m_Position.y - (GameConfig::BASE_SIZE_Y / 2.0f) + 100.0f;

    float spacing = (m_BoomFrames[0].w * boomScale) * 0.8f;
    // ==========================================
    // 🚀 貓咪砲 READY 特效
    // ==========================================
    if (m_CannonPhase == CannonPhase::READY) {

        static std::shared_ptr<Util::Image> readyImg =
            std::make_shared<Util::Image>(
                RESOURCE_DIR "/img/img002_tw.png"
            );

        static Util::GameObject readyRenderer(readyImg, 5);

        // 🚀 分隔圖區域
        int cutX = 439;
        int cutY = 259;
        int cutW = 450-439;
        int cutH = 358-259;

        // 🚀 顯示位置
        float effectX =  mouthX + (dir * m_CannonRange) - cameraX;
        float effectY = floorY-15;

        glm::vec2 imgSize = readyImg->GetSize();

        // 🚀 縮放倍率
        float scale = 0.6f;

        readyRenderer.m_Transform.translation = {
            effectX,
            effectY
        };

        readyRenderer.m_Transform.scale = {
            (cutW * scale) / imgSize.x,
            (cutH * scale) / imgSize.y
        };

        readyRenderer.m_Transform.rotation = 0.0f;

        readyRenderer.DrawRect(
            cutX,
            cutY,
            cutW,
            cutH
        );
    }
    // ==========================================
    // 🎯 預備階段 (掃描)：碎石只留在「撞擊點」！走過就清掉！
    // ==========================================
    if (m_CannonPhase == CannonPhase::PRECAST) {
        float scanProgress = 1.0f - (std::max(m_PrecastTimer, 0.0f) / m_PrecastDuration);
        float currentScanDist = m_CannonRange * scanProgress;
        float currentFloorX = mouthX + (dir * currentScanDist);

        // 🚀 1. 動態掃描射線
        float dx = currentFloorX - mouthX;
        float dy = floorY - mouthY;
        float angle = std::atan2(dy, dx);
        float length = std::sqrt(dx * dx + dy * dy);

        const auto& beamFrame = m_BoomFrames[9];
        m_BoomRenderer.m_Transform.rotation = angle;
        m_BoomRenderer.m_Transform.scale = { length / imgSize.x*1.0f, 12.0f / imgSize.y };
        m_BoomRenderer.m_Transform.translation = { (mouthX + dx / 2.0f) - cameraX, mouthY + dy / 2.0f };
        m_BoomRenderer.DrawRect(beamFrame.x, beamFrame.y, beamFrame.w, beamFrame.h);

        // 🪨 2. 撞擊點的碎石 (沒有迴圈了！只跟著球跑，利用時間切換5和6產生揚塵感)
        m_BoomRenderer.m_Transform.rotation = 0.0f;
        float dustScaleMultiplier = 3.0f;
        int dustIdx = (static_cast<int>(m_PrecastTimer * 15.0f) % 2 == 0) ? 5 : 6;
        const auto& dustFrame = m_BoomFrames[dustIdx];
        m_BoomRenderer.m_Transform.scale = {
            (dustFrame.w * boomScale * dustScaleMultiplier) / imgSize.x,
            (dustFrame.h * boomScale * dustScaleMultiplier) / imgSize.y
        };
        m_BoomRenderer.m_Transform.translation = {currentFloorX - cameraX, floorY};
        m_BoomRenderer.DrawRect(dustFrame.x, dustFrame.y, dustFrame.w, dustFrame.h);



    // 🔮 3. 掃描前端的能量球
        const auto& orbFrame = m_BoomFrames[7];
        m_BoomRenderer.m_Transform.scale = { (orbFrame.w * boomScale) / imgSize.x, (orbFrame.h * boomScale) / imgSize.y };
        m_BoomRenderer.m_Transform.translation = {mouthX- (dir * 40.0f)-36.0f - cameraX, mouthY};
        m_BoomRenderer.DrawRect(orbFrame.x, orbFrame.y, orbFrame.w, orbFrame.h);


        m_BoomRenderer.m_Transform.scale = { (orbFrame.w * boomScale) / imgSize.x, (orbFrame.h * boomScale) / imgSize.y };
        m_BoomRenderer.m_Transform.translation = {currentFloorX - cameraX, floorY};
        m_BoomRenderer.DrawRect(orbFrame.x, orbFrame.y, orbFrame.w, orbFrame.h);
    }

    // ==========================================
    // 💥 開火階段 (延遲爆炸)：碎石與光柱共存亡！播完就清掉！
    // ==========================================
    if (m_CannonPhase == CannonPhase::FIRING) {
        float waveSpeed = m_CannonRange / m_CannonSec;
        int distIndex = 0;
        m_BoomRenderer.m_Transform.rotation = 0.0f;


        for (float distX = 0.0f; distX <= m_CannonRange; distX += spacing) {
            float localAge = m_FiringTimer - (distX / waveSpeed);

            if (localAge >= 0.0f) {
                int frameIdx = static_cast<int>(localAge * m_BoomFPS);
                if (frameIdx == 0 && distIndex % 2 == 0) {
                    Util::SFX(
                        RESOURCE_DIR "/music/laserboom.mp3"
                    ).Play();
                }
                // 💡 關鍵：只有在 frameIdx <= 4（動畫還沒結束前），才畫出碎石跟光柱！
                // 一旦超過 4，這個座標點就不會再畫任何東西，完美清掉！
                if (frameIdx <= 4) {
                    float effectX = mouthX + (dir * distX);



                    const auto& pillarFrame = m_BoomFrames[frameIdx];
                    m_BoomRenderer.m_Transform.scale = { (pillarFrame.w * boomScale) / imgSize.x, (pillarFrame.h * boomScale) / imgSize.y };
                    float pillarY = floorY + ((pillarFrame.h * boomScale) / 2.0f);
                    m_BoomRenderer.m_Transform.translation = {effectX - cameraX, pillarY};
                    m_BoomRenderer.DrawRect(pillarFrame.x, pillarFrame.y, pillarFrame.w, pillarFrame.h);
                }
            }
            distIndex++;
        }
    }

    // 確保離開時重置角度
    m_BoomRenderer.m_Transform.rotation = 0.0f;
}