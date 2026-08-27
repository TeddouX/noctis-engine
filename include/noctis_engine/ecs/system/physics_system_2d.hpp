#pragma once
#include <memory>
#include <vector>

#include "collision_shape_2d.hpp"
#include "../ecs_world.hpp"
#include "../component/transform_2d.hpp"


namespace NoctisEngine::ECS
{

/// @brief You should use this system if you want physics in your game
class PhysicsSystem2D
{
public:
    /// @brief Default amount of time to simulate for physics update
    static constexpr float DEFAULT_TIMESTEP = 1.0f / 60.0f;

    /// @param world Your game's ECS world, used to create physics entities
    PhysicsSystem2D(std::shared_ptr<World> world);
    ~PhysicsSystem2D() = default;

    /// @brief Creates a physics entity
    /// @param collision_shapes The physics entity's collision shapes 
    /// @param transform The physics entity's default transform 
    /// @return The created physics entity.
    /// The created entity is invalid if something went wrong during creation
    auto create_physics_entity(
        const std::vector<CollisionShape2D> &collision_shapes, 
        const ECS::Transform2D &transform = ECS::Transform2D{}
    ) -> Entity;

    /// @brief Updates the physics engine's transforms
    /// @important Call this before any calls to physics_step()
    auto sync_physics_engine_to_ecs() -> void;
    
    /// @brief Updates the physics engine
    /// @param time_step The amount of time to simulate, this should be a fixed number 
    /// for better stability. Default 1/60
    /// @param substep_count The number of sub-steps, increasing this number may 
    /// increase accuracy, but at the cost of performance.
    /// @important sync_physics_engine_to_ecs() should be called before 
    /// calling this function
    auto update_physics(float time_step = DEFAULT_TIMESTEP, std::uint16_t substep_count = 4) -> void;

    /// @brief Updates the transforms according to what 
    /// the physics engine calculated.
    /// This should be called after 
    auto sync_ecs_to_physics_engine() -> void;

private:
    std::shared_ptr<World>  world_;
    std::vector<Entity>     physics_entities_;
  
    auto process_contact_events() -> void;
    auto process_sensor_events() -> void;
};

} // namespace NoctisEngine::ECS
