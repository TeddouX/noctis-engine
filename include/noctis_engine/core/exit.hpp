#pragma once
#include <string>


namespace NoctisEngine::Core
{
    
auto exit_program_failure(std::string_view reason = "Unrecoverable error") -> void;
auto exit_program_success() -> void;

} // namespace NoctisEngine::Core
