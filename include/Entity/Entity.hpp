#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <memory>
#include <string>
#include <vector>
#include <cmath>
#include "Util/Image.hpp"
#include "Util/GameObject.hpp"
#include "GameConfig.hpp"
#include "Entity/UnitData.hpp"
#include <iostream>
#include "Util/SFX.hpp"
// 簡單的向量運算
struct Vector2 {
    float x = 0;
    float y = 0;
    Vector2() = default;
    Vector2(float _x, float _y) : x(_x), y(_y) {}
    float DistanceTo(const Vector2& other) const {
        return std::sqrt(std::pow(x - other.x, 2) + std::pow(y - other.y, 2));
    }
};

struct SpriteFrame {
    int x, y, w, h;
};

// ==========================================
// 🚀 新增：骨架系統核心資料結構
// ==========================================
// 單一零件 (例如：身體、獨立的腳尖)
struct AnimPart {
    SpriteFrame rect;     // 零件在 imgcut 裡的切塊座標
    float offsetX = 0.0f; // 相對中心點的 X 偏移
    float offsetY = 0.0f; // 相對中心點的 Y 偏移
};

// 單一影格 (包含這瞬間要同時畫出的所有零件)
struct AnimFrame {
    std::vector<AnimPart> parts;
};

enum class EntityState {
    IDLE,       // 閒置 (例如：基地)
    WALK,       // 走路
    ATTACK,     // 攻擊
    KNOCKBACK,  // 被擊退
    DEATH       // 死亡 (播放爆炸特效)
};

class Entity {
public:
    // ==========================================
    // 建構子
    // ==========================================
    Entity(const Vector2& pos, int hp, float speed, int damage, float range, float attackCd, int cost, float spawnCd)
        : m_Position(pos), m_HP(hp), m_MaxHP(hp), m_Speed(speed), m_AttackDamage(damage),
          m_AttackRange(range), m_AttackCooldown(attackCd), m_UnitCost(cost), m_SpawnCooldown(spawnCd) {}

    Entity(const Vector2& pos, int hp, float speed, int damage, float range, float attackCd, int cost, float spawnCd, const std::string& imgPath, int kb)
        : Entity(pos, hp, speed, damage, range, attackCd, cost, spawnCd) {
        m_ImagePath = imgPath;
        m_Image = std::make_shared<Util::Image>(m_ImagePath);
        m_MaxKBCount = kb;
    }

    Entity(const Vector2& pos, int hp, float speed, int damage, float range, float attackCd, int cost, float spawnCd, int rank, const std::string& imgPath, int kb)
        : Entity(pos, hp, speed, damage, range, attackCd, cost, spawnCd) {
        m_Rank = rank;
        m_ImagePath = imgPath;
        m_Image = std::make_shared<Util::Image>(m_ImagePath);
        m_MaxKBCount = kb;
    }

    virtual ~Entity() = default;

    // 每幀更新邏輯 (子類別實作移動方向)
    virtual void Update(float dt) = 0;

    // ==========================================
    // 動畫與狀態控制接口
    // ==========================================
    void SetState(EntityState newState) {
        if (m_CurrentState != newState) {
            m_CurrentState = newState;
            m_CurrentFrameIndex = 0; // 切換動作時，動畫從第 0 張開始播
            m_AnimationTimer = 0.0f;
        }
    }
    EntityState GetState() const { return m_CurrentState; }

    void SetAnimFPS(float fps) { m_FPS = fps; }
    void SetSpriteSheet(std::shared_ptr<Util::Image> sheet) { m_SpriteSheet = sheet; }

    // 💡 升級：現在全都接收 AnimFrame 陣列
    void SetIdleFrames(const std::vector<AnimFrame>& f) { m_IdleFrames = f; }
    void SetWalkFrames(const std::vector<AnimFrame>& f) { m_WalkFrames = f; }
    void SetAttackFrames(const std::vector<AnimFrame>& f) { m_AttackFrames = f; }
    void SetKnockbackFrames(const std::vector<AnimFrame>& f) { m_KnockbackFrames = f; }
    void SetDeathFrames(const std::vector<AnimFrame>& f) { m_DeathFrames = f; }

    // 播放動畫的核心邏輯
    void UpdateAnimation(float dt) {
        const auto& currentFrames = GetCurrentFrames();

        if (currentFrames.empty()) {
            if (m_CurrentState == EntityState::ATTACK || m_CurrentState == EntityState::KNOCKBACK) {
                m_AnimationTimer += dt;
                if (m_AnimationTimer >= 0.3f) {
                    SetState(EntityState::WALK); // 強制切回走路
                }
            }
            return;
        }

        m_AnimationTimer += dt;
        float frameInterval = 1.0f / m_FPS;

        if (m_AnimationTimer >= frameInterval) {
            m_AnimationTimer = 0.0f;
            m_CurrentFrameIndex++;
            if (m_CurrentState == EntityState::ATTACK && m_CurrentFrameIndex == m_AttackSoundFrame) {
                Util::SFX(m_AttackSoundPath).Play();
            }
            if (m_CurrentFrameIndex >= currentFrames.size()) {
                if (m_CurrentState == EntityState::WALK || m_CurrentState == EntityState::IDLE) {
                    m_CurrentFrameIndex = 0; // 無限循環
                } else if (m_CurrentState == EntityState::DEATH) {
                    m_CurrentFrameIndex = currentFrames.size() - 1; // 死亡停在最後一張
                } else {
                    SetState(EntityState::WALK); // 攻擊、擊退播完切回走路
                }
            }
        }
    }

