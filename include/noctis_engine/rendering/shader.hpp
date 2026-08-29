#pragma once
#include <string>
#include <cstdint>

#include "../noctis_engine.hpp"
#include "draw_list.hpp"


namespace NoctisEngine::Rendering
{

class Shader 
{
public:
    Shader() = default;
    Shader(const char *code, const std::string &name);

    ~Shader() = default;

    auto compile() -> bool;
    auto use(DrawList &draw_list) -> void;

    auto gl_handle() -> std::uint32_t;

private:
    std::uint32_t programID_;
    std::uint32_t vert_shader_, frag_shader_;
};

} // namespace NoctisEngine::Rendering