from PIL import Image
import os

input_folder = "NormalSpin"        # 你原本的資料夾名稱
output_folder = "NormalSpin_Fixed" # 轉檔後存到新資料夾

os.makedirs(output_folder, exist_ok=True)

for i in range(69):
    filename = f"__-{i}.png"
    filepath = os.path.join(input_folder, filename)
    
    if os.path.exists(filepath):
        # 🌟 讀取圖片，並強制轉換為 32-bit RGBA 全彩格式
        img = Image.open(filepath)
        img = img.convert("RGBA") 
        img.save(os.path.join(output_folder, filename))

print("轉檔完成！快去把新圖丟進遊戲裡！")