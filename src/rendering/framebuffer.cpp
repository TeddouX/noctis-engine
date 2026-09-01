#include <noctis_engine/rendering/framebuffer.hpp>

#include <GL/gl.h>


namespace NoctisEngine
{
    
FrameBuffer::FrameBuffer(std::string_view name, std::uint32_t width, std::uint32_t height)
    : name_{name}
{
    glCreateFramebuffers(1, &handle_);

    color_tex_ = Texture{
        Texture::Data{
            .data = nullptr,
            .width = static_cast<int>(width),
            .height = static_cast<int>(height),
            .nr_channels = 4,
            .name = std::string(name) + COLOR_TEX_SUFFIX,
        }
    };
    color_tex_.set_minifying_function(MinifyingFunction::LINEAR);
    color_tex_.set_magnifying_function(MagnifyingFunction::LINEAR);

    glNamedFramebufferTexture(handle_, GL_COLOR_ATTACHMENT0, color_tex_.gl_handle(), 0);

    glCreateRenderbuffers(1, &rbo_);
    glNamedRenderbufferStorage(rbo_, GL_DEPTH24_STENCIL8, width, height);
    glNamedFramebufferRenderbuffer(handle_, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo_);
}

auto FrameBuffer::resize(std::uint32_t new_width, std::uint32_t new_height) -> void
{
    glNamedRenderbufferStorage(rbo_, GL_DEPTH24_STENCIL8, new_width, new_height);

    color_tex_.set_data(
        Texture::Data{
            .data = nullptr,
            .width = static_cast<int>(new_width),
            .height = static_cast<int>(new_height),
            .nr_channels = 4,
            .name = color_tex_.name(),
        }
    );
}

auto FrameBuffer::attach_color_tex(Texture &tex) -> void
{
    color_tex_.delete_gpu();
    color_tex_ = tex;
}

auto FrameBuffer::gl_handle() const -> std::uint32_t
{
    return handle_;
}

auto FrameBuffer::color_tex() -> Texture &
{
    return color_tex_;
}

auto FrameBuffer::color_tex() const -> const Texture &
{
    return color_tex_;
}

auto FrameBuffer::name() const -> std::string_view
{
    return name_;
}

} // namespace NoctisEngine
