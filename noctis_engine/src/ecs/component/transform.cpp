#include <ecs/component/transform.hpp>

#include <core/logging.hpp>

namespace NoctisEngine
{
    
Transform::Transform(const glm::vec3 &position, const glm::vec3 &scale, const glm::vec3 &eulerAngles) 
    : cachedModelMatrix_{1}
    , dirty_{false}
{
    set_position(position);
    set_scale(scale);
    set_euler_angles(eulerAngles);
}

Transform::Transform(const glm::vec3 &position, const glm::vec3 &scale, const glm::quat &rotation) 
    : cachedModelMatrix_{1}
    , dirty_{false}
{
    set_position(position);
    set_scale(scale);
    set_rotation(rotation);
}

Transform::Transform(const glm::vec2 &position, const glm::vec2 &scale, const glm::vec3 &eulerAngles) 
    : cachedModelMatrix_{1}
    , dirty_{false}
{
    set_position(position);
    set_scale(scale);
    set_euler_angles(eulerAngles);
}

Transform::Transform(const glm::vec2 &position, const glm::vec2 &scale, const glm::quat &rotation) 
    : cachedModelMatrix_{1}
    , dirty_{false}
{
    set_position(position);
    set_scale(scale);
    set_rotation(rotation);
}

auto Transform::set_position(const glm::vec3 &pos) -> void {
    if (pos_ != pos)
        dirty_ = true;
    pos_ = pos;
}

auto Transform::set_scale(const glm::vec3 &scale) -> void {
    if (scale_ != scale)
        dirty_ = true;
    scale_ = scale;
}

auto Transform::set_position(const glm::vec2 &pos) -> void {
    set_position(glm::vec3(pos, 0));
}

auto Transform::set_scale(const glm::vec2 &scale) -> void {
    set_scale(glm::vec3(scale, 1));
}

auto Transform::set_rotation(const glm::quat &rot) -> void {
    if (rotation_ != rot)
        dirty_ = true;
    rotation_ = rot;
}

auto Transform::set_euler_angles(const glm::vec3 &rot) -> void {
    set_rotation(glm::quat{rot});
}

auto Transform::translate(const glm::vec3 &translation) -> void {
    set_position(pos_ + translation);
}

auto Transform::translate(const glm::vec2 &translation) -> void {
    set_position(pos_ + glm::vec3(translation, 0));
}

auto Transform::get_position() const -> const glm::vec3 & {
    return pos_;
}

auto Transform::get_position_2D() const -> glm::vec2 {
    return glm::vec2(pos_.x, pos_.y);
}

auto Transform::get_scale() const -> const glm::vec3 & {
    return scale_;
}

auto Transform::get_rotation() const -> const glm::quat & {
    return rotation_;
}

auto Transform::get_euler_angles() const -> glm::vec3 {
    return glm::eulerAngles(rotation_);
}

auto Transform::distance_to(const Transform &other) -> float {
    return glm::distance(pos_, other.get_position());
}


auto Transform::model_matrix() const -> const glm::mat4 & {
    if (dirty_) {
        // cachedModelMatrix_ = glm::translate(glm::mat4{1}, pos_)
        //     * glm::toMat4(rotation_)
        //     * glm::scale(glm::mat4{1}, scale_);

        const float x = rotation_.x;
        const float y = rotation_.y;
        const float z = rotation_.z;
        const float w = rotation_.w;

        const float x2 = x + x;
        const float y2 = y + y;
        const float z2 = z + z;

        const float xx = x * x2;
        const float yy = y * y2;
        const float zz = z * z2;
        const float xy = x * y2;
        const float xz = x * z2;
        const float yz = y * z2;
        const float wx = w * x2;
        const float wy = w * y2;
        const float wz = w * z2;

        const float scaleX = scale_.x;
        const float scaleY = scale_.y;
        const float scaleZ = scale_.z;

        cachedModelMatrix_ = glm::mat4{
            { (1.0f - (yy + zz)) * scaleX,   (xy + wz) * scaleX,             (xz - wy) * scaleX,             0.0f },
            { (xy - wz) * scaleY,            (1.0f - (xx + zz)) * scaleY,    (yz + wx) * scaleY,             0.0f },
            { (xz + wy) * scaleZ,            (yz - wx) * scaleZ,             (1.0f - (xx + yy)) * scaleZ,    0.0f },
            { pos_.x,                        pos_.y,                         pos_.z,                         1.0f }
        };

        dirty_ = false;
    }

    return cachedModelMatrix_;
}
    
} // namespace NoctisEngine
