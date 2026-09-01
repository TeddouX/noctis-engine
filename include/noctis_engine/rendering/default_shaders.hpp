#pragma once
#include <string_view>


/// @brief Default shader code
namespace NoctisEngine::DefaultShaders
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

/// @brief Default vertex composition program used by the renderer
inline std::string_view COMPOSITION_VERT_SHADER = R"(
layout (location = 0) in vec3 aPos;
layout (location = 3) in vec2 aTexCoord;

layout (location = 0) out vec3 fsTexCoord;

void main()
{
    gl_Position = vec4(aPos.xy, 0.0, 1.0);
    fsTexCoord = aTexCoord;
}
)";


/// @brief Default fragment shader for drawing 2D debug elements
inline std::string_view COMPOSITION_FRAG_SHADER = R"(
layout (location = 0) in vec3 fsTexCoord;

layout (location = 0) out vec4 FragColor;

layout (location = 0) uniform sampler2D world_fb_color_tex;
layout (location = 1) uniform sampler2D ui_fb_color_tex;

void main()
{
    vec4 world_color = texture(world_fb_color_tex, fsTexCoord);
    vec4 ui_color    = texture(ui_fb_color_tex, fsTexCoord);

    vec3 final_color = mix(world_color.rgb, ui_color.rgb, ui_color.a);
    FragColor = vec4(final_color, 1.0);
}
)";

} // namespace NoctisEngine::DefaultShaders
