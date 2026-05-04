#pragma once
#include <stdint.h>
#include <string_view>
#include <span>
#include <vector>
#include <stdexcept>
#include <format>

#include "../noctis_engine.hpp"
#include "../core/exception.hpp"
#include "../core/logging.hpp"

namespace NoctisEngine
{

enum class BufferTarget : std::uint32_t {
    ARRAY_BUFFER = 0x8892,               // Vertex attributes
    ATOMIC_COUNTER_BUFFER = 0x92C0,      // Atomic counter storage
    COPY_READ_BUFFER = 0x8F36,           // Buffer copy source
    COPY_WRITE_BUFFER = 0x8F37,          // Buffer copy destination
    DISPATCH_INDIRECT_BUFFER = 0x90EE,   // Indirect compute dispatch commands
    DRAW_INDIRECT_BUFFER = 0x8F3F,       // Indirect command arguments
    ELEMENT_ARRAY_BUFFER = 0x8893,       // Vertex array indices
    PIXEL_PACK_BUFFER = 0x88EB,          // Pixel read target
    PIXEL_UNPACK_BUFFER = 0x88EC,        // Texture data source
    QUERY_BUFFER = 0x9192,               // Query result buffer
    SHADER_STORAGE_BUFFER = 0x90D2,      // Read-write storage for shaders
    TEXTURE_BUFFER = 0x8C2A,             // Texture data buffer
    TRANSFORM_FEEDBACK_BUFFER = 0x8C8E,  // Transform feedback buffer
    UNIFORM_BUFFER = 0x8A11,             // Uniform block storage
};

enum class BufferFlag : std::uint32_t {
    MAP_READ_BIT = 0x0001,
    MAP_WRITE_BIT = 0x0002,
    MAP_PERSISTENT_BIT = 0x0040,
    MAP_COHERENT_BIT = 0x0080,
    DYNAMIC_STORAGE_BIT = 0x0100,
    CLIENT_STORAGE_BIT = 0x0200,
};

NCENG_API BufferFlag operator |(BufferFlag left, BufferFlag right);

enum class GPUBufMapAccess : std::uint32_t {
    READ_ONLY = 0x88B8,
    WRITE_ONLY = 0x88B9,
    READ_WRITE = 0x88BA,
};

NCENG_API auto to_string(BufferTarget type) -> std::string;

using CPUBufferReadView = const std::span<const std::byte>;
using CPUBufferWriteView = std::span<std::byte>;

class NCENG_API GPUBuffer {
public:
    GPUBuffer() = default;
    GPUBuffer(size_t size, std::string_view name, BufferFlag flags = BufferFlag::DYNAMIC_STORAGE_BIT);

    auto copy_to(GPUBuffer &other) -> void;
    auto write(CPUBufferReadView data, size_t offset) const -> void;
    auto size_bytes() const -> size_t;
    auto gl_handle() const -> std::uint32_t;

    auto delete_gpu() -> void;

    auto bind_as(BufferTarget type) const -> void;
    auto bind_buffer_base(BufferTarget type, uint32_t bindPoint) const -> void;
    auto bind_buffer_range(BufferTarget type, uint32_t bindPoint, size_t offset, size_t size) const -> void;

    // Returns a pointer to the mapped buffer, can be cast to any type.
    // GPUBuffer::mapped_write should be used instead of writing the raw pointer if performance isn't that important because it is more memory safe.
    auto map() -> void *;
    auto unmap() -> void;
    auto mapped_write(CPUBufferReadView data, size_t offset) -> void;
    auto is_mapped() const -> bool;
    // The pointer shouldn't be kept and used after calling resize_buffer(), nullptr if the buffer is unmapped
    auto get_mapped_ptr() -> void *;

    auto get_data(std::size_t offset, CPUBufferWriteView data) const -> void;

    auto get_flags() const -> BufferFlag;

    auto get_name() const -> std::string_view;

private:
    uint32_t            id_;
    size_t              size_;
    void               *map_;
    BufferFlag          flags_;

    std::string_view    name_;
};

struct GPUBufferBlock {
    std::size_t offset;
    std::size_t size;
};

} // namespace NoctisEngine
