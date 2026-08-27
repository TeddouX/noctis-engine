#pragma once
#include <cstdint>

#include "../../math/color.hpp"


namespace NoctisEngine::ECS
{
    
/// @brief Describes the physical properties of a collision shape
struct PhysicsMaterial2D
{
    /// @brief A custom debug draw color
    Color debug_draw_color{0, 255, 0};

    /// @brief The friction coefficient. In the range [0, 1]
    float friction;
    
    /// @brief The bounciness. In the range [0, 1]
    float bounciness;
    
    /// @brief The rolling resistance. In the range [0, 1]
    float rolling_resistance;

    /// @brief The tangent speed.
    float tangent_speed;
};

/// @brief Used to create a physics object
struct CollisionShape2D
{
    /// @brief Describes a box collision collision shape
    struct Box
    {
        /// @brief The size of the box
        glm::vec2 half_extents;

        /// @brief The local center of the box
        glm::vec2 center{0};

        /// @brief The local rotation of the box
        float rotation_deg{0};
        
        /// @brief The radius of the rounded corners
        float rounded_radius{0};
    };

    /// @brief Describes a circle collision shape
    struct Circle
    {
        /// @brief The radius of the circle
        float radius;

        /// @brief The local center of the circle
        glm::vec2 center{0};
    };

    /// @brief Describes a capsule collision shape
    struct Capsule
    {
        /// @brief The local center of the first semicircle
        glm::vec2 center_1;
        
        /// @brief The local center of the second semicircle
        glm::vec2 center_2;

        /// @brief The radius of the semicircles
        float radius;
    };

    /// @brief Describes a line segment with two sided collision
    struct Segment
    {
        /// @brief The first point
        glm::vec2 point_1;
        /// @brief The second point
        glm::vec2 point_2;
    };

    /// @brief Describes a convex poligon shape
    struct Poligon
    {
        /// - Points should be a CCW (counter-clockwise) order
        /// - Points must be decently far away from each other.
        /// - Points mustn't be on the same line.
        /// - There should be more than 2 points.
        std::vector<glm::vec2> points;
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
        /// @brief Points should be a CCW (counter-clockwise) order
        std::vector<glm::vec2> points;
        /// @brief 
        /// @brief `true` if the chain's points form a loop
        bool is_loop;
        
        /// @brief Either 0 physics materials or the number of points. If 0, the CollisionShape2D's 
        /// physics material will be used
        std::vector<PhysicsMaterial2D> physics_materials;
    };

    enum class ShapeType
    {
        BOX,
        CIRCLE,
        CAPSULE,
        SEGMENT,
        POLIGON,
        CHAIN,
    };

    using Shapes = std::variant<
        Box,
        Circle,
        Capsule,
        Segment,
        Poligon,
        Chain,
    >;

    /// @brief This collision shape's shape type
    ShapeType shape_type;

    /// @brief This collision shape's shape, one among: Box, Circle, Capsule, Segment, Poligon and Chain.
    /// Must match the shape_type
    Shapes shape;

    /// @brief The density of this collision shape, in kg/m^2
    float density = 1.0f;

    /// @brief Enables contact events for this collision shape
    bool enable_contact_events = false;

    /// @brief Enables hit events for this collision shape.
    /// The hit event threshold can be set in the PhysicsSystem2D
    bool enable_hit_events = false;

    /// @brief A sensor shape generates overlap events but never generates a collision response.
    /// Sensors do not have continuous collision. Instead, use a ray or shape cast for those scenarios. 
    /// Sensors still contribute to the body mass if they have non-zero density.
    bool is_sensor = false;

    /// @brief This represents the collision shape's category
    std::uint64_t type_bit;

    /// @brief This states the categories that this shape would accept for collision.
    std::uint64_t collision_mask_bits;
};

} // namespace NoctisEngine::ECS
