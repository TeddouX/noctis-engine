#pragma once
#include "gpu_buffer.hpp"

namespace NoctisEngine
{

/// @brief Creates a CPU read view from a vector
/// @tparam T The type of the vector's elements
/// @param buf The vector used to create the read view
/// @param offset The offset the read view should be created from.
/// @param length The length of the read view. Set to 0 to use the length of the vector (default), 
/// any other positive number will use the provided length
/// @return The created read view
template <typename T>
auto get_cpu_read_view(const std::vector<T> &buf, std::size_t offset, std::size_t length = 0) -> CPUReadView 
{
    if (length + offset > buf.size())
    {
        RENDERING_LOGGER.error(
            "Can't create cpu buffer view, invalid offset and/or size (off {} + length {} > buf.size {}).", 
            offset, length, buf.size()
        );

        return {};
    }

    return std::as_bytes(std::span{
        buf.data() + offset, 
        length == 0 ? buf.size() : length
    });
}

/// @brief Creates a CPU read view from an object
/// @tparam T The object type
/// @param val The object
/// @return The created read vie
template <typename T>
auto get_cpu_read_view(const T &val) -> CPUReadView 
{
    return std::as_bytes(std::span{std::addressof(val), 1});
}

} // namespace NoctisEngine