#include <rendering/texture.hpp>

#include <GL/gl.h>


namespace NoctisEngine::Rendering
{
    
Texture::Texture(TextureInfo texInfo)
    : name_(texInfo.name)
{
    glGenTextures(1, &texID_);
    glBindTexture(GL_TEXTURE_2D, texID_);
    glObjectLabel(GL_TEXTURE, texID_, -1, name_.c_str());

    GLenum internalFormat = GL_RGBA8;
    GLenum dataFormat = GL_RGBA;

    if (texInfo.nr_channels == 1)
    {
        internalFormat = GL_R8;
        dataFormat = GL_RED;
    }
    else if (texInfo.nr_channels == 3)
    {
        internalFormat = GL_RGB8;
        dataFormat = GL_RGB;
    }

	glTexImage2D(
		GL_TEXTURE_2D, 
		0, 
		internalFormat, 
		texInfo.width, 
		texInfo.height, 
		0, 
		dataFormat,
		GL_UNSIGNED_BYTE, 
		texInfo.data
	);

	glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
}

auto Texture::use(DrawList &draw_list, std::uint32_t bind_point) const -> void 
{
    draw_list.bind_texture(texID_, bind_point);
}

auto Texture::set_min_function(MinifyingFunction param) const -> void 
{
    glBindTexture(GL_TEXTURE_2D, texID_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(param));
    glBindTexture(GL_TEXTURE_2D, 0);
}

auto Texture::set_mag_function(MagnifyingFunction param) const -> void 
{
    glBindTexture(GL_TEXTURE_2D, texID_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(param));
    glBindTexture(GL_TEXTURE_2D, 0);
}

auto Texture::set_wrap_function(WrapParam paramU, WrapParam paramV) const -> void 
{
    glBindTexture(GL_TEXTURE_2D, texID_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLint>(paramU));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLint>(paramV));
    glBindTexture(GL_TEXTURE_2D, 0);
}

auto Texture::gl_handle() const -> std::uint32_t 
{
    return texID_; 
}

} // namespace NoctisEngine::Rendering