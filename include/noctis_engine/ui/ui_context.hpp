#pragma once
#include "../rendering/texture.hpp"
#include "rect.hpp"


namespace NoctisEngine
{
    
class UIContext
{
public:
    UIContext() = default;
    ~UIContext() = default;

    /// @brief Enters a new context, call this function before drawing your root widget
    auto enter_ctx() -> void;
    
    /// @return True if the UIContext has a valid context 
    auto has_ctx() -> bool;

    /// @brief Resets the current context's draw list
    auto reset() -> void;

    /// @return True if the the current context's draw list was reset this frame
    auto is_reset() -> bool;

    /// @brief Exits the current context, call this function after drawing your root widget
    auto exit_ctx() -> void;

    auto draw_textured_rect(const Rect &rect, const Texture &texture) -> void;
    auto draw_colored_rect(const Rect &rect, const Color &color) -> void;

    /// @brief UNIMPLEMENTED
    auto draw_text(const glm::vec2 &pos, const std::string_view &text) -> void;

private:
    std::vector<DrawList>   draw_lists_;
    std::size_t             ctx_num_;
    bool                    has_ctx_;
};

} // namespace NoctisEngine
