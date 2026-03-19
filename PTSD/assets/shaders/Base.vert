#version 410 core

layout(location = 0) in vec2 vertPosition;
layout(location = 1) in vec2 vertUv;

layout(location = 0) out vec2 uv;

layout(std140) uniform Matrices {
    mat4 model;
    mat4 viewProjection;
};

uniform int u_UseSourceRect;
uniform vec4 u_SourceRect; // 裡面裝著 x, y, w, h
uniform vec2 u_TexSize;    // 裡面裝著圖片的原始寬高

void main() {
    // Reference from
    // https://github.com/NOOBDY/Indigo/blob/f31c7ef82c610d8e91214892a7a1e3f860ba4aaa/assets/shaders/base_pass.vert#L21-L22
    gl_Position = viewProjection * model * vec4(vertPosition, 0, 1);

   if (u_UseSourceRect == 1) {
           // 算出要縮放的比例 (例如只切寬度一半，就是 0.5)
           vec2 scale = vec2(u_SourceRect.z / u_TexSize.x, u_SourceRect.w / u_TexSize.y);

           // 算出要平移的起點座標
           vec2 offset = vec2(u_SourceRect.x / u_TexSize.x, u_SourceRect.y / u_TexSize.y);

           // 套用縮放與平移到原本的 UV 座標上
           uv = vertUv * scale + offset;
       } else {
           // 沒開裁切的時候，就維持原樣
           uv = vertUv;
       }
}