    // ==========================================
    // 基礎戰鬥邏輯
    // ==========================================
    bool IsPlayerTeam() const { return m_IsPlayerTeam; }
    void SetTeam(bool isPlayer) { m_IsPlayerTeam = isPlayer; }
    bool IsAlive() const { return m_HP > 0; }
    bool IsAoE() const { return m_IsAoE; }
    void SetAoE(bool aoe) { m_IsAoE = aoe; }

    void TakeDamage(int dmg) {
        m_HP = std::max(0, m_HP - dmg);
        if (m_HP <= 0) {
            m_KnockbackTimer = m_KnockbackDuration;
            SetState(EntityState::DEATH);
            // 🎵 播放死亡音效
            Util::SFX(m_DeadSoundPath).Play();
            return;
        }
        Util::SFX(m_HitSoundPath).Play();

        if (m_HP > 0 && m_MaxKBCount > 0) {
            int hpPerKB = std::max(1, m_MaxHP / m_MaxKBCount);
            int expectedKBs = (m_MaxHP - m_HP) / hpPerKB;

            if (expectedKBs > m_CurrentKBs) {
                m_CurrentKBs = expectedKBs;
                m_KnockbackTimer = m_KnockbackDuration;
                m_AttackTimer = m_AttackCooldown;
                SetState(EntityState::KNOCKBACK);
            }
        }
    }

    bool UpdateKnockback(float dt) {
        if (m_KnockbackTimer > 0.0f) {
            m_KnockbackTimer -= dt;
            float kbDir = m_IsPlayerTeam ? 1.0f : -1.0f;
            m_Position.x += kbDir * m_KnockbackSpeed * dt;
            return true;
        }
        return false;
    }

    bool CheckCollision(const std::shared_ptr<Entity>& other) const {
        if (!other) return false;

        float myHalfW = m_Size.x / 2.0f;
        float myHalfH = m_Size.y / 2.0f;
        float oHalfW  = other->GetSize().x / 2.0f;
        float oHalfH  = other->GetSize().y / 2.0f;

        float myTop    = m_Position.y + myHalfH;
        float myBottom = m_Position.y - myHalfH;
        float oTop     = other->GetPosition().y + oHalfH;
        float oBottom  = other->GetPosition().y - oHalfH;
        bool yOverlap  = (myBottom < oTop) && (myTop > oBottom);

        float myLeft, myRight;
        if (m_IsPlayerTeam) {
            myLeft  = m_Position.x - myHalfW - m_AttackRange;
            myRight = m_Position.x + myHalfW;
        } else {
            myLeft  = m_Position.x - myHalfW;
            myRight = m_Position.x + myHalfW + m_AttackRange;
        }

        float oLeft  = other->GetPosition().x - oHalfW;
        float oRight = other->GetPosition().x + oHalfW;
        bool xOverlap = (myLeft < oRight) && (myRight > oLeft);
        return xOverlap && yOverlap;
    }

    bool CanAttack() const { return m_AttackTimer <= 0.0f; }
    void ResetAttackTimer() { m_AttackTimer = m_AttackCooldown; }
    void UpdateCooldown(float dt) {
        if (m_AttackTimer > 0.0f) m_AttackTimer -= dt;
    }

    const Vector2& GetPosition() const { return m_Position; }
    int GetHP() const { return m_HP; }
    int GetDamage() const { return m_AttackDamage; }
    float GetAttackRange() const { return m_AttackRange; }
    int GetUnitCost() const { return m_UnitCost; }
    void SetMoving(bool moving) { m_IsMoving = moving; }
    float GetSpawnCooldown() const { return m_SpawnCooldown; }
    std::string GetImgPath() const { return m_ImagePath; }
    int GetRank() const { return m_Rank; }
    void SetRank(int rank) { m_Rank = rank; }
    Vector2 GetSize() const { return m_Size; }
    void SetSize(const Vector2& size) { m_Size = size; }
    void SetSize(float w, float h) { m_Size.x = w; m_Size.y = h; }
    bool CanBeDeleted() const { return m_HP <= 0 && m_KnockbackTimer <= 0.0f; }

