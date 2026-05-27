from PIL import Image
import os

def split_gif_to_png(input_path, output_folder, crop_rect):
    if not os.path.exists(output_folder):
        os.makedirs(output_folder)
        
    img = Image.open(input_path)
    
    try:
        frame_idx = 0
        while True:
            # 1. 轉成 RGBA
            frame = img.copy().convert("RGBA")
            # 2. 裁切
            frame = frame.crop(crop_rect)
            # 3. 儲存為 PNG (PNG 對透明度的支援完美，不會有調色盤錯誤)
            frame.save(f"{output_folder}/frame_{frame_idx}.png", "PNG")
            
            frame_idx += 1
            img.seek(img.tell() + 1)
    except EOFError:
        pass
    print(f"成功！已將 {frame_idx} 張影格存至 {output_folder}")

# 確保這四個數值都在原始圖片的範圍內
rect = (0, 0, 558, 238) 
split_gif_to_png("normalSpin.gif", "frames_output", rect)