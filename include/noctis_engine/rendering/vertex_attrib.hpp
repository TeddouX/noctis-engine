#pragma once
#include <cstdint>


namespace NoctisEngine::Rendering
{
    
/// @brief This describes a vertex attribute, used when creating a vertex array object
struct VertexAttribute
{
    /// @brief This describes a vertex attribute's type
    enum class ComponentType
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

    /// @brief This vertex attribute's component type.
    ComponentType component_type;

    /// @brief This vertex attribute's number of components. Must be 1, 2, 3 or 4.
    /// This is useful for vectors, for example a glm::vec3 should be of type FLOAT and have 3 components
    std::size_t num_components;

    /// @brief Specifies whether fixed-point data values should be normalized 
    /// or converted directly as fixed-point values when they are accessed
    bool normalized = false;
};

} // namespace NoctisEngine::Rendering
