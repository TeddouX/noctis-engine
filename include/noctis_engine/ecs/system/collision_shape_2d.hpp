#pragma once
#include <cstdint>
#include <variant>

#include "../entity.hpp"
#include "../ecs_world.hpp"
#include "../component/physics_body_2d.hpp"
#include "../../math/color.hpp"


namespace NoctisEngine
{

/// @brief Describes the physical properties of a collision shape
struct PhysicsMaterial2D
{
    /// @brief A custom debug draw color
    Color debug_draw_color{0, 255, 0};

    /// @brief The friction coefficient. In the range [0, 1]
    float friction{0.6f};
    
    /// @brief The bounciness. In the range [0, 1]
    float bounciness{0.f};
    
    /// @brief The rolling resistance. In the range [0, 1]
    float rolling_resistance{0.f};

    /// @brief The tangent speed.
    float tangent_speed{0.f};
};

/// @brief Describes the contact points between colliding shapes
struct CollisionManifold2D
{
    /// @brief The unit normal vector in world space, 
    /// points from this shape to the other body.
    glm::vec2 normal;

    /// @brief Describes a manifold point
    struct Point
    {
        /// @brief Location of the contact point relative to shape A's origin in world space.
        glm::vec2 anchor_a;

        /// @brief Location of the contact point relative to shape B's origin in world space.
        glm::vec2 anchor_b;

        /// @brief The impulse along the manifold normal vector.
        float normal_impulse;

        /// @brief Relative normal velocity pre-solve.
        /// Used for hit events. If the normal impulse is zero then there was no hit. Negative means shapes are approaching.
        float normal_velocity;
        
        /// @brief Did this contact point exist the previous step?
        bool persisted;

        /// @brief Location of the contact point in world space. Subject to precision loss at large coordinates.
        glm::vec2 point;

        /// @brief The separation of the contact point, negative if penetrating.
        float separation;

        /// @brief The friction impulse.
        float tangent_impulse;

        /// @brief The total normal impulse applied across sub-stepping and restitution.
        /// This is important to identify speculative contact points that had an interaction in the time step.
        float tangent_normal_impulse;
    };

    /// @brief The manifold points, up to two are possible in 2D.
    std::array<Point, 2> points;

    /// @brief Angular impulse applied for rolling resistance. N*m*s = kg*m^2/s.
    float rolling_impulse;
};

/// @brief Used to create a physics object
struct CollisionShape2D
{
    /// @brief Describes a box collision collision shape
    struct Box
    {
        /// @brief The size of the box, in pixels
        glm::vec2 half_extents;

        /// @brief The local center of the box, in pixels
        glm::vec2 center{0};

        /// @brief The local rotation of the box
        float rotation_deg{0};
        
        /// @brief The radius of the rounded corners, in pixels
        float corner_radius{0};
    };

    /// @brief Describes a circle collision shape
    struct Circle
    {
        /// @brief The radius of the circle, in pixels
        float radius;

        /// @brief The local center of the circle, in pixels
        glm::vec2 center{0};
    };

    /// @brief Describes a capsule collision shape
    struct Capsule
    {
        /// @brief The local center of the first semicircle, in pixels
        glm::vec2 center_1;
        
        /// @brief The local center of the second semicircle, in pixels
        glm::vec2 center_2;

        /// @brief The radius of the semicircles, in pixels
        float radius;
    };

    /// @brief Describes a line segment with two sided collision
    struct Segment
    {
        /// @brief The first point, in pixels
        glm::vec2 point_1;

        /// @brief The second point, in pixels
        glm::vec2 point_2;
    };

    /// @brief Describes a convex poligon shape
    struct Polygon
    {
        /// - Points should be a CCW (counter-clockwise) order
        /// - Points must be decently far away from each other.
        /// - Points mustn't be on the same line.
        /// - There should be more than 2 points.
        /// - Points are should use pixels as units
        std::vector<glm::vec2> points;

        float corner_radius{0.0f};
    };

    /// This is designed to eliminate ghost collisions with some limitations:
    /// - Chains are one-sided
    /// - Chains have no mass and should be used on static bodies
    /// - Chains are either a loop or open
    /// - A chain must have at least 4 points
    /// - A chain shape should not self intersect (this is not validated)
    /// - An open chain shape has NO COLLISION on the first and final edge
    /// - You may overlap two open chains on their first three and/or 
    /// last three points to get smooth collision
    struct Chain
    {
        /// - Points should be a CCW (counter-clockwise) order
        /// - Points are should use pixels as units
        std::vector<glm::vec2> points;
        
