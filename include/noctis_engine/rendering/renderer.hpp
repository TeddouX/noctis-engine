#pragma once
#include "gpu_buffer.hpp"
#include "framebuffer.hpp"
#include "graphics_program.hpp"
#include "mesh/mesh_manager.hpp"
#include "../math/color.hpp"


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

struct RenderPass
{
    std::string_view            name;

    /// @brief Leave it null if this render pass should draw to the screen's framebuffer
    std::optional<FrameBuffer>  frame_buffer;
};

/// @brief Use this class to render draw lists and to parameterize your rendering
class Renderer
{
public:
    /// @brief Default constructor that initialises required buffers
    /// @param framebuffer_size Your window's framebuffer size (use Window::framebuffer_size)
    Renderer(const glm::ivec2 &framebuffer_size);

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

    /// @brief This function resizes this renderer's frambuffer, call this in your window's resize callback
    /// @param new_fb_size The frame buffer's new size
    auto resize_framebuffer(int new_width, int new_height) -> void;

    /// @brief This sets the graphics program that merges all the render passes. 
    /// Use this if you have custom render passes
    /// @warning This deletes the old composition program
    auto set_composition_program(GraphicsProgram program) -> void;

    /// @brief Renders a draw list in a render pass
    /// @param draw_list The draw list that should be rendered
    /// @param render_pass The render pass that should be used
    auto render_pass(DrawList &draw_list, const RenderPass &render_pass) -> void;

    /// @brief Renders the contents of a draw list on screen, using the default world render pass
    /// @param draw_list The draw list that should be rendered
    auto render_world(DrawList &draw_list) -> void;

    /// @brief Renders the contents of a draw list on screen, using the default UI render pass
    /// @param draw_list The draw list that should be rendered
    auto render_ui(DrawList &draw_list) -> void;

    /// @brief Puts on the user's screen every render pass you've drawn
    auto show_final_image() -> void;

private:
    GPUBuffer                       objects_ssbo_;
    GPUBuffer                       command_buffer_;

    RenderPass                      world_render_pass_;
    RenderPass                      ui_render_pass_;
    RenderPass                      composition_render_pass_;
    GraphicsProgram                 composition_program_;
    MeshManager                     quad_mesh_manager_;

    std::vector<const Texture *>    fb_textures_;

    MeshView                        default_quad_mv_;

    bool                            throw_on_err_;

    static auto opengl_debug_message_callback(
        std::uint32_t source, 
        std::uint32_t type, 
        std::uint32_t id, 
        std::uint32_t severity,
        int length, 
        const char *message, 
        const void *userParam
    ) -> void;
};

} // namespace NoctisEngine
