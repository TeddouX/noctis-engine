#include <noctis_engine/rendering/gpu_buffer.hpp>

#include <format>
#include <cstring>
#include <GL/gl.h>

#include <noctis_engine/core/logging.hpp>
#include <noctis_engine/core/exit.hpp>


namespace NoctisEngine
{
    
static auto is_bindable_to_index(BufferTarget type) -> bool;


GPUBuffer::GPUBuffer(std::size_t size, std::string_view name, BufferFlag flags)
    : size_{size}
    , flags_{flags}
    , name_{name}
    , map_{nullptr}
{
    glCreateBuffers(1, &handle_);
    glNamedBufferStorage(handle_, size_, nullptr, static_cast<GLbitfield>(flags_));
    glObjectLabel(GL_BUFFER, handle_, name_.size(), name_.data());
}

auto GPUBuffer::resize(GPUBuffer &buffer, std::size_t req_size, bool copy_data) -> bool
{
    if (buffer.size() >= req_size)
        return false;

    std::size_t new_buf_size = std::max(buffer.size() * 2, 1zu);
    while (new_buf_size < req_size)
        new_buf_size *= 2;

    RENDERING_LOGGER.debug("Resizing buffer \"{}\", {} => {}", 
        buffer.name(), buffer.size(), new_buf_size
    );

    GPUBuffer new_buf{new_buf_size, buffer.name(), buffer.flags()};

    if (copy_data)
        buffer.copy_to(new_buf);
    
    buffer.delete_gpu();
    buffer = new_buf;

    return true;
}

auto GPUBuffer::map(BufferMapAccess access, std::size_t offset, std::size_t length) -> void *
{
    map_ = glMapNamedBufferRange(
        handle_,
        offset,
        length == 0 ? size_ : length,
        static_cast<GLbitfield>(access)
    );
    
    if (!map_)
    {
        RENDERING_LOGGER.error("Failed to map buffer {}.", name_);
        return nullptr;
    }

    map_access_ = access;

    return map_;
}

auto GPUBuffer::unmap() -> void
{
    glUnmapNamedBuffer(handle_);
    
    map_ = nullptr;
    map_access_ = BufferMapAccess::NONE;
}

auto GPUBuffer::mapped_ptr() -> void *
{
    return map_;
}

auto GPUBuffer::mapped() -> bool
{
    return map_ != nullptr;
}

auto GPUBuffer::write(CPUReadView data, std::size_t offset) const -> void
{
    write(data.data(), data.size_bytes(), offset);
}

auto GPUBuffer::write(const void *data, std::size_t size, std::size_t offset) const -> void
{
    if (offset + size > size_)
    {
        RENDERING_LOGGER.error(
            "Tried to write {} bytes at offset {} into a buffer that is {} bytes long (buffer \"{}\")", 
            size, offset, size_, name_
        );

        return;
    }

    glNamedBufferSubData(handle_, offset, size, data);
}

auto GPUBuffer::copy_to(
    const GPUBuffer    &other, 
    std::size_t         read_offset, 
    std::size_t         write_offset, 
    std::size_t         length
) const -> void
{
    if (size_ > other.size_)
    {
        RENDERING_LOGGER.error(
            "Tried copying from a buffer that is {} bytes long to a buffer that is {} bytes long",
            size_, other.size_
        );

        return;
    }

    glCopyNamedBufferSubData(
        handle_, 
        other.handle_, 
        read_offset, write_offset, 
        length == 0 ? size_ : length
    );
}

auto GPUBuffer::flush_mapped_buffer_ranges(const std::vector<FlushRange> &ranges) const -> void
{
    if (not map_)
    {
        RENDERING_LOGGER.error(
            "Can't flush mapped buffer range if it wasn't mapped (buffer \"{}\")",
            name_
        );

        return;
    }

    if ((static_cast<std::uint32_t>(map_access_) 
      & static_cast<std::uint32_t>(BufferMapAccess::MAP_UNSYNCHRONIZED_BIT)) == 0)
    {
        RENDERING_LOGGER.error(
            "Can't flush mapped buffer range if it wasn't mapped with the MAP_UNSYNCHRONIZED_BIT (buffer \"{}\")",
            name_
        );

        return;
    }

    for (const auto &range : ranges)
        glFlushMappedNamedBufferRange(handle_, range.first, range.second);
}

auto GPUBuffer::delete_gpu() -> void
{
    if (map_)
        unmap();

    glDeleteBuffers(1, &handle_);
}

auto GPUBuffer::size() const -> std::size_t
{
    return size_;
}

auto GPUBuffer::flags() const -> BufferFlag
{
    return flags_;
}

auto GPUBuffer::map_access() const -> BufferMapAccess
{
    return map_access_;
}

auto GPUBuffer::gl_handle() const -> std::uint32_t
{
    return handle_;
}

auto GPUBuffer::name() const -> std::string_view
{
    return name_;
}


auto is_bindable_to_index(BufferTarget type) -> bool 
{
    switch (type) 
    {
        using enum BufferTarget;
        case ATOMIC_COUNTER_BUFFER:
        case SHADER_STORAGE_BUFFER:
        case TRANSFORM_FEEDBACK_BUFFER:
        case UNIFORM_BUFFER:
            return true;
        
        default:
            return false;
    }
}

} // namespace NoctisEngine