#include <noctis_engine/asset/loaders.hpp>

#include <stb/stb_image.h>

#include <noctis_engine/core/logging.hpp>


namespace NoctisEngine::Asset
{
    
Core::Logger ASSET_LOGGER{"Noctis Engine", "Asset"};

auto load_texture(const std::filesystem::path &path, const std::string &name) -> std::optional<Rendering::Texture>
{
    int width, height;
    int nr_channels;


    if (!std::filesystem::exists(path)) 
    {
        ASSET_LOGGER.error("{} does not exist.", path.string());
        return std::nullopt;
    }

    uint8_t *data = stbi_load(
        path.string().c_str(), 
        &width, &height, 
        &nr_channels, 0
    );

    if (!data) 
    {
        ASSET_LOGGER.error("Failed to load image {}: {}", path.string(), stbi_failure_reason());
        return std::nullopt;
    }

    std::string tex_name = name;
    if (tex_name.empty())
        tex_name = path.stem();

    Rendering::TextureInfo texInfo 
    {
        .data = data,
        .width = width, .height = height,
        .nr_channels = nr_channels,
        .name = tex_name
    };

    Rendering::Texture tex{texInfo};

    stbi_image_free(data);

    return tex;
}

} // namespace NoctisEngine::Asset
