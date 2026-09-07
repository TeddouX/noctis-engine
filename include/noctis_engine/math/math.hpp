#pragma once 
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/vector_angle.hpp>


namespace NoctisEngine
{
    
class Vector2
{
public:
    static inline constexpr glm::vec2 UP{0, 1};
    static inline constexpr glm::vec2 DOWN{0, -1};
    static inline constexpr glm::vec2 LEFT{-1, 0};
    static inline constexpr glm::vec2 RIGHT{1, 0};

private:
};

} // namespace NoctisEngine

