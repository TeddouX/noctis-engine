#include <noctis_engine/rendering/vertex_array.hpp>

#include <GL/gl.h>

#include <noctis_engine/rendering/buffer_utils.hpp>


namespace NoctisEngine
{
    
auto attrib_type_size_bytes(VertexAttribute::ComponentType type) -> std::size_t;


VertexArray::VertexArray(
    const std::vector<VertexAttribute> &vertex_attribs, 
    std::string_view                    name, 
    bool                                create_buffers,
    bool                                use_ebo)
    : use_ebo_{use_ebo}
{
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    glObjectLabel(GL_VERTEX_ARRAY, vao_, name.size(), name.data());

    if (create_buffers)
    {
        vbo_ = GPUBuffer(1, name, BufferFlag::DYNAMIC_STORAGE_BIT);

        if (use_ebo_)
            ebo_ = GPUBuffer(1, name, BufferFlag::DYNAMIC_STORAGE_BIT);
    }

    std::uint32_t attrib_idx{0};
    std::uint32_t offset{0};

    for (const auto &vertex_attrib : vertex_attribs)
    {
        GLuint gl_attrib_idx = vertex_attrib.index < 0 ? attrib_idx : vertex_attrib.index;

        glVertexAttribFormat(
            gl_attrib_idx,
            vertex_attrib.num_components, 
            static_cast<GLenum>(vertex_attrib.component_type), 
            vertex_attrib.normalized ? GL_TRUE : GL_FALSE, 
            offset
        );
        glVertexAttribBinding(gl_attrib_idx, 0);
        glEnableVertexAttribArray(gl_attrib_idx);
    
        attrib_idx++;
        offset += attrib_type_size_bytes(vertex_attrib.component_type) * vertex_attrib.num_components;
    }

    if (create_buffers)
    {
        // Offset should have the total size of one vertex by now
        link_vbo(vbo_, offset);

        if (use_ebo_)
            link_ebo(ebo_);
    }
}

auto VertexArray::upload_vertices(void *vertices, std::size_t size, std::size_t vertex_size) -> void
{
    bool resized = GPUBuffer::resize(vbo_, size);
    vbo_.write(vertices, size, 0);

    // Relink if resized because a new buffer was created
    if (resized)
        link_vbo(vbo_, vertex_size, 0);
}

auto VertexArray::upload_indices(const std::vector<std::uint32_t> &indices) -> void
{
    if (not use_ebo_)
    {
        RENDERING_LOGGER.error("Can't upload indices to a vertex array that doesn't use an EBO");
        return;
    }

    bool resized = GPUBuffer::resize(ebo_, indices.size());
    ebo_.write(get_cpu_read_view(indices, 0), 0);

    // Relink if resized because a new buffer was created
    if (resized)
        link_ebo(ebo_);
}

auto VertexArray::link_vbo(const GPUBuffer &vbo_buf, std::size_t vertex_size, std::size_t first_el_off) -> void
{
    glVertexArrayVertexBuffer(vao_, 0, vbo_buf.gl_handle(), first_el_off, vertex_size);
    vbo_ = vbo_buf;
}

auto VertexArray::link_ebo(const GPUBuffer &ebo_buf) -> void
{
    glVertexArrayElementBuffer(vao_, ebo_buf.gl_handle());
    ebo_ = ebo_buf;
    use_ebo_ = true;
}

auto VertexArray::delete_gpu() -> void
{
    glDeleteVertexArrays(1, &vao_);
    vbo_.delete_gpu();
    ebo_.delete_gpu();
}

auto VertexArray::bind(DrawList &draw_list) -> void
{
    draw_list.bind_vao(vao_);
    if (use_ebo_)
        draw_list.bind_buffer(ebo_, BufferTarget::ELEMENT_ARRAY_BUFFER);
}

auto VertexArray::vao_gl_handle() -> std::uint32_t
{
    return vao_;
}

auto VertexArray::ebo() -> GPUBuffer &
{
    return ebo_;
}

auto VertexArray::vbo() -> GPUBuffer &
{
    return vbo_;
}


auto attrib_type_size_bytes(VertexAttribute::ComponentType type) -> std::size_t
{
    switch (type)
    {
        using enum VertexAttribute::ComponentType;
        case BYTE: 
            return 1;

        case SHORT:
        case HALF_FLOAT:
            return 2;

        case INT:
        case FLOAT:
        case INT_2_10_10_10_REV:
        case UNSIGNED_INT_2_10_10_10_REV:
        case UNSIGNED_INT_10F_11F_11F_REV:
            return 4;

        case DOUBLE:
            return 8;

        default:
            return 0;
    }
}

} // namespace NoctisEngine
