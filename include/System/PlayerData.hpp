#ifndef PLAYER_DATA_HPP
#define PLAYER_DATA_HPP

#include "Entity/UnitID.hpp"
#include <vector>
#include <unordered_map>
#include <string>

class PlayerData {
public:
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

    // 🌟 補上稀有票券的 Get / Add / Spend
    int GetRareTickets() const { return m_RareTickets; }
    void AddRareTickets(int amount) { m_RareTickets += amount; }
    bool SpendRareTickets(int amount);

    // ================= 隊伍 (Deck) =================
    const std::vector<UnitID>& GetDeck() const { return m_Deck; }
    void SetDeckUnit(int index, UnitID id);

    // ================= 圖鑑與解鎖 =================
    void UnlockOrUpgradeCat(UnitID id);
    bool HasCat(UnitID id) const;
    int GetCatLevel(UnitID id) const;

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
    int m_RareTickets = 0; // 🌟 稀有票券變數

    // 隊伍固定 10 格
    std::vector<UnitID> m_Deck;

    // 已解鎖貓咪 (Key: ID, Value: 等級)
    std::unordered_map<UnitID, int> m_UnlockedCats;

    // 冰箱內的貓咪 (抽到但還沒使用的)
    std::vector<UnitID> m_Fridge;
};

#endif