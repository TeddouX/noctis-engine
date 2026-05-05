#pragma once
#include <cstdint>
#include "../../math/color.hpp"
#include "../bindless_texture.hpp"

namespace NoctisEngine
{
    
struct alignas(16) MaterialData {
    glm::vec3 baseColor{1};
    float alpha = 1.f;

    BindlessTexture albedo{BindlessTexture::default_textures().white_1x1};
};

class MaterialKey {
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

} // namespace NoctisEngine
