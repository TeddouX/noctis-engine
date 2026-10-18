#include <noctis_engine/ecs/component/transform_2d.hpp>

#include <noctis_engine/math/angle.hpp>
#include <noctis_engine/math/matrix_math.hpp>
#include <noctis_engine/definitions.hpp>


namespace NoctisEngine
{
    
Transform2D::Transform2D(Vec2 pos, float rotation_rad, Vec2 scale)
    : pos_{pos}
    , rot_rad_{rotation_rad}
    , scale_{scale}
    , dirty_{true}
    , model_matrix_{Mat4x4::IDENTITY}
{
    // Cache the model matrix
    model_matrix();
}

auto Transform2D::set_position(const Vec2 &pos) -> void
{
    if (pos != pos_)
        dirty_ = true;
    pos_ = pos;
}

auto Transform2D::set_scale(const Vec2 &scale) -> void
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
    set_rotation(radians(rot));
}

auto Transform2D::translate(const Vec2 &translation) -> void
{
    set_position(pos_ + translation);
}

auto Transform2D::rotate(float rot) -> void
{
    set_rotation(rot_rad_ + rot);
}

auto Transform2D::rotate_deg(float rot) -> void
{
    set_rotation(rot_rad_ + radians(rot));
}

auto Transform2D::position() const -> const Vec2 &
{
    return pos_;
}

auto Transform2D::scale() const -> const Vec2 &
{
    return scale_;
}

auto Transform2D::rotation() const -> float
{
    return rot_rad_;
}

auto Transform2D::rotation_deg() const -> float
{
    return degrees(rot_rad_);
}

auto Transform2D::distance_to(const Transform2D &other) -> float
{
    return distance(pos_, other.pos_);
}

auto Transform2D::distance2_to(const Transform2D &other) -> float
{
    return distance2(pos_, other.pos_);
}

auto Transform2D::model_matrix() const -> const Mat4x4 &
{
    if (dirty_) 
    {
        model_matrix_ = model_matrix_2d(pos_, scale_, rot_rad_);
        dirty_ = false;
    }

    return model_matrix_;
}


} // namespace NoctisEngine
