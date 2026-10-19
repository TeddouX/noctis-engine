#pragma once
#include <memory>
#include <vector>

#include "../math/vec2.hpp"
#include "rect.hpp"
#include "ui_event.hpp"


namespace NoctisEngine
{
    
enum class AnchorType
{
    ANCHORED,
    FILL,
};

class IWidget
{
public:
    virtual ~IWidget() = default;

    virtual auto draw() -> void {};
    virtual auto update_layout(const Rect &available_space) -> void {}
    virtual auto on_event(const UIEvent &event) -> bool { return false; }
    
    Rect                                    bounds;

    /// @brief The anchor position in SSC [0, 1]
    Vec2                                    anchor_pos;
    AnchorType                              anchor_type;
    
    std::vector<std::shared_ptr<IWidget>>   children;
    IWidget                                *parent = nullptr;
    
    int                                     draw_order = 0;
    bool                                    visible = true;
    bool                                    dirty = false;

protected:
};

} // namespace NoctisEngine
