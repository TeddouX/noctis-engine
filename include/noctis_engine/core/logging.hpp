#pragma once
#include <string>
#include <format>
#include <chrono>
#include <print>
#include <iostream>
#include <source_location>
#include <filesystem>


namespace NoctisEngine
{
    
namespace Core
{
    
/// @brief Describes which level a log message is
enum class LogLevel 
{
    /// @brief Used for debugging
    DEBUG,

    /// @brief Used for information
    INFO,

    /// @brief Used for warning about something
    WARN,

    /// @brief Used for an error
    ERROR,

    /// @brief Used when an unrecoverable error was encountered
    CRITICAL,
};

inline auto to_string(LogLevel level) -> std::string 
{
    switch (level) {
        using enum LogLevel;
        case DEBUG:     return "DEBUG"; 
        case INFO:      return "INFO"; 
        case WARN:      return "WARN"; 
        case ERROR:     return "ERROR"; 
        case CRITICAL:  return "CRITICAL";
        default:        return "?";
    }
}

/// @brief A helper class to print messages to the console
class Logger 
{
public:
    /// @brief Creates a logger, the directory and sub directories are used to better 
    /// identify where messages come from
    /// @param directory The directory the logger is placed in. This should be your project's name
    /// @param sub_directory The sub-directory the logger is placed in. This should be one 
    /// of your project's components, like rendering, level loading, etc...
    Logger(const std::string &directory, const std::string &sub_directory)
        : directory_{directory}
        , sub_directory_{sub_directory}
    {}

    /// @brief Used for printing debug messages
    /// @tparam ...Args_ The format arguments types
    /// @param fmt The format string
    /// @param ...args The format arguments
    template <typename ...Args_>
    auto debug(std::format_string<Args_...> fmt, Args_ &&...args) const -> void;

    /// @brief Used for printing messages that give information about what is happening
    /// @tparam ...Args_ The format arguments types
    /// @param fmt The format string
    /// @param ...args The format arguments
    template <typename ...Args_>
    auto info(std::format_string<Args_...> fmt, Args_ &&...args) const -> void;

    /// @brief Used for printing messages that warn about something that is not too serious
    /// @tparam ...Args_ The format arguments types
    /// @param fmt The format string
    /// @param ...args The format arguments
    template <typename ...Args_>
    auto warn(std::format_string<Args_...> fmt, Args_ &&...args) const -> void;

    /// @brief Used for printing error messages
    /// @tparam ...Args_ The format arguments types
    /// @param fmt The format string
    /// @param ...args The format arguments
    template <typename ...Args_>
    auto error(
        std::format_string<Args_...> fmt, 
        Args_ &&...args
    ) const -> void;

    /// @brief Used for printing unrecoverable error messages
    /// @tparam ...Args_ The format arguments types
    /// @param fmt The format string
    /// @param ...args The format arguments
    template <typename ...Args_>
    auto critical(
        std::format_string<Args_...> fmt, 
        Args_ &&...args
    ) const -> void;

private:
    std::string directory_; 
    std::string sub_directory_;

    template <typename... Args_>
    auto log(
        LogLevel level, 
        std::format_string<Args_...> fmt, 
        Args_ &&...args
    ) const -> void;

    auto get_time_string() const -> std::string 
    {
        using namespace std::chrono;

        system_clock::time_point now = system_clock::now();
        // Time zone offset
        seconds offset = current_zone()->get_info(now).offset;
        duration sys_time = now.time_since_epoch() + offset;

        int64_t s = duration_cast<seconds>(sys_time).count() % 60;
        int64_t m = duration_cast<minutes>(sys_time).count() % 60;
        int64_t h = duration_cast<hours>(sys_time).count()   % 24;

        return std::format("{:02}:{:02}:{:02}", h, m, s);
    }
};

template <typename ...Args_>
auto Logger::debug(std::format_string<Args_...> fmt, Args_ &&...args) const -> void 
{
#ifdef NCENG_DEBUG
    this->log(LogLevel::DEBUG, fmt, std::forward<Args_>(args)...);
#endif
}

template <typename ...Args_>
auto Logger::info(std::format_string<Args_...> fmt, Args_ &&...args) const -> void 
{
    this->log(LogLevel::INFO, fmt, std::forward<Args_>(args)...);
}

template <typename ...Args_>
auto Logger::warn(std::format_string<Args_...> fmt, Args_ &&...args) const -> void 
{
    this->log(LogLevel::WARN, fmt, std::forward<Args_>(args)...);
}

template <typename ...Args_>
auto Logger::error(
    std::format_string<Args_...> fmt, 
    Args_ &&...args
) const -> void 
{
    this->log(LogLevel::ERROR, fmt, std::forward<Args_>(args)...);
}

template <typename ...Args_>
auto Logger::critical(
    std::format_string<Args_...> fmt,
    Args_ &&...args
) const -> void 
{
    this->log(LogLevel::CRITICAL, fmt, std::forward<Args_>(args)...);
}

template <typename... Args_>
auto Logger::log(
    LogLevel level, 
    std::format_string<Args_...> fmt,
    Args_ &&...args
) const -> void 
{
    std::string timeStr = get_time_string();
    std::string formattedMsg = std::format(fmt, std::forward<Args_>(args)...);
    std::ostream &stream = (level >= LogLevel::ERROR) ? std::cerr : std::cout;

    std::println(stream, "[{}] [{}] ({}/{}): {}", 
        timeStr, 
        to_string(level), 
        directory_, sub_directory_, 
        formattedMsg
    );
}

inline static Logger CORE_LOGGER{"Noctis Engine", "Core"};

} // namespace Core

namespace Rendering 
{ 

inline static Core::Logger RENDERING_LOGGER{"Noctis Engine", "Rendering"}; 

} // namespace Rendering

} // namespace NoctisEngine

