#ifndef BASE_HPP
#define BASE_HPP
#include "Entity.hpp"
enum class CannonPhase {
    CHARGING, // 充能中
    READY,    // 充能完畢
    PRECAST,  // 前搖瞄準中
    FIRING    // 雷射轟炸中
};
class Base : public Entity {
public:
    Base(const Vector2& pos, int hp);
    void Update(float dt) override;
    void SetImage(const std::string& imgPath);
    // 傷害鎖：確保一發雷射只會扣一次血
    float GetCannonProgress() const { return m_CannonTimer / m_CannonCooldown; }
    bool IsCannonReady() const { return m_CannonPhase == CannonPhase::READY; }
    void FireCannon();

    // 給 BattleSystem 讀取的傷害觸發器
    bool HasDamageTrigger() const { return m_DamageTrigger; }
    void ConsumeDamageTrigger() { m_DamageTrigger = false; }
    // 🚀 新增：用來接收你寫好的 boom.imgcut 座標
    void InitBoomAnimation(const std::vector<SpriteFrame>& frames);

    // 🚀 新增：覆寫 Draw 來畫出全畫面爆炸！
    void Draw(float cameraX = 0.0f) override;
    float GetCannonRange() const { return m_CannonRange; }
    float GetCannonDamage() const { return m_CannonDamage; }
    void SetCannonDamage(float dmg) { m_CannonDamage = dmg; }
    // 🚀 新增 1：判斷是否正在開火中
    bool IsFiring() const { return m_CannonPhase == CannonPhase::FIRING; }

    // 🚀 新增 2：取得目前雷射波「推進到哪裡了」的絕對 X 座標
    float GetCurrentWaveX() const {
        float dir = m_IsPlayerTeam ? -1.0f : 1.0f;
        float mouthX = m_Position.x + (dir * 40.0f);
        float waveSpeed = m_CannonRange / m_CannonSec;
        float headDist = waveSpeed * m_FiringTimer;
        if (headDist > m_CannonRange) headDist = m_CannonRange; // 限制不超過極限距離
        return mouthX + (dir * headDist);
    }

    // 🚀 新增 3：名單管理，避免同一發雷射重複扣同一個人的血
    bool HasHit(const std::shared_ptr<Entity>& target) const {
        for (const auto& t : m_HitTargets) {
            if (t == target) return true;
        }
        return false;
    }
    void AddHitTarget(const std::shared_ptr<Entity>& target) { m_HitTargets.push_back(target); }
    void ClearHitTargets() { m_HitTargets.clear(); }
    // 🚀 新增：完美同步動畫與傷害的「動態爆炸判定」
    bool IsInActiveExplosion(float targetX) const {
        if (m_CannonPhase != CannonPhase::FIRING) return false;

        float dir = m_IsPlayerTeam ? -1.0f : 1.0f;
        float startX = m_Position.x + (dir * 40.0f);
        float relativeX = targetX - startX;

        // 1. 檢查方向對不對 (防止打到背後的敵人)
        if ((dir < 0 && relativeX > 0) || (dir > 0 && relativeX < 0)) return false;

        // 2. 檢查是否超出極限射程
        float dist = std::abs(relativeX);
        if (dist > m_CannonRange) return false;

        // 3. 🎯 核心神邏輯：計算目標所在位置的「在地時間」
        float waveSpeed = m_CannonRange / m_CannonSec;
        float localAge = m_FiringTimer - (dist / waveSpeed);

        if (localAge >= 0.0f) {
            int frameIdx = static_cast<int>(localAge * m_BoomFPS);
            // 💡 只有當這個座標的爆炸動畫「還沒播完 (<= 4)」時，才算踩在爆炸範圍內！
            if (frameIdx <= 4) {
                return true;
            }
        }
        return false; // 波浪還沒到，或是爆炸已經消散了
    }
private:
    CannonPhase m_CannonPhase = CannonPhase::CHARGING;

    // 充能設定
    float m_CannonCooldown = 3.0f;
    float m_CannonTimer = 0.0f;

    // 前搖設定 (未來放瞄準輔助線的時間)
    float m_PrecastDuration = 1.5f;
    float m_PrecastTimer = 0.0f;

    float m_FiringTimer = 0.0f;
    // 💥 修正 1 & 2：拿掉多段傷害設定，改為極限距離與單次開關
    float m_CannonRange = 1000.0f; // 💡 砲塔射程極限 (可以依據你的地圖大小微調
    float m_CannonSec = 3.5f;//彈頭 0.3 秒衝到極限距離
    float boomScale = 1.2f;
    float m_BoomFPS = 15.0f; // 爆炸動畫的播放速度 (可自由微調)

    bool m_DamageTrigger = false;   // 通知 BattleSystem 扣血的開關

    std::shared_ptr<Util::Image> m_BoomImage;
    Util::GameObject m_BoomRenderer;
    std::vector<SpriteFrame> m_BoomFrames;
    int m_BoomFrameIndex = 0;
    float m_BoomAnimTimer = 0.0f;
    float m_CannonDamage = 100.0f;//爆炸傷害
    std::vector<std::shared_ptr<Entity>> m_HitTargets;
};
#endif