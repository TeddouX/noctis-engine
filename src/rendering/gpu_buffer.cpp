#include <noctis_engine/rendering/gpu_buffer.hpp>

#include <format>
#include <cstring>
#include <GL/gl.h>

#include <noctis_engine/core/logging.hpp>
#include <noctis_engine/core/format.hpp>


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
    if (offset + data.size_bytes() > size_)
    {
        RENDERING_LOGGER.critical(
            "Tried to write {} bytes at offset {} into a buffer that is {} bytes long.", 
            data.size_bytes(), offset, size_
        );
    }

    glNamedBufferSubData(id_, offset, data.size_bytes(), data.data());
}

auto GPUBuffer::copy_to(GPUBuffer &other) -> void 
{
    if (size_ > other.size_)
        RENDERING_LOGGER.critical(
            "Tried copying from a buffer that is {} bytes long to a buffer that is {} bytes long",
            size_, other.size_
        );

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
        RENDERING_LOGGER.critical("Failed to map buffer.");

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
        RENDERING_LOGGER.critical(
            "Failed to read data at offset {} with an object size of {} bytes because it exceeds the buffer's size ({} bytes)" ,
            offset, data.size_bytes(), size_
        );

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


auto to_string(BufferTarget type) -> std::string 
{
    switch (type) 
    {
        using enum BufferTarget;
        case ARRAY_BUFFER:              return "ARRAY_BUFFER"; 
        case ATOMIC_COUNTER_BUFFER:     return "ATOMIC_COUNTER_BUFFER"; 
        case COPY_READ_BUFFER:          return "COPY_READ_BUFFER"; 
        case COPY_WRITE_BUFFER:         return "COPY_WRITE_BUFFER"; 
        case DISPATCH_INDIRECT_BUFFER:  return "DISPATCH_INDIRECT_BUFFER"; 
        case DRAW_INDIRECT_BUFFER:      return "DRAW_INDIRECT_BUFFER"; 
        case ELEMENT_ARRAY_BUFFER:      return "ELEMENT_ARRAY_BUFFER"; 
        case PIXEL_PACK_BUFFER:         return "PIXEL_PACK_BUFFER"; 
        case PIXEL_UNPACK_BUFFER:       return "PIXEL_UNPACK_BUFFER"; 
        case QUERY_BUFFER:              return "QUERY_BUFFER"; 
        case SHADER_STORAGE_BUFFER:     return "SHADER_STORAGE_BUFFER"; 
        case TEXTURE_BUFFER:            return "TEXTURE_BUFFER"; 
        case TRANSFORM_FEEDBACK_BUFFER: return "TRANSFORM_FEEDBACK_BUFFER"; 
        case UNIFORM_BUFFER:            return "UNIFORM_BUFFER";
        default:
            RENDERING_LOGGER.info("Unsupported BufferTarget: {}", static_cast<int>(type));
            return "unknown";
    }
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