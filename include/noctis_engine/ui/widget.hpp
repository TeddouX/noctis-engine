#pragma once
#include <memory>
#include <vector>

#include "rect.hpp"
#include "ui_event.hpp"


namespace NoctisEngine
{
    
class IWidget
{
public:
    virtual ~IWidget() = default;

    virtual auto draw() -> void {};
    virtual auto update_layout(const Rect &available_space) -> void {};
    virtual auto on_event(const UIEvent &event) -> bool {};

    Rect                                    bounds;
    std::vector<std::shared_ptr<IWidget>>   children;
    IWidget                                *parent = nullptr;
    int                                     draw_order = 0;
    bool                                    visible = true;
    bool                                    dirty = false;
};

} // namespace NoctisEngine
