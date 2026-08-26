#pragma once
#include "../math/color.hpp"
#include "gpu_buffer.hpp"
#include "mesh/mesh_manager.hpp"


namespace NoctisEngine::Rendering
{
    
enum class BlendFunc 
{
    ZERO = 0, 
    ONE = 1,

    SRC_COLOR = 0x0300, 
    ONE_MINUS_SRC_COLOR = 0x0301, 

    SRC_ALPHA = 0x0302, 
    ONE_MINUS_SRC_ALPHA = 0x0303, 

    DST_ALPHA = 0x0304, 
    ONE_MINUS_DST_ALPHA = 0x0305, 

    DST_COLOR = 0x0306, 
    ONE_MINUS_DST_COLOR = 0x0307,

    CONSTANT_COLOR = 0x8001,
    ONE_MINUS_CONSTANT_COLOR = 0x8002,

    CONSTANT_ALPHA = 0x8003,
    ONE_MINUS_CONSTANT_ALPHA = 0x8004
};

class Renderer
{
public:
    Renderer();

    auto set_backface_culling(bool b) const -> void;
    auto set_depth_testing(bool b) const -> void;
    auto set_throw_on_err(bool b) -> void;
    auto set_blend(bool b) const -> void;
    auto set_blend_func(BlendFunc sFactor, BlendFunc dFactor) const -> void;

    auto render(DrawList &draw_list) -> void;

private:
    GPUBuffer                       objectsSSBO_;
    GPUBuffer                       command_buffer_;

    bool                            throw_on_err_;

    auto check_ogl_extensions() -> void;

    static void opengl_debug_message_callback(uint32_t source, uint32_t type, uint32_t id, uint32_t severity,
        int length, const char* message, const void* userParam);
};

} // namespace NoctisEngine::Rendering
