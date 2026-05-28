#ifndef PLAYER_DATA_HPP
#define PLAYER_DATA_HPP

#include "Entity/UnitID.hpp"
#include <vector>
#include <unordered_map>
#include <string>

class PlayerData {
public:
    // 🚀 新增：定義貓咪的雙軌等級結構體
    struct CatLevel {
        int base = 1; // 基礎等級 (吃 XP 升級)
        int plus = 0; // 額外等級 (轉蛋吃重複貓)
    };

    // 單例模式：全域唯一存取點
    static PlayerData* GetInstance();

    // ================= 存檔與讀檔 =================
    void SaveToFile();
    bool LoadFromFile();

    // ================= 資源與貨幣 =================
    int GetXP() const { return m_XP; }
    void AddXP(int amount) { m_XP += amount; }
    bool SpendXP(int amount);

    int GetCatFood() const { return m_CatFood; }
    void AddCatFood(int amount) { m_CatFood += amount; }
    bool SpendCatFood(int amount);

    // 一般票券
    int GetNormalTickets() const { return m_NormalTickets; }
    void AddNormalTickets(int amount) { m_NormalTickets += amount; }
    bool SpendNormalTickets(int amount);

    // 稀有票券
    int GetRareTickets() const { return m_RareTickets; }
    void AddRareTickets(int amount) { m_RareTickets += amount; }
    bool SpendRareTickets(int amount);

    // ================= 隊伍 (Deck) =================
    const std::vector<UnitID>& GetDeck() const { return m_Deck; }
    void SetDeckUnit(int index, UnitID id);

    // ================= 圖鑑與解鎖 =================
    // 🚀 分流管理：區分首次解鎖、吃 XP 升級、轉蛋加值
    void UnlockCat(UnitID id);           // 首次抽到，解鎖 1 等 +0
    void UpgradeCatBaseLevel(UnitID id); // 吃 XP 提升基礎等級
    void AddCatPlusLevel(UnitID id);     // 冰箱使用重複貓，提升額外等級
    void UnlockOrUpgradeCat(UnitID id);
    bool HasCat(UnitID id) const;
    CatLevel GetCatLevel(UnitID id) const; // 🚀 改回傳 CatLevel 結構體
    std::vector<UnitID> GetUnlockedCatsList() const;

    // ================= 冰箱 (Storage) =================
    void AddToFridge(UnitID id);
    const std::vector<UnitID>& GetFridge() const { return m_Fridge; }
    void RemoveFromFridge(int index);

private:
    PlayerData(); // 隱藏建構子
    ~PlayerData() = default;

    int m_XP = 0;
    int m_CatFood = 0;
    int m_NormalTickets = 0;
    int m_RareTickets = 0;

    // 隊伍固定 10 格
    std::vector<UnitID> m_Deck;

    // 🚀 升級：將儲存型態從 int 改為 CatLevel 結構體
    std::unordered_map<UnitID, CatLevel> m_UnlockedCats;

    // 冰箱內的貓咪 (抽到但還沒使用的)
    std::vector<UnitID> m_Fridge;
};

#endif