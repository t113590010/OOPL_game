#ifndef BASE_HPP
#define BASE_HPP
#include "Entity.hpp"

class Base : public Entity {
public:
    Base(const Vector2& pos, int hp);
    void Update(float dt) override;
    void SetImage(const std::string& imgPath);
};
#endif