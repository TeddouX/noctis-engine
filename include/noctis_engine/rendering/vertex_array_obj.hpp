#pragma once
#include <cstdint>
#include <vector>

#include "gpu_buffer.hpp"


namespace NoctisEngine::Rendering
{

/// @brief This describes a vertex attribute, used when creating a vertex array object
struct VertexAttribute
{
    /// @brief This describes a vertex attribute's type
    enum class Type
    {
        /// @brief 8 bits integer
        BYTE = 0x1400, 
        
        /// @brief 16 bits integer
        SHORT = 0x1402, 
        
        /// @brief 32 bits integer
        INT = 0x1404, 
        
        /// @brief This is OpenGL's fixed point numeric type, calculated using the formula:
        /// `real_value = fixed / 65536.0`. This means it stores 16 bits for the integer part
        /// and 16 bits for the fractional part. Legacy type.
        FIXED = 0x140C,
        
        /// @brief 32 bits floating point number
        FLOAT = 0x1406, 
        
        /// @brief 16 bits floating point number
        HALF_FLOAT = 0x140B,
        
        /// @brief 64 bits floating point number
        DOUBLE = 0x140A,

        /// @brief A 2 bits signed integer and 3 other 10 bits signed integers stored in a 32 bits int,
        /// with the 2 bits int stored in the top 2 bits of the 32 bits int.
        /// This is commonly used to store normal or tangent vectors
        INT_2_10_10_10_REV = 0x8D9F,
        
        /// @brief A 2 bits unsigned integer and 3 10 bits unsigned integers stored in a 32 bits int,
        /// with the 2 bits int stored in the top 2 bits of the 32 bits int.
        /// This is commonly used to store normal or tangent vectors
        UNSIGNED_INT_2_10_10_10_REV = 0x8368,

        /// @brief A 10 bits floating point number and 2 11 bits floating point 
        /// numbers stored in a 32 bits int, with the 10 bits stored in the top 10 bits of the 32 bits int.
        /// This is commonly used to store HDR colors
        UNSIGNED_INT_10F_11F_11F_REV = 0x8C3B,
    };

    /// @brief This vertex attribute's index.
    /// -1 if the VAO should dynamically compute this based on the other provided vertex attributes.
    /// It is recommended to leave it at -1.
    int index = -1;

    /// @brief This vertex attribute's number of components. Must be 1, 2, 3 or 4.
    /// This is useful for vectors, for example a glm::vec3 should be of type FLOAT and have 3 components
    std::size_t num_components;

    /// @brief This vertex attribute's type.
    Type type;

    /// @brief Specifies whether fixed-point data values should be normalized 
    /// or converted directly as fixed-point values when they are accessed
    bool normalized = false;
};

/// @brief This class is used to store vertices for drawing on the GPU
class VertexArrayObject
{
public:
    /// @brief Creates a VAO on the GPU
    /// @param vertex_attribs This VAO's vertex attributes
    /// @param name This VAO's name
    /// @param create_buffers Specifies if the VAO should create the VBO and EBO buffers, 
    /// this will also link them. True by default
    VertexArrayObject(
        const std::vector<VertexAttribute> &vertex_attribs,
        std::string_view name,
        bool create_buffers = true
    );

    /// @brief Uploads vertices to this VAO
    /// @param vertices The type erased vertices
    /// @param size The size in bytes of the vertices
    /// @param vertex_size The size of one vertex
    /// @warning This should match vertex attributes provided in the constructor.
    /// Failure to do so will result in weird crashes or undefined behaviour.
    auto upload_vertices(void *vertices, std::size_t size, std::size_t vertex_size) -> void;

    /// @brief Uploads indices linked to this VAO
    /// @param indices The indices
    auto upload_indices(const std::vector<std::uint32_t> &indices) -> void;

    /// @brief Links a vertex buffer object to this VAO.
    /// Use this if you want precise memory control over your vertex buffer.
    /// The VBO should be relinked when it gets resized.
    /// @param vbo_buf The vertex buffer
    /// @param vertex_size The size of one vertex
    /// @param first_el_off The offset of the first element of the buffer. Default 0
    auto link_vbo(const GPUBuffer &vbo_bu, std::size_t vertex_size, std::size_t first_el_off = 0) -> void;

    /// @brief Links a element buffer object (indices) to this VAO.
    /// Use this if you want precise memory control over your element buffer.
    /// The EBO should be relinked when it gets resized.
    /// @param ebo_buf The element buffer
    auto link_ebo(const GPUBuffer &ebo_buf) -> void;

    /// @brief Deletes the object from the GPU. 
    /// This VAO shouldn't be used after calling this function.
    /// Also deletes linked buffers.
    auto delete_gpu() -> void;

    /// @return The VAO's OpenGL handle  
    auto vao_gl_handle() -> std::uint32_t;

    /// @return The linked EBO's OpenGL handle
    auto ebo_gl_handle() -> std::uint32_t;

    /// @return The linked VBO's OpenGL handle
    auto vbo_gl_handle() -> std::uint32_t;

private:
    std::uint32_t vao_;

    GPUBuffer ebo_;
    GPUBuffer vbo_;
};

} // namespace NoctisEngine::Rendering
