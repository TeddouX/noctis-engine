#pragma once
#include "../../math/math.hpp"
#include "../../noctis_engine.hpp"

namespace NoctisEngine
{
    
class NCENG_API Transform {
public:
    Transform() = default;
    Transform(const glm::vec3 &position, const glm::vec3 &scale, const glm::vec3 &eulerAngles);
    Transform(const glm::vec2 &position, const glm::vec2 &scale, const glm::vec3 &eulerAngles);

    Transform(const glm::vec3 &position, const glm::vec3 &scale, const glm::quat &rotation);
    Transform(const glm::vec2 &position, const glm::vec2 &scale, const glm::quat &rotation);

    auto set_position(const glm::vec3 &pos) -> void;
    auto set_position(const glm::vec2 &pos) -> void;

    auto set_scale(const glm::vec3 &scale) -> void;
    auto set_scale(const glm::vec2 &scale) -> void;

    auto set_rotation(const glm::quat &rot) -> void;
    auto set_euler_angles(const glm::vec3 &eulerAngles) -> void;

    auto translate(const glm::vec3 &translation) -> void;
    auto translate(const glm::vec2 &translation) -> void;

    auto get_position() const -> const glm::vec3 &;
    auto get_position_2D() const -> glm::vec2;
    auto get_scale() const -> const glm::vec3 &;
    auto get_rotation() const -> const glm::quat &;
    auto get_euler_angles() const -> glm::vec3;

    auto distance_to(const Transform &other) -> float;

    auto model_matrix() const -> const glm::mat4 &;

private:
    glm::vec3 pos_;
    glm::vec3 scale_;
    glm::quat rotation_;

    mutable bool        dirty_;
    mutable glm::mat4x4 cachedModelMatrix_;
};

} // namespace NoctisEngine

