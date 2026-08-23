#pragma once
#include "bindless_texture.hpp"
#include "../math/color.hpp"


namespace NoctisEngine::Rendering
{

struct alignas(16) Material
{
    Color base_color;
    BindlessTexture albedo{};
};

class MaterialKey 
{
public:
    constexpr MaterialKey() 
        : key_{-1} {}

    constexpr explicit MaterialKey(int key)
        : key_(key) {}

    constexpr auto get() const -> int { return key_; }
    constexpr auto is_valid() const -> bool { return key_ >= 0; }

private:
    int key_;
};

} // namespace NoctisEngine::Rendering
