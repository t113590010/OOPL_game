import os

def resort_frames(folder_path):
    if not os.path.exists(folder_path):
        return

    # 1. 取得所有檔案並排序
    files = sorted([f for f in os.listdir(folder_path) if f.startswith("frame_") and f.endswith(".png")])
    
    # 2. 建立更名清單 (使用不補0的格式)
    # 我們先建立一個映射，把舊檔名轉成新的 frame_0.png 格式
    tasks = []
    new_index = 0
    for old_name in files:
        new_name = f"frame_{new_index}.png" # 這邊去掉了 :03d
        tasks.append((old_name, new_name))
        new_index += 1
    
    # 3. 為了避免檔名衝突，我們先把所有檔案「暫時」改成 .tmp 檔
    # 這樣保證改名過程中不會撞名
    for old_name, _ in tasks:
        os.rename(os.path.join(folder_path, old_name), os.path.join(folder_path, old_name + ".tmp"))
    
    # 4. 再把 .tmp 檔改成你要的最終檔名
    for old_name, new_name in tasks:
        os.rename(os.path.join(folder_path, old_name + ".tmp"), os.path.join(folder_path, new_name))
        print(f"Renamed: {old_name} -> {new_name}")

    print(f"完成！共處理了 {new_index} 個影格。")

resort_frames("frames_output")