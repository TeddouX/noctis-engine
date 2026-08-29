#pragma once
#include <cstdint>
#include <string_view>
#include <span>
#include <vector>
#include <stdexcept>
#include <format>

#include "../noctis_engine.hpp"
#include "../core/logging.hpp"


namespace NoctisEngine::Rendering
{

enum class BufferTarget : std::uint32_t 
{
    /// @brief Vertex attributes
    ARRAY_BUFFER = 0x8892,

    /// @brief Atomic counter storage
    ATOMIC_COUNTER_BUFFER = 0x92C0,

    /// @brief Buffer copy source
    COPY_READ_BUFFER = 0x8F36,

    /// @brief Buffer copy destination
    COPY_WRITE_BUFFER = 0x8F37,

    /// @brief Indirect compute dispatch commands
    DISPATCH_INDIRECT_BUFFER = 0x90EE,

    /// @brief Indirect command arguments
    DRAW_INDIRECT_BUFFER = 0x8F3F,

    /// @brief Vertex array indices
    ELEMENT_ARRAY_BUFFER = 0x8893,

    /// @brief Pixel read target
    PIXEL_PACK_BUFFER = 0x88EB,

    /// @brief Texture data source
    PIXEL_UNPACK_BUFFER = 0x88EC,

    /// @brief Query result buffer
    QUERY_BUFFER = 0x9192,

    /// @brief Read-write storage for shaders
    SHADER_STORAGE_BUFFER = 0x90D2,

    /// @brief Texture data buffer
    TEXTURE_BUFFER = 0x8C2A,

    /// @brief Transform feedback buffer
    TRANSFORM_FEEDBACK_BUFFER = 0x8C8E,

    /// @brief Uniform block storage
    UNIFORM_BUFFER = 0x8A11,
};

enum class BufferFlag : std::uint32_t 
{
    MAP_READ_BIT = 0x0001,
    MAP_WRITE_BIT = 0x0002,
    MAP_PERSISTENT_BIT = 0x0040,
    MAP_COHERENT_BIT = 0x0080,
    DYNAMIC_STORAGE_BIT = 0x0100,
    CLIENT_STORAGE_BIT = 0x0200,
};

BufferFlag operator |(BufferFlag left, BufferFlag right);

enum class GPUBufMapAccess : std::uint32_t 
{
    READ_ONLY = 0x88B8,
    WRITE_ONLY = 0x88B9,
    READ_WRITE = 0x88BA,
};

using CPUBufferReadView = const std::span<const std::byte>;
using CPUBufferWriteView = std::span<std::byte>;

class GPUBuffer 
{
public:
    GPUBuffer() = default;
    GPUBuffer(std::int64_t size, std::string_view name, BufferFlag flags = BufferFlag::DYNAMIC_STORAGE_BIT);

    auto copy_to(GPUBuffer &other) -> void;
    auto write(CPUBufferReadView data, std::int64_t offset) const -> void;
    auto write(const void *data, std::int64_t size, std::int64_t offset) const -> void;
    auto size_bytes() const -> std::size_t;
    auto gl_handle() const -> std::uint32_t;

    auto delete_gpu() -> void;

    // Returns a pointer to the mapped buffer, can be cast to any type.
    // GPUBuffer::mapped_write should be used instead of writing the raw pointer if performance isn't that important because it is more memory safe.
    auto map() -> void *;
    auto unmap() -> void;
    auto is_mapped() const -> bool;
    // The pointer shouldn't be kept and used after calling resize_buffer(), nullptr if the buffer is unmapped
    auto get_mapped_ptr() -> void *;

    auto get_data(std::size_t offset, CPUBufferWriteView data) const -> void;

    auto get_flags() const -> BufferFlag;

    /// TODO: buffer should bind itself 

    auto get_name() const -> std::string_view;

private:
    std::uint32_t       id_;
    std::int64_t        size_;
    void               *map_;
    BufferFlag          flags_;

    std::string_view    name_;
};

struct GPUBufferBlock 
{
    std::size_t offset;
    std::size_t size;
};

} // namespace NoctisEngine::Rendering