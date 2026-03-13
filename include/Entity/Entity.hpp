#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <memory>
#include <string>
#include <cmath>
#include "Util/Image.hpp"
#include "Util/GameObject.hpp"
#include "GameConfig.hpp"
#include "Entity/UnitData.hpp"
#include <iostream>
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

class Entity {
public:
    // 在 Entity.hpp 裡面
    Entity(const Vector2& pos, int hp, float speed, int damage, float range, float attackCd, int cost, float spawnCd)
        : m_Position(pos), m_HP(hp), m_MaxHP(hp), m_Speed(speed), m_AttackDamage(damage),
          m_AttackRange(range), m_AttackCooldown(attackCd), m_UnitCost(cost), m_SpawnCooldown(spawnCd) {}

    // 💡 方案 B：新增的建構子 (給 Cat 和 LongLegCat 使用)
    Entity(const Vector2& pos, int hp, float speed, int damage, float range, float attackCd, int cost, float spawnCd, const std::string& imgPath)
        : Entity(pos, hp, speed, damage, range, attackCd, cost, spawnCd) { // 呼叫上面的建構子
        m_ImagePath = imgPath;
        m_Image = std::make_shared<Util::Image>(m_ImagePath);

    }

    Entity(const Vector2& pos, int hp, float speed, int damage, float range, float attackCd, int cost, float spawnCd, int rank, const std::string& imgPath)
        : Entity(pos, hp, speed, damage, range, attackCd, cost, spawnCd) {
        m_Rank = rank;
        m_ImagePath = imgPath;
        m_Image = std::make_shared<Util::Image>(m_ImagePath);


    }

    virtual ~Entity() = default;

    // 每幀更新邏輯 (子類別實作移動方向)
    virtual void Update(float dt) = 0;

    // 基礎戰鬥邏輯
    bool IsPlayerTeam() const { return m_IsPlayerTeam; }
    void SetTeam(bool isPlayer) { m_IsPlayerTeam = isPlayer; }
    bool IsAlive() const { return m_HP > 0; }

    void TakeDamage(int dmg) {
        m_HP = std::max(0, m_HP - dmg);
    }

    // 碰撞與攻擊判定
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

        // 3. X 軸判定 (把射程方向修正回來！)
        float myLeft, myRight;
        if (m_IsPlayerTeam) {
            // 💡 貓咪是往左走的，所以射程要減在左邊！
            myLeft  = m_Position.x - myHalfW - m_AttackRange;
            myRight = m_Position.x + myHalfW;
        } else {
            // 💡 敵人是往右走的，所以射程要加在右邊！
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

    // Getter 群：給 System 呼叫
    const Vector2& GetPosition() const { return m_Position; }
    int GetHP() const { return m_HP; }
    int GetDamage() const { return m_AttackDamage; }
    float GetAttackRange() const { return m_AttackRange; }
    int GetUnitCost() const { return m_UnitCost; }
    void SetMoving(bool moving) { m_IsMoving = moving; }
    float GetSpawnCooldown() const { return m_SpawnCooldown; }
    std::string GetImgPath()const { return  m_ImagePath  ;}
    int GetRank() const { return m_Rank; }
    void SetRank(int rank) { m_Rank = rank; }
    Vector2 GetSize() const { return m_Size; }
    void SetSize(const Vector2& size) { m_Size = size; }
    void SetSize(float w, float h) {
        m_Size.x = w;
        m_Size.y = h;
    }

    // 繪製邏輯 (考量相機偏移)
    // 繪製邏輯 (考量相機偏移)
    virtual void Draw(float cameraX = 0.0f) {
        if (m_Image) {
            m_Renderer.SetDrawable(m_Image);

            // 💡 核心修改：算出半個身高
            float halfHeight = m_Size.y / 2.0f;

            // 💡 把 Y 座標「往上拉」半個身高
            // 這樣 m_Position.y 就會剛好是這隻貓的「腳底板」位置
            m_Renderer.m_Transform.translation = {m_Position.x - cameraX, m_Position.y + halfHeight};

            auto imgSize = m_Image->GetSize();
            if (imgSize.x > 0 && imgSize.y > 0) {
                m_Renderer.m_Transform.scale = {m_Size.x / imgSize.x, m_Size.y / imgSize.y};
            }
            m_Renderer.Draw();
        }
    }

protected:
    // 動態狀態
    Vector2 m_Position;
    int m_HP;
    int m_MaxHP;
    float m_AttackTimer = 0.0f;
    bool m_IsMoving = true;
    bool m_IsPlayerTeam = true;

    // 靜態屬性 (由子類別決定)
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
    int m_Rank ;
};

#endif