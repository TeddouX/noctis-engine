#pragma once
#include <cstdint>
#include <vector>

#include "gpu_buffer.hpp"
#include "draw_list.hpp"
#include "vertex_attrib.hpp"


namespace NoctisEngine::Rendering
{

/// @brief This class is used to store vertices for drawing on the GPU
class VertexArray
{
public:
    /// @brief Default constructor. Doesn't create anything on the GPU.
    VertexArray() = default;

    /// @brief Creates a VAO on the GPU
    /// @param vertex_attribs This vertex array's vertex attributes
    /// @param name This vertex array's name. Default "vertex_array"
    /// @param create_buffers Specifies if the vertex array should create the VBO and, possibly (see use_ebo), 
    /// EBO buffers and link them. True by default
    /// @param use_ebo Specifies if the vertex array should use an EBO buffer. 
    /// Doesn't do anything if create buffers is set to false.
    VertexArray(
        const std::vector<VertexAttribute> &vertex_attribs,
        std::string_view                    name = "vertex_array",
        bool                                create_buffers = true,
        bool                                use_ebo = true
    );

    /// @brief Uploads vertices to this vertex array
    /// @param vertices The type erased vertices
    /// @param size The size in bytes of the vertices
    /// @param vertex_size The size of one vertex
    /// @warning This should match vertex attributes provided in the constructor.
    /// Failure to do so will result in weird crashes or undefined behaviour.
    auto upload_vertices(void *vertices, std::size_t size, std::size_t vertex_size) -> void;

    /// @brief Uploads indices linked to this vertex array
    /// @param indices The indices
    /// @warning For this function to work, use_ebo should be set to true in the construct, 
    /// or an ebo should be linked
    auto upload_indices(const std::vector<std::uint32_t> &indices) -> void;

    /// @brief Links a vertex buffer object to this vertex array.
    /// Use this if you want precise memory control over your vertex buffer.
    /// The VBO should be relinked when it gets resized.
    /// @param vbo_buf The vertex buffer
    /// @param vertex_size The size of one vertex
    /// @param first_el_off The offset of the first element of the buffer. Default 0
    auto link_vbo(const GPUBuffer &vbo_bu, std::size_t vertex_size, std::size_t first_el_off = 0) -> void;

    /// @brief Links a element buffer object (indices) to this vertex array.
    /// Use this if you want precise memory control over your element buffer.
    /// The EBO should be relinked when it gets resized.
    /// @param ebo_buf The element buffer
    auto link_ebo(const GPUBuffer &ebo_buf) -> void;

    /// @brief Deletes the object from the GPU. 
    /// This vertex array shouldn't be used after calling this function.
    /// Also deletes linked buffers.
    auto delete_gpu() -> void;

    /// @brief Binds this VAO and its linked EBO to a draw list
    /// @param draw_list The draw list this vertex array should be bound to
    auto bind(DrawList &draw_list) -> void;

    /// @return The VAO's OpenGL handle  
    auto vao_gl_handle() -> std::uint32_t;

    /// @return Linked EBO 
    auto ebo() -> GPUBuffer &;
    
    /// @return Linked VBO 
    auto vbo() -> GPUBuffer &;

private:
    std::uint32_t   vao_;

    bool            use_ebo_;
    GPUBuffer       ebo_;
    
    GPUBuffer       vbo_;
};

} // namespace NoctisEngine::Rendering
