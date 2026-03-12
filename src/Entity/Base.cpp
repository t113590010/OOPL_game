#include "Entity/Base.hpp"

Base::Base(const Vector2& pos, int hp)
    : Entity(pos, hp, 0.0f, 0, 0.0f, 0.0f, 0, 0.0f) {
    m_IsMoving = false;
    m_Size = {GameConfig::BASE_SIZE_X,GameConfig::BASE_SIZE_Y};
    m_Renderer.SetZIndex(-1);
}

void Base::Update(float /*dt*/) {
    // 基地不需要移動
}

void Base::SetImage(const std::string& imgPath) {
    m_Image = std::make_shared<Util::Image>(imgPath);
}