    // 💡 向下相容邏輯：如果子類別沒特別寫，就當作普通單圖貓，自動轉成無偏移的 AnimFrame
    virtual void InitAnimation(const std::vector<SpriteFrame>& allFrames) {
        if (allFrames.size() >= 3) {
            std::vector<AnimFrame> walk;

            // 明確宣告並推入，解決 C++ 類型推導錯誤
            for(int i = 0; i < 3; i++) {
                AnimFrame frame;
                frame.parts.push_back({allFrames[i], 0.0f, 0.0f});
                walk.push_back(frame);
            }

            SetWalkFrames(walk);
        }
    }
    // ==========================================
    // 🚀 繪製邏輯 (多重渲染骨架系統)
    // ==========================================
    virtual void Draw(float cameraX = 0.0f) {
        // ==========================================
        // 模式 A：如果有大圖 (Texture Atlas / 骨架模式)
        // ==========================================
        if (m_SpriteSheet) {
            const auto& currentFrames = GetCurrentFrames();

            if (!currentFrames.empty() && m_CurrentFrameIndex < currentFrames.size()) {
                const AnimFrame& frame = currentFrames[m_CurrentFrameIndex];
                m_Renderer.SetDrawable(m_SpriteSheet);
                auto sheetSize = m_SpriteSheet->GetSize();

                if (sheetSize.x > 0 && sheetSize.y > 0) {
                    // 💡 核心：跑迴圈，把這瞬間的所有零件全部疊加上去畫出來！
                    for (const AnimPart& part : frame.parts) {
                        float dynamicHalfHeight = part.rect.h / 2.0f;

                        // 動態中心對齊腳底 + 加上該零件專屬的偏移量
                        m_Renderer.m_Transform.translation = {
                            m_Position.x - cameraX + part.offsetX,
                            m_Position.y + dynamicHalfHeight + part.offsetY
                        };

                        // 抵銷巨大化，還原真實像素比例
                        m_Renderer.m_Transform.scale = {
                            (float)part.rect.w / sheetSize.x,
                            (float)part.rect.h / sheetSize.y
                        };

                        m_Renderer.DrawRect(part.rect.x, part.rect.y, part.rect.w, part.rect.h);
                    }
                }
            }
            return;
        }

        // ==========================================
        // 模式 B：如果沒有大圖 (單圖防呆模式)
        // ==========================================
        if (m_Image) {
            m_Renderer.SetDrawable(m_Image);
            float halfHeight = m_Size.y / 2.0f;
            m_Renderer.m_Transform.translation = {m_Position.x - cameraX, m_Position.y + halfHeight};

            auto imgSize = m_Image->GetSize();
            if (imgSize.x > 0 && imgSize.y > 0) {
                m_Renderer.m_Transform.scale = {m_Size.x / imgSize.x, m_Size.y / imgSize.y};
            }
            m_Renderer.Draw();
        }
    }

protected:
    Vector2 m_Position;
    int m_HP;
    int m_MaxHP;
    float m_AttackTimer = 0.0f;
    bool m_IsMoving = true;
    bool m_IsPlayerTeam = true;

    float m_Speed;
    int m_AttackDamage;
    float m_AttackRange;
    float m_AttackCooldown;
    int m_UnitCost;
    float m_SpawnCooldown;
    Vector2 m_Size = {GameConfig::CAT_SIZE_X, GameConfig::CAT_SIZE_Y};
    std::shared_ptr<Util::Image> m_Image;
    std::string m_ImagePath;
    Util::GameObject m_Renderer;
    int m_Rank;
    float m_KnockbackTimer = 0.0f;
    float m_KnockbackDuration = 0.3f;
    float m_KnockbackSpeed = 200.0f;
    int m_MaxKBCount = 0;
    int m_CurrentKBs = 0;
    bool m_IsAoE = false;

    EntityState m_CurrentState = EntityState::WALK;
    int m_CurrentFrameIndex = 0;
    float m_AnimationTimer = 0.0f;
    float m_FPS = 12.0f;

    std::shared_ptr<Util::Image> m_SpriteSheet;
    std::string m_AttackSoundPath = RESOURCE_DIR "/music/atk.mp3";
    std::string m_HitSoundPath    = RESOURCE_DIR "/music/gethit.mp3";
    std::string m_DeadSoundPath    =RESOURCE_DIR "/music/dead.mp3";
    int m_AttackSoundFrame = 0;
    // 💡 升級：統一使用 AnimFrame
    std::vector<AnimFrame> m_IdleFrames;
    std::vector<AnimFrame> m_WalkFrames;
    std::vector<AnimFrame> m_AttackFrames;
    std::vector<AnimFrame> m_KnockbackFrames;
    std::vector<AnimFrame> m_DeathFrames;

private:
    const std::vector<AnimFrame>& GetCurrentFrames() const {
        switch (m_CurrentState) {
            case EntityState::IDLE:      return m_IdleFrames;
            case EntityState::WALK:      return m_WalkFrames;
            case EntityState::ATTACK:    return m_AttackFrames;
            case EntityState::KNOCKBACK: return m_KnockbackFrames;
            case EntityState::DEATH:     return m_DeathFrames;
        }
        return m_IdleFrames;
    }
};

#endif