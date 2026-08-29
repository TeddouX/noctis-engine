#pragma once
#include "gpu_buffer.hpp"

namespace NoctisEngine::Rendering
{
    
inline auto resize_buffer(GPUBuffer &buf, std::int64_t buf_size, bool copy = false) -> bool 
{
    if (buf.size() >= buf_size) 
        return false;

    std::size_t new_buf_size = std::max(buf.size() * 2, 1zu);
    while (new_buf_size < buf_size)
        new_buf_size *= 2;
    
    RENDERING_LOGGER.debug("Resizing buffer '{}', {} => {}", buf.name(), buf.size(), new_buf_size);


    GPUBuffer new_buf{new_buf_size, buf.name(), buf.flags()};

    if (copy)
        buf.copy_to(new_buf);
    
    if (buf.mapped()) 
    {
        RENDERING_LOGGER.debug("Remapping buffer '{}'", buf.name());
        new_buf.map(buf.map_access());
    }

    buf.delete_gpu();
    buf = new_buf;

    return true;
}

template <typename T>
auto resize_buffer(GPUBuffer &buf, const std::vector<T> &cpu_buf, bool copy = false) -> bool 
{
    std::int64_t cpuBufSize = cpu_buf.size() * sizeof(T);
    return resize_buffer(buf, cpuBufSize, copy);
}

template <typename T>
auto get_cpu_buffer_view(const std::vector<T> &buf, std::size_t offset, std::size_t size) -> CPUReadView 
{
    if (size + offset > buf.size())
    {
        RENDERING_LOGGER.critical(
            "Can't create cpu buffer view, invalid offset and/or size (off {} + size {} > buf {}).", 
            offset, size, buf.size()
        );

        return {};
    }

    return std::as_bytes(std::span{buf.data() + offset, size});
}

template <typename T>
auto get_cpu_buffer_view(const T &val) -> CPUReadView 
{
    return std::as_bytes(std::span{std::addressof(val), 1});
}

template <typename T>
auto get_cpu_buffer_view(T &val) -> CPUWriteView 
{
    return std::as_writable_bytes(std::span{std::addressof(val), 1});
}

} // namespace NoctisEngine::Rendering