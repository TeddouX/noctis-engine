#pragma once
#include <vector>

#include "../math/mat4x4.hpp"
#include "gpu_buffer.hpp"
#include "draw_list.hpp"


namespace NoctisEngine 
{

/// @brief Helper for handling model matrices
class ModelManager
{
public:
    ModelManager();

    /// @brief Adds a model matrix to the buffer used for rendering
    auto add_model_matrix(const Mat4x4 &matrix) -> void;

    /// @brief Adds a sprite's model matrix to the model buffer
    auto add_sprite_model(Entity entity, const ECSWorld &world) -> void;

    /// @brief Binds this model manager to a draw list
    auto use(DrawList &draw_list) -> void;

    /// @brief Sends all matrices to the GPU
    auto flush() -> void;

private:
    std::vector<Mat4x4>     matrices_;
    GPUBuffer               objects_ssbo_;
};

}