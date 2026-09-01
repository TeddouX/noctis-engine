#pragma once
#include "../math/color.hpp"
#include "gpu_buffer.hpp"
#include "mesh/mesh_manager.hpp"


namespace NoctisEngine
{
    
/// @brief Blending functions
enum class BlendFunc 
{
    /// @brief Multiplies the color component by 0
    ZERO = 0, 

    /// @brief Multiplies the color component by 1, contributing it fully unchanged
    ONE = 1,

    /// @brief Multiplies by the source color's RGB values
    SRC_COLOR = 0x0300, 

    /// @brief Multiplies by (1 - source color), i.e. the inverse of the source RGB
    ONE_MINUS_SRC_COLOR = 0x0301, 

    /// @brief Multiplies by the source fragment's alpha value
    SRC_ALPHA = 0x0302, 

    /// @brief Multiplies by (1 - source alpha), i.e. how transparent the source isn't
    ONE_MINUS_SRC_ALPHA = 0x0303, 

    /// @brief Multiplies by the destination (existing framebuffer) fragment's alpha value.
    DST_ALPHA = 0x0304, 

    /// @brief Multiplies by (1 - destination alpha).
    ONE_MINUS_DST_ALPHA = 0x0305, 

    /// @brief Multiplies by the destination (existing framebuffer) color's RGB values.
    DST_COLOR = 0x0306, 

    /// @brief Multiplies by (1 - destination color).
    ONE_MINUS_DST_COLOR = 0x0307,

    /// @brief Multiplies by a fixed, user-set RGB color, set with Renderer::blend_color
    CONSTANT_COLOR = 0x8001,

    /// @brief Multiplies by (1 - the user-set constant color, set with Renderer::blend_color).
    ONE_MINUS_CONSTANT_COLOR = 0x8002,

    /// @brief Multiplies by a fixed, user-set alpha value, set with Renderer::blend_color
    CONSTANT_ALPHA = 0x8003,

    /// @brief Multiplies by (1 - the user-set constant alpha, set with Renderer::blend_color).
    ONE_MINUS_CONSTANT_ALPHA = 0x8004
};

/// @brief Use this class to render draw lists and to parameterize your rendering
class Renderer
{
public:
    /// @brief Default constructor that initialises required buffers
    Renderer();

    /// @brief Backface culling skips rendering triangles whose face is pointing away from the camera
    /// (based on vertex winding order)
    /// @param b true if it should be enabled, false otherwise
    auto set_backface_culling(bool b) const -> void;

    /// @brief Depth testing compares each new pixels's depth (distance from the camera) against the value already 
    /// stored in the depth buffer at that pixel, and discards the pixel if it's farther away, so closer objects 
    /// correctly draw over farther ones regardless of render order.
    /// @param b true if it should be enabled, false otherwise
    auto set_depth_testing(bool b) const -> void;

    /// @brief Enables the application to crash if an OpenGL error is encountered 
    /// @param b true if it should be enabled, false otherwise
    auto set_throw_on_err(bool b) -> void;
    
    /// @brief Enables blending
    /// @param b true if it should be enabled, false otherwise
    auto set_blend(bool b) const -> void;

    /// @brief Sets the blending color, used by _CONSTANT_COLOR_ blending functions
    /// @param col The blending color
    auto set_blend_color(const Color &col);

    /// @brief Sets the two multiplier factors: one for the incoming (source) pixel and one for the 
    /// already drawn (destination) framebuffer pixel that get combined
    /// to produce the final blended color when blending is enabled
    /// @param source The source pixel
    /// @param dest The destination framebuffer pixel
    auto set_blend_func(BlendFunc source, BlendFunc dest) const -> void;

    /// @brief Renders the contents of a draw list on screen
    /// @param draw_list The draw list that should be rendered
    auto render(DrawList &draw_list) -> void;

private:
    GPUBuffer                       objects_ssbo_;
    GPUBuffer                       command_buffer_;

    bool                            throw_on_err_;

    auto check_ogl_extensions() -> void;

    static void opengl_debug_message_callback(uint32_t source, uint32_t type, uint32_t id, uint32_t severity,
        int length, const char* message, const void* userParam);
};

} // namespace NoctisEngine
