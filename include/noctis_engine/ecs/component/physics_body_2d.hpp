#pragma once
#include <cstdint>

#include "../../math/math.hpp"


namespace NoctisEngine::ECS
{

/// @brief This component should be used on an entity that is affected by physics,
/// it doesn't necessarily mean that the entity will be moved.
/// It also modifies the Transform2D component.
struct PhysicsBody2D
{
private:
    friend class PhysicsSystem2D;

    std::uint64_t               body_id;

public:
    /// @brief This is a runtime collision shape, it can be deleted, modified etc...
    /// Do not confuse with NoctisEngine::ECS::CollisionShape2D
    struct CollisionShape
    {
    private:
        friend class PhysicsSystem2D;
        bool is_chain = false;
        std::uint64_t shape_id;
    
    public:
        
    };

    std::vector<CollisionShape> collision_shapes;

    /// @brief Enum used to describe the type of a physics body
    enum class Type
    {
        /// @brief Zero mass, zero velocity, may be manually moved
        STATIC = 0,
        /// @brief Zero mass, velocity set by user, moved by solver
        KINEMATIC = 1,
        /// @brief Positive mass, velocity determined by forces, moved by solver
        DYNAMIC = 2
    };

    /// @brief Structure used to lock the motion of a physics body
    struct MotionLocks
    {
        /// @brief Locks motion on the x axis
        bool x;
        /// @brief Locks motion on the y axis
        bool y;
        /// @brief Locks rotation
        bool rotation;
    };

    /// @brief Gets the physics body type
    /// @return The physics body type
    auto phys_body_type() -> Type;

    /// @brief Sets the physics body type
    /// @param type The physics body type
    auto set_phys_body_type(Type type) -> void;

    /// @brief Gets the linear velocity of the physics body
    /// @return The linear velocity
    auto linear_velocity() -> glm::vec2;

    /// @brief Gets the angular velocity of the physics body
    /// @return The angular velocity
    auto angular_velocity() -> float;

    /// @brief Sets the linear velocity of the physics body
    /// @param vel The linear velocity
    auto set_linear_velocity(const glm::vec2 &vel) -> void;

    /// @brief Sets the angular velocity of the physics body
    /// @param vel The angular velocity
    auto set_angular_velocity(float vel) -> void;

    /// @brief Applies a force to the physics body. If the force is not applied at the center of mass, 
    /// it will generate a torque and affect the angular velocity.
    /// @param force The world force vector, usually in newtons (N)
    /// @param point The world position of the point of application
    /// @param wake Option to wake up the body
    auto apply_force(const glm::vec2 &force, const glm::vec2 &point, bool wake = true) -> void;

    /// @brief Apply a force to the center of mass.
    /// @param force the world force vector, usually in newtons (N)
    /// @param wake Option to wake up the body
    auto apply_force_to_center(const glm::vec2 &force, bool wake = true) -> void;

    /// @brief Apply a torque. This affects the angular velocity without affecting the linear velocity. 
    /// @param torque about the z-axis (out of the screen), usually in N*m.
    /// @param wake Option to wake up the body
    auto apply_torque(float torque, bool wake = true) -> void;

    /// @brief Apply an impulse at a point. This immediately modifies the velocity. 
    /// It also modifies the angular velocity if the point of application is not at the center of mass.
    /// @param impulse The world impulse vector, usually in N*s or kg*m/s.
    /// @param point The world position of the point of application.
    /// @param wake Option to wake up the body
    /// @warning This should be used for one-shot impulses. If you need a steady force,
    /// use a force instead.
    auto apply_linear_impulse(const glm::vec2 &impulse, const glm::vec2 &point, bool wake = true) -> void;

    /// @brief Apply an impulse to the center of mass. This immediately modifies the velocity. 
    /// @param impulse The world impulse vector, usually in N*s or kg*m/s.
    /// @param wake Option to wake up the body
    /// @warning This should be used for one-shot impulses. If you need a steady force,
    /// use a force instead.
    auto apply_linear_impulse_to_center(const glm::vec2 &impulse, bool wake = true) -> void;

    /// Apply an angular impulse.
    /// @param impulse The angular impulse, usually in units of kg*m*m/s
    /// @param wake Option to wake up the body
    /// @warning This should be used for one-shot impulses. If you need a steady torque,
    /// use a torque instead.
    auto apply_angular_impulse(float impulse, bool wake = true) -> void;

    /// @brief Get the rotational inertia of the body.
    /// @return The rotation inertia
    auto rotational_inertia() -> float;

    /// @brief Sets the linear damping
    /// @param val The linear damping
    auto set_linear_damping(float val) -> void;

    /// @brief Gets the linear damping
    /// @return The linear damping
    auto linear_damping() -> float;

    /// @brief Sets the angular damping
    /// @param val The angular damping
    auto set_angular_damping(float val) -> void;

    /// @brief Gets the angular damping
    /// @return The angular damping
    auto angular_damping() -> float;
    
    /// @brief Sets the gravity scale
    /// @param val The gravity scale
    auto set_gravity_scale(float val) -> void;

    /// @brief Gets the gravity scale
    /// @return The gravity scale
    auto gravity_scale() -> float;

    /// @return `true` if the body is awake 
    auto awake() -> bool;

    /// @brief Wakes or puts a body to sleep
    /// @param awake `true` if the body should be awaken else `false`
    /// @warning Putting a body to sleep will put the entire island of bodies touching this body to sleep,
    auto set_awake(bool awake) -> void;

    /// @brief Wakes touching bodies
    auto wake_touching() -> void;

    /// @brief Enables or disables sleeping for this physics body
    /// @param enable `true` if sleep should be enabled else `false`
    auto enable_sleeping(bool enable) -> void;

    /// @return `true` if sleep is enabled for this physics body 
    auto sleep_enabled() -> bool;

    /// @brief Sets the sleep threshold for this physics body
    /// @param threshold The sleep threshold, usually in meters per second
    auto set_sleep_threshold(float threshold) -> void;

    /// Gets the sleep threshold for this physics body
    /// @return The sleep threshold, usually in meters per second
    auto sleep_threshold() -> float;

    /// @return `true` if this physics body is enabled else `false`
    auto enabled() -> bool;

    /// @brief Enables this physics body by adding it from the simulation
    /// @warning This is expensive
    auto enable() -> void;

    /// @brief Disables this physics body by removing it from the simulation
    /// @warning This is expensive
    auto disable() -> void;

    /// @brief Sets the motion locks on this physics body
    /// @param locks The motion locks
    auto set_motion_locks(const MotionLocks &locks) -> void;
    
    /// @brief Gets the motion locks on this physics body
    /// @return The motion locks
    auto motion_locks() -> MotionLocks;

    /// @return This physics body's mass in kg 
    auto mass() -> float;
};

} // namespace NoctisEngine::ECS
