#include <rendering/bindless_texture.hpp>

#include <GL/gl.h>


namespace NoctisEngine::Rendering
{
    
BindlessTexture::BindlessTexture(Texture tex) 
{
    handle_ = glGetTextureHandleARB(tex.gl_handle());

    if (handle_ == 0)
    {
        RENDERING_LOGGER.error("Failed to get a handle to a texture.");
        return;   
    }

    make_resident(true);
}

auto BindlessTexture::make_resident(bool b) const -> void 
{
    if (b) glMakeTextureHandleResidentARB(handle_);
    else   glMakeTextureHandleNonResidentARB(handle_);
}

auto BindlessTexture::get_handle() const -> std::uint64_t 
{
    return handle_;
}

} // namespace NoctisEngine::Rendering