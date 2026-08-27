#pragma once
#include "../../math/math.hpp"


namespace NoctisEngine::ECS
{
    
class Transform2D
{
public:
    Transform2D(glm::vec2 pos = glm::vec2{0}, float rotation_rad = 0.f, glm::vec2 scale = glm::vec2{1});

    auto set_position(const glm::vec2 &pos) -> void;

    auto set_scale(const glm::vec2 &scale) -> void;

    auto set_rotation(float rot) -> void;
    auto set_rotation_deg(float rot) -> void;

    auto translate(const glm::vec2 &translation) -> void;
    auto rotate(float rot) -> void;
    auto rotate_deg(float rot) -> void;

    auto position() const -> const glm::vec2 &;
    auto scale() const -> const glm::vec2 &;
    auto rotation() const -> float;
    auto rotation_deg() const -> float;

    auto distance_to(const Transform2D &other) -> float;
    auto distance2_to(const Transform2D &other) -> float;

    auto model_matrix() const -> const glm::mat4 &;

private:
    glm::vec2           pos_;
    float               rot_rad_;
    glm::vec2           scale_;

    mutable bool        dirty_;
    mutable glm::mat4   model_matrix_;
};

} // namespace NoctisEngine::ECS
