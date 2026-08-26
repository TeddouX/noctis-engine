#pragma once
#include <stdint.h>
#include <string>
#include <memory>

#include "../noctis_engine.hpp"
#include "../math/color.hpp"
#include "shader.hpp"


namespace NoctisEngine::Rendering
{

struct TextureInfo 
{
    uint8_t *data;
    int width, height;
    int nr_channels;
    std::string name;
};

class Texture 
{
public:
    enum class MinifyingFunction : uint32_t 
    {
        NEAREST = 0x2600,
        LINEAR = 0x2601,
        NEAREST_MIPMAP_NEAREST = 0x2700,
        LINEAR_MIPMAP_NEAREST = 0x2701,
        NEAREST_MIPMAP_LINEAR = 0x2702,
        LINEAR_MIPMAP_LINEAR = 0x2703,
    };

    enum class MagnifyingFunction : uint32_t 
    {
        NEAREST = 0x2600,
        LINEAR = 0x2601,
    };

    enum class WrapParam : uint32_t 
    {
        CLAMP_TO_EDGE = 0x812F,
        CLAMP_TO_BORDER = 0x812D,
        MIRRORED_REPEAT = 0x8370,
        REPEAT = 0x2901,
        MIRROR_CLAMP_TO_EDGE = 0x8743,
    };

    Texture(TextureInfo tex_info);

    auto use(DrawList &draw_list, std::uint32_t bind_point) const -> void;

    auto set_min_function(MinifyingFunction param) const -> void;
    auto set_mag_function(MagnifyingFunction param) const -> void;
    auto set_wrap_function(WrapParam paramU, WrapParam paramV) const -> void;

    auto width() const -> int;
    auto height() const -> int;

    auto gl_handle() const -> std::uint32_t;

private:
    std::uint32_t   texID_;
    std::string     name_;

    int             width_, 
                    height_;
};

namespace TextureSlots
{
    
constexpr std::uint32_t ALBEDO = 0;

} // namespace TextureSlots


} // namespace NoctisEngine::Rendering