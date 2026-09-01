#pragma once
#include <string>


/// @brief Namespace that contains all the things that are useful to the engine
namespace NoctisEngine
{
    
/// @brief Call this when your program failed with something that is unrecoverable
/// @param reason The reason why your program quit. Default "Unrecoverable error"
/// @warning This calls std::exit(EXIT_FAILURE), so anything that should be done before quitting
/// should be done before calling this function
auto exit_program_failure(std::string_view reason = "Unrecoverable error") -> void;

/// @brief Call this function when your program finished without any errors
auto exit_program_success() -> void;

} // namespace NoctisEngine
