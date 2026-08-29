#pragma once
#include <functional>


namespace NoctisEngine::ECS
{
    
class Entity;
class World;

template <typename... Components_>
class View
{
public:
    explicit View(World &world)
        : world_{world}
    {}

    auto each(std::function<void(Entity, Components_ &...)> func) -> void;

private:
    World &world_;
};

} // namespace NoctisEngine::ECS
