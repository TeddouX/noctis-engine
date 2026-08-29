#include <noctis_engine/rendering/gpu_buffer.hpp>

#include <format>
#include <cstring>
#include <GL/gl.h>

#include <noctis_engine/core/logging.hpp>
#include <noctis_engine/core/exit.hpp>


namespace NoctisEngine::Rendering
{
    
static auto is_bindable_to_index(BufferTarget type) -> bool;

BufferFlag operator |(BufferFlag left, BufferFlag right) 
{
    return static_cast<BufferFlag>(
        static_cast<std::uint32_t>(left) | static_cast<std::uint32_t>(right)
    );
}

GPUBuffer::GPUBuffer(std::int64_t size, std::string_view name, BufferFlag flags)
    : size_{size}
    , name_{name}
    , flags_{flags}
    , map_{nullptr}
    , id_{}
{
    glCreateBuffers(1, &id_);
    glNamedBufferStorage(id_, size, nullptr, static_cast<GLbitfield>(flags));
    glObjectLabel(GL_BUFFER, id_, name.size(), name.data());
}

auto GPUBuffer::write(CPUBufferReadView data, GLintptr offset) const -> void 
{
    write(data.data(), data.size_bytes(), offset);
}

auto GPUBuffer::write(const void *data, std::int64_t size, std::int64_t offset) const -> void
{
    if (offset + size > size_)
    {
        RENDERING_LOGGER.error(
            "Tried to write {} bytes at offset {} into a buffer that is {} bytes long.", 
            size, offset, size_
        );

        return;
    }

    glNamedBufferSubData(id_, offset, size, data);
}

auto GPUBuffer::copy_to(GPUBuffer &other) -> void 
{
    if (size_ > other.size_)
    {
        RENDERING_LOGGER.error(
            "Tried copying from a buffer that is {} bytes long to a buffer that is {} bytes long",
            size_, other.size_
        );

        return;
    }

    glCopyNamedBufferSubData(id_, other.id_, 0, 0, size_);
}

auto GPUBuffer::size_bytes() const -> std::size_t 
{
    return size_;
}

auto GPUBuffer::gl_handle() const -> std::uint32_t 
{
    return id_;
}

auto GPUBuffer::delete_gpu() -> void
{
    if (map_)
        glUnmapNamedBuffer(id_);
    glDeleteBuffers(1, &id_);
}

auto GPUBuffer::map() -> void * 
{
    map_ = glMapNamedBufferRange(id_, 0, size_, static_cast<GLbitfield>(flags_));
    if (!map_)
    {
        RENDERING_LOGGER.critical("Failed to map buffer {}.", name_);
        Core::exit_program_failure();
    }

    return map_;
}

auto GPUBuffer::unmap() -> void 
{
    glUnmapNamedBuffer(id_);
    map_ = nullptr;
}

auto GPUBuffer::is_mapped() const -> bool 
{
    return map_ != nullptr;
}

auto GPUBuffer::get_mapped_ptr() -> void * 
{
    return map_;
}

auto GPUBuffer::get_data(std::size_t offset, CPUBufferWriteView data) const -> void 
{
    if (offset + data.size_bytes() > size_)
    {
        RENDERING_LOGGER.error(
            "Failed to read data at offset {} with an object size of {} bytes because it exceeds the buffer's size ({} bytes)" ,
            offset, data.size_bytes(), size_
        );

        return;
    }

    glGetNamedBufferSubData(
        id_, 
        offset, 
        data.size_bytes(), 
        data.data()
    );
}

auto GPUBuffer::get_flags() const -> BufferFlag 
{
    return flags_;
}

auto GPUBuffer::get_name() const -> std::string_view 
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

} // namespace NoctisEngine::Rendering