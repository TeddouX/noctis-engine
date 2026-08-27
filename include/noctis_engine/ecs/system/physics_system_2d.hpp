#pragma once
#include <memory>

#include "collision_shape_2d.hpp"
#include "../ecs_world.hpp"
#include "../component/transform_2d.hpp"


namespace NoctisEngine::ECS
{

class PhysicsSystem2D
{
public:
    PhysicsSystem2D(std::shared_ptr<World> world);
    ~PhysicsSystem2D() = default;

    auto create_physics_entity(
        const std::vector<CollisionShape2D> &collision_shapes, 
        const ECS::Transform2D &transform = ECS::Transform2D{}
    ) -> Entity;

private:
    std::shared_ptr<World> world_;
};

} // namespace NoctisEngine::ECS
