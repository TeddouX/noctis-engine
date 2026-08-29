#pragma once
#include <string_view>


/// @brief Default shader code
namespace NoctisEngine::Rendering::DefaultShaders
{

/// @brief Default vertex shader for drawing 2D sprites
inline std::string_view VERT_SHADER_2D_CODE = R"(
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aTangent;
layout (location = 3) in vec2 aTexCoord;

layout (std140, binding = 0) uniform CameraBuffer {
    mat4 projMat;
    mat4 viewMat;
    vec3 pos;
} camera;


struct Object {
    mat4 modelMat;
};

layout (std430, binding = 1) buffer Objects {
    Object objects[];
} objects;


layout (location = 0) out vec2 fsTexCoord;

void main()
{
    uint objectIndex = gl_BaseInstance + gl_InstanceID;
    Object obj = objects.objects[objectIndex];

    gl_Position = camera.projMat * camera.viewMat * obj.modelMat * vec4(aPos, 1.0);
    // gl_Position = vec4(aPos, 1.0);
    fsTexCoord = aTexCoord;
}
)";


/// @brief Default fragment shader for drawing 2D sprites
inline std::string_view FRAG_SHADER_2D_CODE = R"(
layout (location = 0) in vec2 fsTexCoord;

layout (location = 0) uniform sampler2D albedo;

layout (location = 0) out vec4 FragColor;

void main()
{
    FragColor = texture(albedo, fsTexCoord);
}
)";


/// @brief Default vertex shader for drawing 2D debug elements
inline std::string_view DEBUG_VERT_SHADER_2D_CODE = R"(
layout (location = 0) in vec3 aWorldPos;
layout (location = 1) in vec3 aColor;

layout (std140, binding = 0) uniform CameraBuffer {
    mat4 projMat;
    mat4 viewMat;
    vec3 pos;
} camera;

layout (location = 0) out vec3 fsColor;

void main()
{
    gl_Position = camera.projMat * camera.viewMat * vec4(aWorldPos, 1.0);
    fsColor = aColor;
}
)";


/// @brief Default fragment shader for drawing 2D debug elements
inline std::string_view DEBUG_FRAG_SHADER_2D_CODE = R"(
layout (location = 0) in vec3 fsColor;

layout (location = 0) out vec4 FragColor;

void main()
{
    FragColor = vec4(fsColor, 0);
}
)";

} // namespace NoctisEngine::Rendering::DefaultShaders
