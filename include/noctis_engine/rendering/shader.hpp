#pragma once
#include <variant>
#include <string>
#include <cstdint>

#include "../noctis_engine.hpp"


namespace NoctisEngine::Rendering
{
    
enum class UniformType { BOOL, INT, FLOAT };

struct UniformInfo 
{
    UniformType type;
    std::string name;
    std::variant<bool, int, float> val;
};

class Shader 
{
public:
    Shader(const std::string &code, const std::string &name);
    ~Shader() = default;

    auto compile() -> void;
    auto bind() -> void;
    auto set_uniform(const UniformInfo &info) const -> bool;

private:
    std::uint32_t programID_;
    std::uint32_t vert_shader_, frag_shader_;
};

} // namespace NoctisEngine::Rendering