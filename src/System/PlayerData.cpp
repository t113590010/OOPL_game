#include "PlayerData.hpp"
#include <fstream>
#include <iostream>
#include <filesystem>

// 取得絕對路徑的私有輔助函式
std::string GetSaveFilePath() {
    return std::string(RESOURCE_DIR) + "/../PlayerData/save.txt";
}

PlayerData* PlayerData::GetInstance() {
    static PlayerData instance;
    return &instance;
}

PlayerData::PlayerData() {
    // 初始資源配給
    m_XP = 0;
    m_CatFood = 1500;
    m_NormalTickets = 100;
    m_RareTickets = 50; // 🌟 預設給 100 張稀有券測試

    // 隊伍初始化為 10 格空位
    m_Deck.resize(10, UnitID::NONE);

    // 新手預設解鎖基礎貓，並預設裝備在第一格
    m_UnlockedCats[UnitID::CAT] = 1;
    m_Deck[0] = UnitID::CAT;
}

bool PlayerData::SpendXP(int amount) {
    if (m_XP >= amount) { m_XP -= amount; return true; }
    return false;
}

bool PlayerData::SpendCatFood(int amount) {
    if (m_CatFood >= amount) { m_CatFood -= amount; return true; }
    return false;
}

bool PlayerData::SpendNormalTickets(int amount) {
    if (m_NormalTickets >= amount) { m_NormalTickets -= amount; return true; }
    return false;
}

// 🌟 實作扣除稀有票券的功能
bool PlayerData::SpendRareTickets(int amount) {
    if (m_RareTickets >= amount) { m_RareTickets -= amount; return true; }
    return false;
}

void PlayerData::SetDeckUnit(int index, UnitID id) {
    if (index >= 0 && index < 10) {
        m_Deck[index] = id;
    }
}

void PlayerData::UnlockOrUpgradeCat(UnitID id) {
    m_UnlockedCats[id]++;
}

bool PlayerData::HasCat(UnitID id) const {
    return m_UnlockedCats.find(id) != m_UnlockedCats.end();
}

int PlayerData::GetCatLevel(UnitID id) const {
    auto it = m_UnlockedCats.find(id);
    if (it != m_UnlockedCats.end()) return it->second;
    return 0;
}

void PlayerData::AddToFridge(UnitID id) {
    m_Fridge.push_back(id);
}

void PlayerData::RemoveFromFridge(int index) {
    if (index >= 0 && index < static_cast<int>(m_Fridge.size())) {
        m_Fridge.erase(m_Fridge.begin() + index);
    }
}

// ----------------- 存檔寫入 -----------------
void PlayerData::SaveToFile() {
    std::string filePath = GetSaveFilePath();
    std::filesystem::path pathObj(filePath);

    if (pathObj.has_parent_path() && !std::filesystem::exists(pathObj.parent_path())) {
        std::filesystem::create_directories(pathObj.parent_path());
    }

    std::ofstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "存檔建立失敗！路徑有誤: " << filePath << std::endl;
        return;
    }

    // 🌟 1. 存貨幣 (補上 m_RareTickets)
    file << m_XP << " " << m_CatFood << " " << m_NormalTickets << " " << m_RareTickets << "\n";

    // 2. 存隊伍
    file << m_Deck.size() << "\n";
    for (UnitID id : m_Deck) {
        file << static_cast<int>(id) << " ";
    }
    file << "\n";

    // 3. 存圖鑑
    file << m_UnlockedCats.size() << "\n";
    for (const auto& pair : m_UnlockedCats) {
        file << static_cast<int>(pair.first) << " " << pair.second << "\n";
    }

    // 4. 存冰箱
    file << m_Fridge.size() << "\n";
    for (UnitID id : m_Fridge) {
        file << static_cast<int>(id) << " ";
    }
    file << "\n";

    file.close();
}

// ----------------- 存檔讀取 -----------------
bool PlayerData::LoadFromFile() {
    std::string filePath = GetSaveFilePath();
    std::ifstream file(filePath);

    if (!file.is_open()) return false;

    // 🌟 2. 讀取貨幣 (補上 m_RareTickets 讀取，舊存檔會讀取失敗並回傳 false)
    if (!(file >> m_XP >> m_CatFood >> m_NormalTickets >> m_RareTickets)) return false;

    // 3. 讀取隊伍
    size_t deckSize = 0;
    if (!(file >> deckSize) || deckSize > 10) return false;

    m_Deck.resize(deckSize);
    for (size_t i = 0; i < deckSize; ++i) {
        int idRaw;
        if (!(file >> idRaw)) return false;
        m_Deck[i] = static_cast<UnitID>(idRaw);
    }

    // 4. 讀取圖鑑
    size_t unlockSize = 0;
    if (!(file >> unlockSize) || unlockSize > 1000) return false;

    m_UnlockedCats.clear();
    for (size_t i = 0; i < unlockSize; ++i) {
        int idRaw, level;
        if (!(file >> idRaw >> level)) return false;
        m_UnlockedCats[static_cast<UnitID>(idRaw)] = level;
    }

    // 5. 讀取冰箱
    size_t fridgeSize = 0;
    if (!(file >> fridgeSize) || fridgeSize > 1000) return false;

    m_Fridge.clear();
    for (size_t i = 0; i < fridgeSize; ++i) {
        int idRaw;
        if (!(file >> idRaw)) return false;
        m_Fridge.push_back(static_cast<UnitID>(idRaw));
    }

    file.close();
    return true;
}