#include "Util/GameObject.hpp"
#include "Util/Transform.hpp"
#include "Util/TransformUtils.hpp"
#include "Util/Image.hpp"
namespace Util {

void GameObject::Draw() {
    if (!m_Visible || m_Drawable == nullptr) {
        return;
    }

    auto data = Util::ConvertToUniformBufferData(
        m_Transform, m_Drawable->GetSize(), m_ZIndex);
    data.m_Model = glm::translate(
        data.m_Model, glm::vec3{m_Pivot / m_Drawable->GetSize(), 0} * -1.0F);

    m_Drawable->Draw(data);
}

    void GameObject::DrawRect(float x, float y, float w, float h) {
    if (!m_Visible || m_Drawable == nullptr) {
        return;
    }

    // GameObject 自己算出 data 矩陣
    auto data = Util::ConvertToUniformBufferData(
        m_Transform, m_Drawable->GetSize(), m_ZIndex);
    data.m_Model = glm::translate(
        data.m_Model, glm::vec3{m_Pivot / m_Drawable->GetSize(), 0} * -1.0F);

    // 轉型成 Image 並呼叫我們寫的新方法
    if (auto img = std::dynamic_pointer_cast<Util::Image>(m_Drawable)) {
        img->DrawRect(data, x, y, w, h);
    } else {
        m_Drawable->Draw(data);
    }
}

} // namespace Util
