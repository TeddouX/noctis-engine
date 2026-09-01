#pragma once
#include "draw_list.hpp"
#include "gpu_buffer.hpp"


namespace NoctisEngine
{
    
/// @brief Represents an orthographic projection camera, best suited for 2D games
class Camera2D
{
public:
    /// @brief Creates a 2D camera
    /// @param half_extents The half width and half height of the camera frustum 
    /// @param position The camera's position, default (0, 0)
    /// @param near The near clipping plane position, default -1000
    /// @param far The far clipping plane position, default 1000
    Camera2D(
        const glm::vec2    &half_extents, 
        const glm::vec2    &position = glm::vec2{0}, 
        float               near = -1000.f, 
        float               far = 1000.f
    );

    ~Camera2D() = default;

    /// @brief Translates (moves) the camera by a given amount
    /// @param translation The translation vector
    auto translate(const glm::vec2 &translation) -> void;

    /// @brief Sets the camera's position
    /// @param position The new position
    auto set_position(const glm::vec2 &position) -> void;

    /// @brief Updates the camera's projection matrix, useful when the window gets resized
    /// @param new_half_extents The new half extents
    auto update_projection_matrix(const glm::vec2 &new_half_extents) -> void;

    /// @brief Updates the camera's buffers, call this after it was moved
    auto update_buffers() -> void;

    /// @brief Uses this camera for perspective rendering
    /// @param draw_list The draw list that should be used
    auto use(DrawList &draw_list) -> void;

    /// @brief Deletes this camera's buffer
    auto delete_buffer() -> void;

private:
    GPUBuffer camera_ubo_;

    glm::mat4 projection_matrix_;
    glm::vec2 pos_;
};

} // namespace NoctisEngine
