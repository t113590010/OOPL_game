# OOPL Game Project

This is a game project built upon the [PTSD](https://github.com/ntut-open-source-club/practical-tools-for-simple-design) framework template for students taking OOPL2024s.

##  遊戲簡介 (Game Description)
這是一款橫向卷軸的貓咪元素塔防遊戲。玩家需要分配金錢、點擊介面按鈕來召喚不同兵種，擊退敵人並摧毀敵方主堡以獲得勝利。


## ️ 操作說明 (Controls)
* **滑鼠左鍵**：點擊 UI 介面按鈕以生成單位或升級。
* **方向鍵 `←` `→`**：左右移動遊戲畫面。
* **數字鍵 `0` ~ `9`**：快速召喚對應的貓咪單位。
* **`Enter`**：戰鬥結束後返回主畫面 (Home)。
* **`Esc`**：直接退出遊戲。

---

## ️ 常見問題與環境排錯 (Troubleshooting)

如果在編譯或拉取專案時遇到問題，請參考以下解法：

### 1. CMake 版本異常 (CLion)
如果在 CLion 編譯時跳出 CMake 版本不符的錯誤：
1. 點擊頂部選單：`File` -> `Settings` -> `Build, Execution, Deployment` -> `CMake`
2. 在右側面板找到 **CMake options** 欄位。
3. 輸入以下指令並套用：
   ```text
   -DCMAKE_POLICY_VERSION_MINIMUM=3.5