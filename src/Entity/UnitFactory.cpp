#include "Entity/UnitFactory.hpp"
#include "Entity/Cats/Cat.hpp"
#include "Entity/Cats/LongLegCat.hpp"
#include "Entity/Cats/AxeCat.hpp"

#include "Entity/Enemies/Enemy.hpp" // 💡 記得補上
#include "Entity/Enemies/GAY.hpp" // 💡 記得補上
#include "Entity/Enemies/Snack.hpp" // 💡 記得補上
#include "Entity/Enemies/p3.hpp" // 💡 記得補上
#include "Entity/Enemies/bighead.hpp" // 💡 記得補上


#include "Entity/UnitData.hpp"     // 💡 核心：引入神級資料庫
#include <filesystem> // 💡 核心：引入檔案系統，讓 C++ 能檢查硬碟裡的檔案存不存在！
#include <iostream>
#include <fstream>
#include <sstream>

#include "ninja_cat.hpp"

// ==========================================
// 💡 只要加這一行「前置宣告」，下面的程式碼就完全不用搬家！
// ==========================================
std::vector<SpriteFrame> ParseImgCut(const std::string& filepath);

// ==========================================
// 💡 超級工具：自動尋找並載入連續圖片
// ==========================================
std::vector<std::shared_ptr<Util::Image>> LoadAnimationFrames(const std::string& basePath, const std::string& action) {
    std::vector<std::shared_ptr<Util::Image>> frames;

    // 從 1 開始找，找不到就立刻停止
    for (int i = 1; ; ++i) {
        // 組合出檔名，例如： "RESOURCE_DIR/img/cat" + "_walk_" + "1" + ".png"
        std::string fullPath = basePath + "_" + action + "_" + std::to_string(i) + ".png";

        if (std::filesystem::exists(fullPath)) {
            frames.push_back(std::make_shared<Util::Image>(fullPath));
        } else {
            break; // 只要序號斷掉 (例如有 1, 2，但找不到 3)，代表這組動作讀取完畢，跳出迴圈！
        }
    }
    return frames;
}
// 💡 Getter 群現在變得超級乾淨，全部去 UnitData 拿
int UnitFactory::GetUnitCost(UnitID id) {
    return UnitData::Get(id).cost;
}

float UnitFactory::GetUnitSpawnCooldown(UnitID id) {
    return UnitData::Get(id).spawnCd;
}

int UnitFactory::GetUnitRank(UnitID id) {
    return UnitData::Get(id).rank;
}

std::string UnitFactory::GetUnitIconPath(UnitID id) {
    // 優先回傳頭像路徑，如果沒有頭像就拿場上圖片路徑
    auto& stats = UnitData::Get(id);
    return stats.iconPath.empty() ? stats.imgPath : stats.iconPath;
}

// 實體產兵區
std::shared_ptr<Entity> UnitFactory::CreateUnit(UnitID id, float x, float y, bool isPlayer) {
    std::shared_ptr<Entity> newUnit = nullptr;

    switch (id) {
        case UnitID::CAT:           newUnit = std::make_shared<Cat>(Vector2{x, y}); break;
        case UnitID::LONG_LEG_CAT:  newUnit = std::make_shared<LongLegCat>(Vector2{x, y}); break;
        case UnitID::BASIC_ENEMY:   newUnit = std::make_shared<Enemy>(Vector2{x, y}); break;
        case UnitID::GAY:           newUnit = std::make_shared<GAY>(Vector2{x, y}); break;
        case UnitID::AXE_CAT:       newUnit = std::make_shared<AxeCat>(Vector2{x, y}); break;
        case UnitID::Snack:       newUnit = std::make_shared<Snack>(Vector2{x, y}); break;
        case UnitID::p3:       newUnit = std::make_shared<p3>(Vector2{x, y}); break;
        case UnitID::bighead:       newUnit = std::make_shared<bighead>(Vector2{x, y}); break;
        case UnitID::ninja_cat:       newUnit = std::make_shared<ninja_cat>(Vector2{x, y}); break;

        default: return nullptr;
    }

    if (newUnit) {
        newUnit->SetTeam(isPlayer);
        auto& stats = UnitData::Get(id);

        if (!stats.imgPath.empty()) {
            std::string path = stats.imgPath;

            size_t lastSlash = path.find_last_of("/\\");
            std::string directory = path.substr(0, lastSlash);
            std::string filename = path.substr(lastSlash + 1);

            size_t dotPos = filename.find_last_of(".");
            if (dotPos != std::string::npos) filename = filename.substr(0, dotPos);

            size_t parentSlash = directory.find_last_of("/\\");
            std::string baseDir = directory.substr(0, parentSlash);

            // 💡 修正後的最穩路徑，直接對應你的資料夾
            std::string imgcutPath = baseDir + "/imgcut/" + filename + ".imgcut";

            if (std::filesystem::exists(imgcutPath)) {
                // 讀取大圖並丟給 Entity
                std::shared_ptr<Util::Image> sheet = std::make_shared<Util::Image>(stats.imgPath);
                newUnit->SetSpriteSheet(sheet);

                auto allFrames = ParseImgCut(imgcutPath);

                if (!allFrames.empty()) {
                    // 🚀 重頭戲：呼叫該實體的專屬邏輯去切圖！
                    newUnit->InitAnimation(allFrames);
                }
            }
        }

        newUnit->SetState(EntityState::WALK);
        // 如果實體內部沒有在 InitAnimation 設定 FPS，這裡給個預設值
        if (newUnit->GetState() == EntityState::WALK) {
             // 確保至少有畫面能跑
        }
    }
    return newUnit;
}
// 💡 貓戰專屬：.imgcut 解析器！
std::vector<SpriteFrame> ParseImgCut(const std::string& filepath) {
    std::vector<SpriteFrame> frames;
    std::ifstream file(filepath);

    if (!file.is_open()) {
        std::cerr << "Failed to open imgcut file: " << filepath << "\n";
        return frames;
    }

    std::string line;
    // 1. 跳過前三行 ([imgcut], 0, 000_c.png)
    std::getline(file, line);
    std::getline(file, line);
    std::getline(file, line);

    // 2. 讀取第 4 行 (總共有幾個切塊)
    std::getline(file, line);
    int frameCount = std::stoi(line);

    // 3. 開始迴圈讀取每一個切塊的 x, y, w, h
    for (int i = 0; i < frameCount; ++i) {
        if (!std::getline(file, line)) break;

        std::stringstream ss(line);
        std::string token;
        SpriteFrame frame;

        // 用逗號 (,) 當作分隔符號，連續讀取 4 個數字
        std::getline(ss, token, ','); frame.x = std::stoi(token);
        std::getline(ss, token, ','); frame.y = std::stoi(token);
        std::getline(ss, token, ','); frame.w = std::stoi(token);
        std::getline(ss, token, ','); frame.h = std::stoi(token);

        // 💡 備註：如果這行後面還有逗號和日文註解 (例如 4の影無しパターン)，
        // 因為我們只讀了前 4 個 token，後面的會被直接安全忽略！

        frames.push_back(frame);
    }

    return frames;
}