#include <noctis_engine/ecs/component/transform_2d.hpp>

#include <noctis_engine/definitions.hpp>


namespace NoctisEngine
{
    
Transform2D::Transform2D(glm::vec2 pos, float rotation_rad, glm::vec2 scale)
    : pos_{pos}
    , rot_rad_{rotation_rad}
    , scale_{scale}
    , dirty_{true}
    , model_matrix_{1}
{
    // Cache the model matrix
    model_matrix();
}

auto Transform2D::set_position(const glm::vec2 &pos) -> void
{
    if (pos != pos_)
        dirty_ = true;
    pos_ = pos;
}

auto Transform2D::set_scale(const glm::vec2 &scale) -> void
{
    if (scale != scale_)
        dirty_ = true;
    scale_ = scale;
}

auto Transform2D::set_rotation(float rot) -> void
{
    if (rot != rot_rad_)
        dirty_ = true;
    rot_rad_ = rot;
}

auto Transform2D::set_rotation_deg(float rot) -> void
{
    set_rotation(glm::radians(rot));
}

auto Transform2D::translate(const glm::vec2 &translation) -> void
{
    set_position(pos_ + translation);
}

auto Transform2D::rotate(float rot) -> void
{
    set_rotation(rot_rad_ + rot);
}

auto Transform2D::rotate_deg(float rot) -> void
{
    set_rotation(rot_rad_ + glm::radians(rot));
}

auto Transform2D::position() const -> const glm::vec2 &
{
    return pos_;
}

auto Transform2D::scale() const -> const glm::vec2 &
{
    return scale_;
}

auto Transform2D::rotation() const -> float
{
    return rot_rad_;
}

auto Transform2D::rotation_deg() const -> float
{
    return glm::degrees(rot_rad_);
}

auto Transform2D::distance_to(const Transform2D &other) -> float
{
    return glm::distance(pos_, other.pos_);
}

auto Transform2D::distance2_to(const Transform2D &other) -> float
{
    return glm::distance2(pos_, other.pos_);
}

auto Transform2D::model_matrix() const -> const glm::mat4 &
{
    if (dirty_) 
    {
        const float c = glm::cos(rot_rad_);
        const float s = glm::sin(rot_rad_);

        const float scaleX = scale_.x;
        const float scaleY = scale_.y;

        model_matrix_ = glm::mat4{
            {  c * scaleX,  s * scaleX,  0.0f, 0.0f },
            { -s * scaleY,  c * scaleY,  0.0f, 0.0f },
            {  0.0f,        0.0f,        1.0f, 0.0f },
            { pos_.x,       pos_.y,      0.0f, 1.0f }
        };

        dirty_ = false;
    }

    return model_matrix_;
}


} // namespace NoctisEngine
