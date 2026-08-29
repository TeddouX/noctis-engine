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

/// @brief Describes all targets a buffer can be bound to
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

/// @brief Describes different flags a buffer can have
enum class BufferFlag : std::uint32_t 
{
    /// @brief This flag allows the buffer to be mapped for CPU reading
    MAP_READ_BIT = 0x0001,

    /// @brief This flag allows the buffer to be mapped for CPU writing
    MAP_WRITE_BIT = 0x0002,

    /// @brief This flag allows the buffer to remain mapped while the GPU is also using it
    MAP_PERSISTENT_BIT = 0x0040,

    /// @brief This flag guarantees writes/reads between CPU and GPU stay 
    /// automatically visible to each other without needing explicit memory barriers
    /// @important Requires MAP_PERSISTENT_BIT to be set
    MAP_COHERENT_BIT = 0x0080,

    /// @brief This flagallows the buffer's contents to be updated later via write()
    DYNAMIC_STORAGE_BIT = 0x0100,

    /// @brief This flag is hint to the driver that it should prefer keeping the buffer's backing storage in CPU
    /// useful when CPU access is more frequent than GPU access.
    CLIENT_STORAGE_BIT = 0x0200,
};

/// @brief Binary or operation for buffer flags
/// @param left The left flag
/// @param right The right flag
/// @return The or'ed flags
auto operator |(BufferFlag left, BufferFlag right) -> BufferFlag;


/// @brief Describes how to map a buffer to RAM
enum class BufferMapAccess : std::uint32_t 
{
    /// @brief This flag allows the buffer to be mapped for CPU reading
    MAP_READ_BIT = 0x0001,

    /// @brief This flag allows the buffer to be mapped for CPU writing
    MAP_WRITE_BIT = 0x0002,

    /// @brief This flag allows the buffer to remain mapped while the GPU is also using it
    MAP_PERSISTENT_BIT = 0x0040,

    /// @brief This flag guarantees writes/reads between CPU and GPU stay 
    /// automatically visible to each other without needing explicit memory barriers
    /// @important Requires MAP_PERSISTENT_BIT to be set
    MAP_COHERENT_BIT = 0x0080,

    /// @brief This flag ndicates that the previous contents of the specified range may be discarded.
    /// Data within this range are undefined except for data written after the map.
    /// No OpenGL error will be generated if discarded data is read, but doing so may result 
    /// in undefined behaviour or crashes
    /// @important Requires MAP_READ_BIT to not be set
    MAP_INVALIDATE_RANGE_BIT = 0x0004,

    /// @brief This flag indicates that the previous contents of the entire buffer may be discarded.
    /// Data within this range are undefined except for data written after the map.
    /// No OpenGL error will be generated if discarded data is read, but doing so may result 
    /// in undefined behaviour or crashes
    /// @important Requires MAP_READ_BIT to not be set
    MAP_INVALIDATE_BUFFER_BIT = 0x0008,

    /// @brief This flag allows you to explicitly tell the driver which byte ranges were actually modified in 
    /// the buffer when unmapping it. This is useful if you map a large buffer but only write to a small portion of it. 
    /// It allows the driver to only sync the modified range(s). Ranges that were modified but weren't flushed are undefined. 
    /// Use GPUBuffer::flush_mapped_buffer_ranges() to specify the modified ranges
    /// @important Requires MAP_WRITE_BIT to be set
    MAP_FLUSH_EXPLICIT_BIT = 0x0010,

    /// @brief This flag tells the driver to skip synchronization steps when mapping the buffer, even 
    /// if the GPU is still using the buffer. This may result in undefined behaviour, race conditions, corrupted memory, ...
    /// Use really carefully.
    MAP_UNSYNCHRONIZED_BIT = 0x0020,
};

/// @brief Binary or operation for buffer map accesses
/// @param left The left access
/// @param right The right access
/// @return The or'ed map accesses
auto operator |(BufferMapAccess left, BufferMapAccess right) -> BufferMapAccess;


/// @brief A read only view of a CPU object
using CPUReadView = const std::span<const std::byte>;

/// @brief A writable view of a CPU object
using CPUWriteView = std::span<const std::byte>;


/// @brief This class represents a buffer on the GPU
class GPUBuffer
{
public:
    /// @brief Default constructor, creates an invalid buffer
    GPUBuffer() = default;

    /// @brief Creates a buffer on the GPU
    /// @param size The buffer's size
    /// @param name The buffer's name
    /// @param flags The buffer's flags. You can combine multiple 
    /// of them using the binary or operator
    GPUBuffer(std::int64_t size, std::string_view name, BufferFlag flags);

    ~GPUBuffer() = default;

    /// @brief Maps this buffer to RAM, for easier read and write from the CPU
    /// @param access The access modifiers for the mapped buffer. You can combine multiple 
    /// of them using the binary or operator
    /// @return The mapped pointer
    auto map(BufferMapAccess access) -> void *;

    /// @return The mapped pointer, `nullptr` if the buffer isn't mapped 
    auto get_mapped_ptr() -> void *;

    /// @brief Writes to a buffer
    /// @param data The data that should be written
    /// @param offset The offset inside the buffer it should be written at, default 0
    auto write(CPUReadView data, std::int64_t offset = 0) -> void;

    /// @brief Writes to a buffer
    /// @param data The type erased data that should be written
    /// @param size The size in bytes of the data that should be written
    /// @param offset The offset inside the buffer it should be written at
    auto write(const void *data, std::int64_t size, std::int64_t offset) -> void;

    /// @brief This describes a flushing range
    using FlushRange = std::pair<std::int64_t, int>;

    /// @brief This indicates that modifications have been made to a mapped buffer.
    /// @important The buffer must have been mapped with the MAP_FLUSH_EXPLICIT_BIT flag.
    /// @param ranges The ranges that were modified
    auto flush_mapped_buffer_ranges(const std::vector<FlushRange> &ranges) -> void;

    /// @brief Deletes this buffer and its data from the GPU.
    /// The buffer shouldn't be used after deletion
    auto delete_gpu() -> void;
    
    /// @return The size of this buffer in bytes
    auto size() -> std::int64_t;

    /// @return This buffer's creation flags 
    auto flags() -> BufferFlag;

    /// @return This buffer's map accesses 
    auto map_access() -> BufferMapAccess;

    /// @return This buffer's OpenGL handle 
    auto gl_handle() -> std::uint32_t;

    /// @return The buffer's name
    /// @warning This is expensive as it gets the name from the driver. 
    /// Use this sparringly 
    auto name() -> std::string;

private:
    std::uint32_t   handle_;
    std::int64_t    size_;
    
    void           *map_;

    BufferFlag      flags_;
    BufferMapAccess map_access_;
};

} // namespace NoctisEngine::Rendering