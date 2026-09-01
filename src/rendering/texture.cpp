#include <noctis_engine/rendering/texture.hpp>

#include <GL/gl.h>


namespace NoctisEngine
{
    
Texture::Texture(const Data &texture_data)
    : width_{texture_data.width}
    , height_{texture_data.height}
    , name_{texture_data.name}
{
    glGenTextures(1, &handle_);
    glBindTexture(GL_TEXTURE_2D, handle_);
    glObjectLabel(GL_TEXTURE, handle_, -1, name_.c_str());
    set_data(texture_data);
}

auto Texture::set_minifying_function(MinifyingFunction func) const -> void
{
    glBindTexture(GL_TEXTURE_2D, handle_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLenum>(func));
    glBindTexture(GL_TEXTURE_2D, 0);

}

auto Texture::set_magnifying_function(MagnifyingFunction func) const -> void
{
    glBindTexture(GL_TEXTURE_2D, handle_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLenum>(func));
    glBindTexture(GL_TEXTURE_2D, 0);
}

auto Texture::set_wrap_params(WrapParam u, WrapParam v) const -> void
{
    glBindTexture(GL_TEXTURE_2D, handle_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLenum>(u));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLenum>(v));
    glBindTexture(GL_TEXTURE_2D, 0);
}

auto Texture::set_border_color(const Color &c) const -> void
{
    glBindTexture(GL_TEXTURE_2D, handle_);
    glTexParameterfv(GL_TEXTURE_BORDER_COLOR, GL_TEXTURE_WRAP_S, &c.to_floats()[0]);
    glBindTexture(GL_TEXTURE_2D, 0);
}

auto Texture::bind(DrawList &draw_list, std::uint32_t bind_point, std::string_view uniform_name) const -> void
{
    draw_list.bind_texture(handle_, bind_point, uniform_name);
}

auto Texture::set_data(const Data &data) -> void
{
    glBindTexture(GL_TEXTURE_2D, handle_);

    width_ = data.width;
    height_ = data.height;

    GLenum internal_format = GL_RGBA8;
    GLenum data_format = GL_RGBA;

    if (data.nr_channels == 1)
    {
        internal_format = GL_R8;
        data_format = GL_RED;
    }
    else if (data.nr_channels == 3)
    {
        internal_format = GL_RGB8;
        data_format = GL_RGB;
    }

    glTexImage2D(
        GL_TEXTURE_2D, 
		0, 
		internal_format, 
		data.width, 
		data.height, 
		0, 
		data_format,
		GL_UNSIGNED_BYTE, 
		data.data
    );

    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

auto Texture::delete_gpu() const -> void
{
    glDeleteTextures(1, &handle_);
}

auto Texture::gl_handle() const -> std::uint32_t
{
    return handle_;
}

auto Texture::width() const -> int
{
    return width_;
}

auto Texture::height() const -> int
{
    return height_;
}

auto Texture::name() const -> std::string
{
    return name_;
}


} // namespace NoctisEngine