        /// @brief true if the chain's points form a loop
        bool is_loop;
        
        /// @brief Either 0 physics materials or the number of points. If 0, the CollisionShape2D's 
        /// physics material will be used
        std::vector<PhysicsMaterial2D> physics_materials;

        /// @brief Enable sensors to detect this chain. False by default.
        bool enable_sensor_events = false;
    };

    enum class ShapeType
    {
        BOX,
        CIRCLE,
        CAPSULE,
        SEGMENT,
        POLYGON,
        CHAIN,
    };

    using Shapes = std::variant<
        Box,
        Circle,
        Capsule,
        Segment,
        Polygon,
        Chain
    >;

    /// @brief This collision shape's shape type
    ShapeType shape_type;

    /// @brief This collision shape's shape, one among: Box, Circle, Capsule, Segment, Poligon and Chain.
    /// Must match the shape_type
    Shapes shape;

    /// @brief Describes all the callbacks that relate to collision
    struct Callbacks
    {
        /// @brief Called when a collision begins and if enable_collision_events is set to true
        /// @param 1 The world to which the entities that collided belong to
        /// @param 2 entity A (the one bearing this collision shape)
        /// @param 3 entity B (the one that collided)
        std::function<void (Entity, Entity)> on_collision_begin{nullptr};

        /// @brief Called when a collision ends and if enable_collision_events is set to true
        /// @param 1 The world to which the entities that collided belong to
        /// @param 2 entity A (the one bearing this collision shape)
        /// @param 3 entity B (the one that collided)
        std::function<void (Entity, Entity)> on_collision_end{nullptr};
        
        /// @brief Called when a hit collision is triggered.
        /// The hit event threshold can be set in the PhysicsSystem2D.
        /// @param 1 The world to which the entities that collided belong to
        /// @param 2 entity A (the one bearing this collision shape)
        /// @param 3 entity B (the one that collided)
        /// @param 4 Normal vector pointing from shape A to shape B. 
        /// @param 5 Point where the shapes hit.
        /// @param 6 The speed the shapes are approaching. Always positive. Typically in m/s. 
        std::function<void (Entity, Entity, glm::vec2, glm::vec2, float)> on_hit{nullptr};
        
        /// @brief Called when a collision begins and is_sensor is set to true
        /// @param 1 The world to which the entities that collided belong to
        /// @param 2 entity A (the sensor)
        /// @param 3 entity B (the visitor)
        std::function<void (Entity, Entity)> on_sensor_begin_touch{nullptr};

        /// @brief Called when a collision ends and is_sensor is set to true
        /// @param 1 The world to which the entities that collided belong to
        /// @param 2 entity A (the sensor)
        /// @param 3 entity B (the visitor)
        std::function<void (Entity, Entity)> on_sensor_end_touch{nullptr};
    };

    /// @brief The density of this collision shape, in kg/m^2
    float density{1.0f};

    /// @brief This collision shape's physics material
    PhysicsMaterial2D physics_material;

    /// @brief This represents the collision shape's category
    std::uint64_t type_bit = 0x1;

    static constexpr std::uint64_t ALL_CATEGORIES = std::numeric_limits<std::uint64_t>::max();

    /// @brief This states the categories that this shape would accept for collision.
    std::uint64_t collision_mask_bits = ALL_CATEGORIES;

    /// @brief Enables collision events for this collision shape
    bool enable_collision_events = false;

    /// @brief Enables hit events for this collision shape.
    /// The hit event threshold can be set in the PhysicsSystem2D
    bool enable_hit_events = false;

    /// @brief A sensor shape generates overlap events but never generates a collision response.
    /// Sensors do not have continuous collision. Instead, use a ray or shape cast for those scenarios. 
    /// Sensors still contribute to the body mass if they have non-zero density.
    bool is_sensor = false;

    /// @brief Enables this shape to generate sensor events when colliding with sensors
    bool enable_sensor_events = false;

    /// @brief This collision shape's callbacks
    Callbacks callbacks;
};

} // namespace NoctisEngine
