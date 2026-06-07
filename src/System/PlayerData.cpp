#include "PlayerData.hpp"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <algorithm>

std::string GetSaveFilePath() {
    return std::string(RESOURCE_DIR) + "/../PlayerData/save.txt";
}

PlayerData* PlayerData::GetInstance() {
    static PlayerData instance;
    return &instance;
}

PlayerData::PlayerData() {
    m_XP = 0;
    m_CatFood = 999999;
    m_NormalTickets = 100;
    m_RareTickets = 50;

    m_BgmVolumeLevel = 3;
    m_SfxVolumeLevel = 3;

    m_Deck.resize(10, UnitID::NONE);

    // 🚀 新手預設解鎖基礎貓：基礎 1 等，加值 +0
    m_UnlockedCats[UnitID::CAT] = CatLevel{1, 0};
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

bool PlayerData::SpendRareTickets(int amount) {
    if (m_RareTickets >= amount) { m_RareTickets -= amount; return true; }
    return false;
}

void PlayerData::SetDeckUnit(int index, UnitID id) {
    if (index >= 0 && index < 10) {
        m_Deck[index] = id;
    }
}

// 🚀 實作全新的解鎖與升級機制
void PlayerData::UnlockCat(UnitID id) {
    if (!HasCat(id)) {
        m_UnlockedCats[id] = CatLevel{1, 0}; // 首次解鎖，1等+0
    }
}

void PlayerData::UpgradeCatBaseLevel(UnitID id) {
    if (HasCat(id)) {
        m_UnlockedCats[id].base++; // 吃 XP 升級基礎等級
    }
}

void PlayerData::AddCatPlusLevel(UnitID id) {
    if (HasCat(id)) {
        m_UnlockedCats[id].plus++; // 轉蛋重複，提升額外等級
    }
}

bool PlayerData::HasCat(UnitID id) const {
    return m_UnlockedCats.find(id) != m_UnlockedCats.end();
}

// 🚀 回傳完整的結構體物件，若無此貓則回傳預設防呆
PlayerData::CatLevel PlayerData::GetCatLevel(UnitID id) const {
    auto it = m_UnlockedCats.find(id);
    if (it != m_UnlockedCats.end()) return it->second;
    return CatLevel{1, 0};
}

void PlayerData::AddToFridge(UnitID id) {
    m_Fridge.push_back(id);
}

void PlayerData::RemoveFromFridge(int index) {
    if (index >= 0 && index < static_cast<int>(m_Fridge.size())) {
        m_Fridge.erase(m_Fridge.begin() + index);
    }
}

std::vector<UnitID> PlayerData::GetUnlockedCatsList() const {
    std::vector<UnitID> list;
    for (const auto& pair : m_UnlockedCats) {
        list.push_back(pair.first);
    }
    std::sort(list.begin(), list.end());
    return list;
}

// ================= 關卡進度 =================

int PlayerData::GetMaxUnlockedStage() const {
    return m_MaxUnlockedStage;
}

bool PlayerData::IsStageUnlocked(int stageID) const {
    return stageID <= m_MaxUnlockedStage;
}

void PlayerData::ClearStage(int stageID) {

    // 打完目前最高解鎖關卡
    // 才解鎖下一關

    if (stageID >= m_MaxUnlockedStage) {
        m_MaxUnlockedStage = stageID + 1;
    }
}

void PlayerData::SetBgmVolumeLevel(int level) {
    if (level < 0) {
        level = 0;
    }

    if (level > 3) {
        level = 3;
    }

    m_BgmVolumeLevel = level;
}

void PlayerData::SetSfxVolumeLevel(int level) {
    if (level < 0) {
        level = 0;
    }

    if (level > 3) {
        level = 3;
    }

    m_SfxVolumeLevel = level;
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

    // 1. 存貨幣
    file << m_XP << " " << m_CatFood << " " << m_NormalTickets << " " << m_RareTickets << "\n";

    // 2. 存隊伍
    file << m_Deck.size() << "\n";
    for (UnitID id : m_Deck) {
        file << static_cast<int>(id) << " ";
    }
    file << "\n";

    // 3. 存圖鑑 (🚀 調整：寫入 每個ID 的 基礎等級 與 額外等級)
    file << m_UnlockedCats.size() << "\n";
    for (const auto& pair : m_UnlockedCats) {
        file << static_cast<int>(pair.first) << " " << pair.second.base << " " << pair.second.plus << "\n";
    }

    // 4. 存冰箱
    file << m_Fridge.size() << "\n";

    for (UnitID id : m_Fridge) {
        file << static_cast<int>(id) << " ";
    }

    file << "\n";

    // 5. 存關卡進度
    file << m_MaxUnlockedStage << "\n";

    // 6. 存音量設定
    file << m_BgmVolumeLevel << " " << m_SfxVolumeLevel << "\n";

    file.close();
}

// ----------------- 存檔讀取 -----------------
bool PlayerData::LoadFromFile() {
    std::string filePath = GetSaveFilePath();
    std::ifstream file(filePath);

    if (!file.is_open()) return false;

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

    // 4. 讀取圖鑑 (🚀 調整：讀取 每個ID 的 基礎等級 與 額外等級)
    size_t unlockSize = 0;
    if (!(file >> unlockSize) || unlockSize > 1000) return false;

    m_UnlockedCats.clear();
    for (size_t i = 0; i < unlockSize; ++i) {
        int idRaw, baseLvl, plusLvl;
        if (!(file >> idRaw >> baseLvl >> plusLvl)) return false;
        m_UnlockedCats[static_cast<UnitID>(idRaw)] = CatLevel{baseLvl, plusLvl};
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
    // ========================
    // 關卡進度
    // ========================

    // 舊版存檔沒有這一行
    // 所以讀不到時給預設值

    if (!(file >> m_MaxUnlockedStage)) {
        m_MaxUnlockedStage = 1;
    }

    // ========================
    // 音量設定
    // ========================
    if (!(file >> m_BgmVolumeLevel >> m_SfxVolumeLevel)) {
        m_BgmVolumeLevel = 3;
        m_SfxVolumeLevel = 3;
    }

    SetBgmVolumeLevel(m_BgmVolumeLevel);
    SetSfxVolumeLevel(m_SfxVolumeLevel);

    file.close();
    return true;
}