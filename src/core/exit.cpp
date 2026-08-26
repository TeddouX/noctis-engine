#include <noctis_engine/core/exit.hpp>

#include <noctis_engine/core/logging.hpp>


namespace NoctisEngine::Core
{
    
auto exit_program_failure(std::string_view reason) -> void
{
    CORE_LOGGER.critical("Exit program. Reason: {}", reason);
    std::exit(EXIT_FAILURE);
}

auto exit_program_success() -> void
{
    CORE_LOGGER.info("Exit program.");
    std::exit(EXIT_SUCCESS);
}

} // namespace NoctisEngine::Core
