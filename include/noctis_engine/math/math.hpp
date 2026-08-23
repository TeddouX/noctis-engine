#pragma once 
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "../core/format.hpp"


namespace glm
{
    
inline auto to_string(glm::mat4 mat) -> std::string 
{
    return std::format(
        "mat4:\n[{}, {}, {}, {}]\n[{}, {}, {}, {}]\n[{}, {}, {}, {}]\n[{}, {}, {}, {}]",
        mat[0][0], mat[1][0], mat[2][0], mat[3][0],
        mat[0][1], mat[1][1], mat[2][1], mat[3][1],
        mat[0][2], mat[1][2], mat[2][2], mat[3][2],
        mat[0][3], mat[1][3], mat[2][3], mat[3][3]
    );
}

inline auto to_string(glm::vec3 vec) -> std::string 
{
    return std::format(
        "vec3({}, {}, {})",
        vec[0], vec[1], vec[2]
    );
}

inline auto to_string(glm::quat quat) -> std::string 
{
    return std::format(
        "quat({}, {}, {}, {})",
        quat[0], quat[1], quat[2], quat[3]
    );
}

} // namespace glm
