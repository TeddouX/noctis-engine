#pragma once
#include <string_view>

#include "draw_list.hpp"
#include "texture.hpp"


namespace NoctisEngine
{
    
/// @brief Represents an OpenGL framebuffer object (FBO)
class FrameBuffer
{
public:
    static constexpr std::string_view COLOR_TEX_SUFFIX = "_color_tex";

    /// @brief Default constructor, doesn't create anything on the GPU
    FrameBuffer() = default;

    /// @brief Creates the framebuffer and its color texture (optionally) and render buffer (used for depth testing) on the GPU
    /// @param name The framebuffer's name (also used for its textures)
    /// @param width The framebuffer's width, should be the frambuffer width of your window
    /// @param height The framebuffer's height, should be the frambuffer height of your window
    FrameBuffer(std::string_view name, std::uint32_t width, std::uint32_t height);

    ~FrameBuffer() = default;

    /// @brief Resizes this framebuffer
    /// @param new_width The new width
    /// @param new_height The new height
    /// @return 
    auto resize(std::uint32_t new_width, std::uint32_t new_height) -> void;

    /// @brief Attaches a color texture to this framebuffer  
    /// @param tex The color texture
    /// @warning This deletes the old one from the GPU
    auto attach_color_tex(Texture &tex) -> void;

    /// @brief Gets this frambuffer's handle
    auto gl_handle() const -> std::uint32_t;

    /// @brief Gets this frambuffer's color texture
    auto color_tex() -> Texture &;

    /// @brief Gets this frambuffer's color texture
    auto color_tex() const -> const Texture &;

    /// @brief Gets this framebuffer's name
    auto name() const -> std::string_view;

private:
    std::uint32_t       handle_;
    std::uint32_t       rbo_;
    Texture             color_tex_;

    std::string_view    name_;
};

} // namespace NoctisEngine
