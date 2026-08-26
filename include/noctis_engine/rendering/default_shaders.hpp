#pragma once
#include <string_view>

namespace NoctisEngine::Rendering::Shaders
{
    
constexpr std::string_view DEFAULT_SHADER_2D = 
R"(
#ifdef VERTEX

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

#endif



#ifdef FRAGMENT

layout (location = 0) in vec2 fsTexCoord;

layout (location = 0) uniform sampler2D albedo;

layout (location = 0) out vec4 FragColor;

void main()
{
    FragColor = texture(albedo, fsTexCoord);
}

#endif
)";

} // namespace NoctisEngine::Rendering::Shaders
