#include "Util/Image.hpp"

#include "Util/Logger.hpp"
#include "pch.hpp"

#include "Core/Texture.hpp"
#include "Core/TextureUtils.hpp"

#include "Util/MissingTexture.hpp"
#include "Util/TransformUtils.hpp"

#include "config.hpp"
#include <glm/fwd.hpp>

std::shared_ptr<SDL_Surface> LoadSurface(const std::string &filepath) {
    auto surface = std::shared_ptr<SDL_Surface>(IMG_Load(filepath.c_str()),
                                                SDL_FreeSurface);

    if (surface == nullptr) {
        surface = {GetMissingTextureSDLSurface(), SDL_FreeSurface};
        LOG_ERROR("Failed to load image: '{}'", filepath);
        LOG_ERROR("{}", IMG_GetError());
    }

    return surface;
}

namespace Util {
Image::Image(const std::string &filepath)
    : m_Path(filepath) {
    if (s_Program == nullptr) {
        InitProgram();
    }
    if (s_VertexArray == nullptr) {
        InitVertexArray();
    }

    m_UniformBuffer = std::make_unique<Core::UniformBuffer<Core::Matrices>>(
        *s_Program, "Matrices", 0);

    auto surface = s_Store.Get(filepath);

    m_Texture = std::make_unique<Core::Texture>(
        Core::SdlFormatToGlFormat(surface->format->format), surface->w,
        surface->h, surface->pixels);
    m_Size = {surface->w, surface->h};
}

void Image::SetImage(const std::string &filepath) {
    auto surface = s_Store.Get(filepath);

    m_Texture->UpdateData(Core::SdlFormatToGlFormat(surface->format->format),
                          surface->w, surface->h, surface->pixels);
    m_Size = {surface->w, surface->h};
}

void Image::Draw(const Core::Matrices &data) {
    m_UniformBuffer->SetData(0, data);

    m_Texture->Bind(UNIFORM_SURFACE_LOCATION);
    s_Program->Bind();
    s_Program->Validate();

    s_VertexArray->Bind();
    s_VertexArray->DrawTriangles();
}
    void Image::DrawRect(const Core::Matrices &data, float x, float y, float w, float h) {
    m_UniformBuffer->SetData(0, data);
    m_Texture->Bind(UNIFORM_SURFACE_LOCATION);
    s_Program->Bind();

    GLint useRectLoc = glGetUniformLocation(s_Program->GetId(), "u_UseSourceRect");
    GLint rectLoc = glGetUniformLocation(s_Program->GetId(), "u_SourceRect");
    GLint texSizeLoc = glGetUniformLocation(s_Program->GetId(), "u_TexSize");

    // 1. 拿起剪刀：開啟裁切，並塞入你的 x, y, w, h
    glUniform1i(useRectLoc, 1);
    glUniform4f(rectLoc, x, y, w, h);
    glUniform2f(texSizeLoc, m_Size.x, m_Size.y);

    s_Program->Validate();
    s_VertexArray->Bind();
    s_VertexArray->DrawTriangles();

    // ==========================================
    // 💡 2. 自己擦屁股：畫完立刻放下剪刀！
    // 這樣原本的 Draw() 就絕對不會被影響，也不需要去改它了！
    // ==========================================
    glUniform1i(useRectLoc, 0);
}
void Image::InitProgram() {
    // TODO: Create `BaseProgram` from `Program` and pass it into `Drawable`
    s_Program =
        std::make_unique<Core::Program>(PTSD_ASSETS_DIR "/shaders/Base.vert",
                                        PTSD_ASSETS_DIR "/shaders/Base.frag");
    s_Program->Bind();

    GLint location = glGetUniformLocation(s_Program->GetId(), "surface");
    glUniform1i(location, UNIFORM_SURFACE_LOCATION);
}

void Image::InitVertexArray() {
    s_VertexArray = std::make_unique<Core::VertexArray>();

    // NOLINTBEGIN
    // These are vertex data for the rectangle but clang-tidy has magic
    // number warnings

    // Vertex
    s_VertexArray->AddVertexBuffer(std::make_unique<Core::VertexBuffer>(
        std::vector<float>{
            -0.5F, 0.5F,  //
            -0.5F, -0.5F, //
            0.5F, -0.5F,  //
            0.5F, 0.5F,   //
        },
        2));

    // UV
    s_VertexArray->AddVertexBuffer(std::make_unique<Core::VertexBuffer>(
        std::vector<float>{
            0.0F, 0.0F, //
            0.0F, 1.0F, //
            1.0F, 1.0F, //
            1.0F, 0.0F, //
        },
        2));

    // Index
    s_VertexArray->SetIndexBuffer(
        std::make_unique<Core::IndexBuffer>(std::vector<unsigned int>{
            0, 1, 2, //
            0, 2, 3, //
        }));
    // NOLINTEND
}

std::unique_ptr<Core::Program> Image::s_Program = nullptr;
std::unique_ptr<Core::VertexArray> Image::s_VertexArray = nullptr;

Util::AssetStore<std::shared_ptr<SDL_Surface>> Image::s_Store(LoadSurface);
} // namespace Util
