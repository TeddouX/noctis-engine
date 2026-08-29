#pragma once
#include <functional>


namespace NoctisEngine::ECS
{
    
class Entity;
class World;

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
    World &world_;

    friend class World;

    /// @brief Creates a view from a world
    /// @param world The world
    explicit View(World &world)
        : world_{world}
    {}
};

} // namespace NoctisEngine::ECS
