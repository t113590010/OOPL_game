from PIL import Image
import os

# 自動獲取當前 .py 檔案所在的目錄
base_path = os.path.dirname(os.path.abspath(__file__))

# 將資料夾設定為 .py 所在的目錄下
input_folder = os.path.join(base_path, "cat")
output_folder = os.path.join(base_path, "cat_Fixed")

os.makedirs(output_folder, exist_ok=True)

filename = "edi_f.png"
filepath = os.path.join(input_folder, filename)

print(f"程式目錄: {base_path}")
print(f"正在找尋檔案: {filepath}")

if os.path.exists(filepath):
    img = Image.open(filepath)
    img = img.convert("RGBA")
    
    # 存到 output_folder
    save_path = os.path.join(output_folder, filename)
    img.save(save_path)
    print("轉檔完成！檔案已存入: " + output_folder)
else:
    print("錯誤：依然找不到檔案，請確認檔案是否在: " + filepath)