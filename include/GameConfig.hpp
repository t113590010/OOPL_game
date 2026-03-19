#pragma once

namespace GameConfig {
    constexpr float MAP_SIZE = 800.0f;

    constexpr float ENEMY_BASE_X = - MAP_SIZE;
    constexpr float PLAYER_BASE_X = MAP_SIZE;
    constexpr float BASE_Y = -100.0f;
    constexpr int   BASE_HP = 1000;
    constexpr float MONEY_TEXT_X = 0.7f;
    constexpr float MONEY_TEXT_Y = 0.8f;

    constexpr float CAMERA_OFFSET = -500.0f;
    constexpr float CAMERA_MAX_X = PLAYER_BASE_X + CAMERA_OFFSET;
    constexpr float CAMERA_MIN_X = ENEMY_BASE_X - CAMERA_OFFSET;
    constexpr float CAMERA_SPEED = 800.0f;

    constexpr float SPAWN_OFFSET_X = 80.0f;
    constexpr float SPAWN_OFFSET_Y = 0.0f;


    // === 新增：單位尺寸設定 (寬度, 高度) ===
    constexpr float BASE_SIZE_X = 150.0f;  // 基地通常比較大
    constexpr float BASE_SIZE_Y = 200.0f;

    constexpr float CAT_SIZE_X = 50.0f;   // 貓咪的大小
    constexpr float CAT_SIZE_Y = 50.0f;

    constexpr float ENEMY_SIZE_X = 50.0f; // 敵人的大小
    constexpr float ENEMY_SIZE_Y = 50.0f;

    // === 新增：經濟系統設定 ===
    constexpr int INITIAL_MONEY = 0;       // 開局多少錢
    constexpr int MAX_MONEY_LEVEL_1 = 1000; // 錢包等級 1 的上限
    constexpr int MONEY_GROWTH_SPEED = 50; // 每秒自動增加多少錢 (工作效率)

    // === 新增：兵種價格 ===
    constexpr float CAT_COST = 75.0f;           // 生一隻貓要 75 元
}