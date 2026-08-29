#pragma once
#include <stdint.h>
#include <string>
#include <memory>

#include "../noctis_engine.hpp"
#include "../math/color.hpp"
#include "shader.hpp"
#include "draw_list.hpp"


namespace NoctisEngine::Rendering
{

/// @brief The function used when shrinking the texture
enum class MinifyingFunction : std::uint32_t
{
    /// @brief Picks the closest pixel, best for pixel art rendering
    NEAREST = 0x2600,

    /// @brief Blends the 4 nearest pixels, smooth but not mipmaps
    LINEAR = 0x2601,

    /// @brief Picks the closest mip level, then chooses the nearest pixel
    NEAREST_MIPMAP_NEAREST = 0x2700,

    /// @brief Blends between the two closest mip levels, then chooses the nearest pixel
    LINEAR_MIPMAP_NEAREST = 0x2701,
    
    /// @brief Picks the closest mip level, then does linear filtering
    NEAREST_MIPMAP_LINEAR = 0x2702,

    /// @brief Blends between the two closest mip levels, then does linear filtering
    LINEAR_MIPMAP_LINEAR = 0x2703,
};

/// @brief The function used when magnifying the texture
enum class MagnifyingFunction : std::uint32_t 
{
    /// @brief Picks the closest pixel, best for pixel art rendering
    NEAREST = 0x2600,

    /// @brief Blends the 4 nearest pixels, smoother but low quality-esque
    LINEAR = 0x2601,
};

/// @brief This constrols what happens when texture coordinates go outside the [0, 1] range 
enum class WrapParam : std::uint32_t 
{
    /// @brief Just repeat the edge pixel forever, stretching the texture
    CLAMP_TO_EDGE = 0x812F,
    
    /// @brief Uses a defined border color instead of any pixel
    CLAMP_TO_BORDER = 0x812D,

    /// @brief The texture repeats but flips each time it tiles
    MIRRORED_REPEAT = 0x8370,

    /// @brief the texture repeats over and over
    REPEAT = 0x2901,

    /// @brief Mirrors the texture once, then clamps to the edge beyond that
    MIRROR_CLAMP_TO_EDGE = 0x8743,
};

/// @brief This class represents a texture stored on the GPU
class Texture
{
public:
    /// @brief Describes a texture's data, do not use this directly, 
    /// except if you know what you're doing, instead use Asset::load_texture
    struct Data 
    {
        /// @brief The texture's pixel data
        uint8_t *data;

        /// @brief The texture's width
        int width; 

        /// @brief The texture's height
        int height;
        /// @brief The texture's number of color channels
        int nr_channels;

        /// @brief The texture's name
        std::string name;
    };

    /// @brief Creates a texture and uploads its data to the GPU
    /// @param texture_data The texture's data
    Texture(const Data &texture_data);

    ~Texture() = default;

    /// @brief Sets the texture's minifying function
    /// @param func The minifying function 
    auto set_minifying_function(MinifyingFunction func) const -> void;

    /// @brief Sets the texture's magnifying function
    /// @param func The magnifying function 
    auto set_magnifying_function(MagnifyingFunction func) const -> void;

    /// @brief Sets the texture's wrap parameters
    /// @param u Horizontal wrap parameter
    /// @param v Vertical wrap parameter
    auto set_wrap_params(WrapParam u, WrapParam v) const -> void;
    
    /// @brief Sets the texture's border color
    /// @important Only useful if you use the CLAMP_TO_BORDER wrap parameter
    /// @param c The texture's border color
    auto set_border_color(const Color &c) const -> void;

    /// @brief Sets this texture to be bound in a draw list
    /// @param draw_list The draw list
    /// @param bind_point The point the texture should be bound to in the shader
    auto bind(DrawList &draw_list, std::uint32_t bind_point) const -> void;

    /// @brief Removes this texture from the GPU, it shouldn't be used after deletion
    auto delete_gpu() const -> void;

    /// @return This texture's OpenGL handle 
    auto gl_handle() const -> std::uint32_t;

    /// @return The texture's width 
    auto width() const -> int;

    /// @return The texture's height 
    auto height() const -> int;

    /// @return The texture's name 
    auto name() const -> std::string;

private:
    std::uint32_t   handle_;
    std::string     name_;

    int             width_;
    int             height_;
};

/// @brief Texture uniform indices and names reserved by the 
/// engine to pass textures to shaders
namespace TextureSlots
{

/// @brief The albedo uniform index
constexpr std::uint32_t ALBEDO = 0;

/// @brief The albedo uniform name
constexpr std::string_view ALBEDO_NAME = "albedo";

} // namespace TextureSlots


} // namespace NoctisEngine::Rendering