#pragma once
#include <memory>
#include <vector>
#include <deque>

#include "collision_shape_2d.hpp"
#include "../ecs_world.hpp"
#include "../component/transform_2d.hpp"
#include "../component/physics_body_2d.hpp"
#include "../component/identifier_component.hpp"
#include "../../rendering/draw_list.hpp"
#include "../../rendering/vertex_array.hpp"
#include "../../rendering/graphics_program.hpp"


namespace NoctisEngine::ECS
{

/// @brief Use this system if you want physics in your game
class PhysicsSystem2D
{
public:
    struct DebugDrawSettings
    {
        /// @brief World space coordinates (in pixels) that define the lower bound of 
        /// the area that should be drawn. Every thing outside this area will be culled (not drawn)
        /// It is recommended to use a margin of +100 pixels.
        glm::vec2 lower_draw_bound;

        /// @brief World space coordinates (in pixels) that define the upper bound of 
        /// the area that should be drawn. Every thing outside this area will be culled (not drawn)
        /// It is recommended to use a margin of +100 pixels.
        glm::vec2 upper_draw_bound;

        /// @brief Scale to use when drawing forces
        float force_scale;

        /// @brief Scale to use when drawing joints
        float joint_scale;

	    /// @brief Option to draw contact points
        bool draw_contacts : 1;

        /// @brief Draw anchor A for contact points (instead of anchor B)
        bool draw_anchor_A : 1;

        /// @brief Option to draw shapes
        bool draw_shapes : 1;

        /// @brief Option to draw chain normals
        bool draw_chain_normals : 1;

        /// @brief Option to draw joints
        bool draw_joints : 1;

        /// @brief Option to draw additional information for joints
        bool draw_joint_extras : 1;

        /// @brief Option to draw bounding boxes for shapes
        bool draw_bounds : 1;

        /// @brief Option to draw the mass and center of mass of dynamic bodies
        bool draw_mass : 1;

        /// @brief Option to draw contact normals
        bool draw_contact_normals : 1;

        /// @brief Option to draw contact forces
        bool draw_contact_forces : 1;

        /// @brief Option to draw friction forces
        bool draw_friction_forces : 1;

        /// @brief Option to draw islands as bounding boxes
        bool draw_islands : 1;
    };

    /// @brief Default amount of time to simulate for physics update
    static constexpr float DEFAULT_TIMESTEP = 1.0f / 60.0f;
    static constexpr glm::vec2 DEFAULT_GRAVITY{0, 9.81};

    /// @param world Your game's ECS world, used to create physics entities
    PhysicsSystem2D(std::shared_ptr<World> world);
    ~PhysicsSystem2D();

    /// @brief Enables debug rendering by creating required buffers.
    /// Do not call except if you want to draw debug shapes
    /// @return true on success, false otherwise
    /// @warning Default shaders should be compiled before calling this function
    auto enable_debug_rendering() -> bool;

    /// @brief Set the gravity vector for the physics world
    /// @param gravity The gravity vector
    auto set_gravity(glm::vec2 gravity) -> void;

    /// @brief This controls the collision speed needed to generate a hit even. Usually in m/s.
    /// @param threshold The speed necessary
    auto set_hit_event_threshold(float threshold) -> void;

    /// @brief Creates a physics entity
    /// @param collision_shapes The physics entity's collision shapes
    /// @param physics_body_type The physics body's type 
    /// @param transform The physics entity's default transform, in pixels
    /// @param name The physics entity's name, used for debugging 
    /// @return The created physics entity.
    /// The created entity is invalid if something went wrong during creation
    auto create_physics_entity(
        const std::vector<CollisionShape2D>    &collision_shapes,
        PhysicsBody2D::Type                     physics_body_type = PhysicsBody2D::Type::STATIC,
        const Transform2D                      &transform = Transform2D{},
        Identifier                              id
    ) -> Entity;

    /// @brief Updates the physics engine's transforms
    /// This should be called before calling to update_physics()
    auto sync_physics_engine_to_ecs() -> void;
    
    /// @brief Updates the physics engine
    /// @param dt The frame delta time
    /// @param time_step The amount of time to simulate, this should be a fixed number 
    /// for better stability. Default 1/60
    /// @param substep_count The number of sub-steps, increasing this number may 
    /// increase accuracy, but at the cost of performance.
    /// @warning sync_physics_engine_to_ecs() should be called before 
    /// calling this function to update position changes
    auto update_physics(float dt, float time_step = DEFAULT_TIMESTEP, std::uint16_t substep_count = 4) -> void;

    /// @brief Updates the transforms according to what 
    /// the physics engine calculated.
    /// Also processes contact and sensor events.
    /// This should be called after calling update_physics().
    auto sync_ecs_to_physics_engine() -> void;

    /// @brief Draws wireframes to the screen to debug colliders
    /// @param draw_list The draw list that should be used to draw the wireframes
    /// @param settings The draw settings
    /// @warning enable_debug_rendering() should be called before.
    /// Overrides previously bound VAOs and shaders, so be careful to when in your 
    /// rendering loop you call this function. 
    /// Default shaders must be compiled before calling this function
    auto draw_debug(Rendering::DrawList &draw_list, const DebugDrawSettings &settings) -> void;

private:
    std::shared_ptr<World>                      world_;
    std::vector<Entity>                         physics_entities_;
    std::uint32_t                               physics_world_;
    std::deque<CollisionShape2D::Callbacks>     collision_callbacks_;
    float                                       accumulator_;

    Rendering::VertexArray                      line_vertex_array_;
    Rendering::VertexArray                      tri_vertex_array_;
    Rendering::GraphicsProgram                  graphics_prog_;

  
    /// @brief This is called in sync_ecs_to_physics_engine().
    /// Calls callbacks in colliding collision shapes
    auto process_contact_events() -> void;

    /// @brief This is called in sync_ecs_to_physics_engine().
    /// Calls callbacks in colliding collision shapes
    auto process_sensor_events() -> void;
};

} // namespace NoctisEngine::ECS
