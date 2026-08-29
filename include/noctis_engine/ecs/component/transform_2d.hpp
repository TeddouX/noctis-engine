#pragma once
#include "../../math/math.hpp"


namespace NoctisEngine::ECS
{
    
/// @brief This component is used to place an entity in the world
class Transform2D
{
public:
    /// @brief Creates a new 2D transform
    /// @param pos The position
    /// @param rotation_rad The rotation in radians. Use glm::rad to convert from degrees.
    /// @param scale The scale. RENDERING ONLY, SCALING ISN'T HANDLED BY THE PHYSICS SYSTEM.
    Transform2D(glm::vec2 pos = glm::vec2{0}, float rotation_rad = 0.f, glm::vec2 scale = glm::vec2{1});

    /// @brief Sets this transform's position
    /// @param pos The new position
    auto set_position(const glm::vec2 &pos) -> void;

    /// @brief Sets this transform's scale
    /// @param pos The new scale
    /// @warning RENDERING ONLY, SCALING ISN'T HANDLED BY THE PHYSICS SYSTEM.
    auto set_scale(const glm::vec2 &scale) -> void;

    /// @brief Sets this transform's rotation
    /// @param pos The new rotation in radians
    auto set_rotation(float rot) -> void;

    /// @brief Sets this transform's rotation
    /// @param pos The new rotation in degrees
    auto set_rotation_deg(float rot) -> void;

    /// @brief Translates this transform's position
    /// @param translation The translation vector
    auto translate(const glm::vec2 &translation) -> void;
    
    /// @brief Rotates this transform
    /// @param rot Rotation in radians
    auto rotate(float rot) -> void;
    
    /// @brief Rotates this transform
    /// @param rot Rotation in degrees
    auto rotate_deg(float rot) -> void;

    /// @return This transform's position 
    auto position() const -> const glm::vec2 &;

    /// @return This transform's scale 
    auto scale() const -> const glm::vec2 &;
    
    /// @return This transform's rotation in radians 
    auto rotation() const -> float;

    /// @return This transform's rotation in degrees 
    auto rotation_deg() const -> float;

    /// @brief Calculates the distance between two transforms using Pythagoras's theorem.
    /// @param other The other transform
    /// @return The distance between the two transforms
    auto distance_to(const Transform2D &other) -> float;

    /// @brief Calculates the squared distance between two transforms using Pythagoras's theorem.
    /// Less expensive than distance_to(), use this if comparing distances.
    /// @param other The other transform
    /// @return The squared distance between the two transforms
    auto distance2_to(const Transform2D &other) -> float;

    /// @return This transform's model matrix, used for rendering 
    auto model_matrix() const -> const glm::mat4 &;

private:
    glm::vec2           pos_;
    float               rot_rad_;
    glm::vec2           scale_;

    mutable bool        dirty_;
    mutable glm::mat4   model_matrix_;
};

} // namespace NoctisEngine::ECS
