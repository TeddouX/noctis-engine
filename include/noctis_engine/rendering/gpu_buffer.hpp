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
    /// @brief This flag means the buffer will not have any special behaviour.
    /// It also means it can't be mapped
    NONE = 0,

    /// @brief This flag allows the buffer to be mapped for CPU reading
    MAP_READ_BIT = 0x0001,

    /// @brief This flag allows the buffer to be mapped for CPU writing
    MAP_WRITE_BIT = 0x0002,

    /// @brief This flag allows the buffer to remain mapped while the GPU is also using it
    /// This allows you to not unbind it each time you need the GPU to use it
    /// @important requires MAP_READ_BIT or MAP_WRITE_BIT to be set
    MAP_PERSISTENT_BIT = 0x0040,

    /// @brief This flag guarantees writes/reads between CPU and GPU stay 
    /// automatically visible to each other without needing explicit memory barriers
    /// @important Requires MAP_PERSISTENT_BIT to be set
    MAP_COHERENT_BIT = 0x0080,

    /// @brief This flag allows the buffer's contents to be updated later via write()
    DYNAMIC_STORAGE_BIT = 0x0100,

    /// @brief This flag is hint to the driver that it should prefer keeping the buffer's backing storage in CPU
    /// useful when CPU access is more frequent than GPU access.
    CLIENT_STORAGE_BIT = 0x0200,
};

/// @brief Binary or operation for buffer flags
/// @param left The left flag
/// @param right The right flag
/// @return The or'ed flags
constexpr auto operator |(BufferFlag left, BufferFlag right) -> BufferFlag
{
    return static_cast<BufferFlag>(
        static_cast<std::uint32_t>(left) | static_cast<std::uint32_t>(right)
    );
}


/// @brief Describes how to map a buffer to RAM
enum class BufferMapAccess : std::uint32_t 
{
    /// @brief This is an invalid map access used when a buffer is unmapped
    /// @important DO NOT USE WHEN MAPPING BUFFERS
    NONE = 0,

    /// @brief This flag allows the buffer to be mapped for CPU reading
    MAP_READ_BIT = 0x0001,

    /// @brief This flag allows the buffer to be mapped for CPU writing
    MAP_WRITE_BIT = 0x0002,

    /// @brief This flag allows the buffer to remain mapped while the GPU is also using it.
    /// This allows you to not unbind it each time you need the GPU to use it
    /// @important requires MAP_READ_BIT or MAP_WRITE_BIT to be set
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
constexpr auto operator |(BufferMapAccess left, BufferMapAccess right) -> BufferMapAccess
{
    return static_cast<BufferMapAccess>(
        static_cast<std::uint32_t>(left) | static_cast<std::uint32_t>(right)
    );
}


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
    /// @param name The buffer's name, must outlive the buffer
    /// @param flags The buffer's flags. You can combine multiple 
    /// of them using the binary or operator
    GPUBuffer(std::size_t size, std::string_view name, BufferFlag flags);

    ~GPUBuffer() = default;

    /// @brief Resizes a GPU buffer if necessary by creating a new one. The new one's size will 
    /// always be a power of two. Optionally copies data to the new buffer
    /// @param buffer The buffer that should be resized
    /// @param req_size The buffer's new required size
    /// @param copy_data true if it should copy data from the old buffer to the new one
    /// @return true if the buffer was resized, false otherwise
    /// @warning This doesn't remap the buffer
    static auto resize(GPUBuffer &buffer, std::size_t new_size, bool copy_data = false) -> bool;

    /// @brief Maps this buffer to RAM, for easier read and write from the CPU
    /// @param access The access modifiers for the mapped buffer. You must put at most the flags 
    /// that were requested at buffer creation, for example, if MAP_READ_BIT is set at creation 
    /// but not MAP_WRITE_BIT, you can't map the buffer for writing, but you can still map it for reading.
    /// You can combine multiple map accesses using the binary or operator
    /// @param offset The starting offset within the buffer of the range to be mapped. Default 0
    /// @param length The length of the range to be mapped. Set to 0 to use the size of the buffer (default), 
    /// any other positive number will use the provided length
    /// @return The mapped pointer
    /// @important This buffer must've created with either the MAP_READ_BIT flag or the MAP_WRITE_BIT flag 
    /// for this function to not cause an OpenGL error.
    auto map(BufferMapAccess access, std::size_t offset = 0, std::size_t length = 0) -> void *;

    /// @brief Unmaps this buffer, any pointers left to the mapped range will be invalidated
    auto unmap() -> void;

    /// @return The mapped pointer, nullptr if the buffer isn't mapped 
    auto mapped_ptr() -> void *;

    /// @return true if the buffer is mapped false otherwise 
    auto mapped() -> bool;

    /// @brief Writes to a buffer
    /// @param data The data that should be written
    /// @param offset The offset inside the buffer it should be written at
    /// @important This buffer must've created with the GL_DYNAMIC_STORAGE flag for this function 
    /// to not cause an OpenGL error
    auto write(CPUReadView data, std::size_t offset) const -> void;

    /// @brief Writes to a buffer
    /// @param data The type erased data that should be written
    /// @param size The size in bytes of the data that should be written
    /// @param offset The offset inside the buffer it size_t be written at
    /// @important This buffer must've created with the GL_DYNAMIC_STORAGE flag for this function 
    /// to not cause an OpenGL error
    auto write(const void *data, std::size_t size, std::size_t offset) const -> void;

    /// @brief Copies this buffer's data to another buffer
    /// @param other The other buffer
    /// @param read_offset The starting offset within this buffer of the range to be copied. Default 0
    /// @param write_offset The starting offset where this buffer's data will be written to. Default 0
    /// @param length The length of the range to be copied. Set to 0 to use the size of the buffer (default), 
    /// any other positive number will use the provided length
    auto copy_to(
        const GPUBuffer    &other, 
        std::size_t         read_offset = 0, 
        std::size_t         write_offset = 0, 
        std::size_t         length = 0
    ) const -> void;

    /// @brief This describes a flushing range
    using FlushRange = std::pair<std::size_t, std::size_t>;

    /// @brief This indicates that modifications have been made to a mapped buffer.
    /// @important The buffer must have been mapped with the MAP_FLUSH_EXPLICIT_BIT flag.
    /// @param ranges The ranges that were modified
    auto flush_mapped_buffer_ranges(const std::vector<FlushRange> &ranges) const -> void;

    /// @brief Deletes this buffer and its data from the GPU.
    /// The buffer shouldn't be used after deletion
    auto delete_gpu() -> void;
    
    /// @return The size of this buffer in bytes
    auto size() const -> std::size_t;

    /// @return This buffer's creation flags 
    auto flags() const -> BufferFlag;

    /// @return This buffer's map accesses 
    auto map_access() const -> BufferMapAccess;

    /// @return This buffer's OpenGL handle 
    auto gl_handle() const -> std::uint32_t;

    /// @return The buffer's name
    auto name() const -> std::string_view;

private:
    std::uint32_t       handle_;
    std::size_t         size_;
    std::string_view    name_;
    void               *map_;

    BufferFlag          flags_;
    BufferMapAccess     map_access_;
};

} // namespace NoctisEngine::Rendering