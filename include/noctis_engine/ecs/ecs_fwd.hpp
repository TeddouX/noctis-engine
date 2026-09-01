#pragma once
#include <functional>


namespace NoctisEngine
{
    
class Entity;
class ECSWorld;

/// @brief Describes a non owning view of a world
/// @tparam ...Components_ The components this view gets from the world
template <typename... Components_>
class View
{
public:
    /// @brief Iterates over this view's components
    /// @param func The callback function that should be called for 
    /// each component in this view 
    auto each(std::function<void(Entity, Components_ &...)> func) -> void;

private:
    ECSWorld &world_;

    friend class ECSWorld;

    /// @brief Creates a view from a world
    /// @param world The world
    explicit View(ECSWorld &world)
        : world_{world}
    {}
};

} // namespace NoctisEngine
