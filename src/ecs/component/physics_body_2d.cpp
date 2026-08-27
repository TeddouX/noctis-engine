#include <noctis_engine/ecs/component/physics_body_2d.hpp>

#include <box2d/box2d.h>


namespace NoctisEngine::ECS
{
    
auto PhysicsBody2D::rb_type() -> PhysicsBody2D::Type
{
    b2BodyType type = b2Body_GetType(b2LoadBodyId(body_id)); 
    return static_cast<PhysicsBody2D::Type>(type);
}

auto PhysicsBody2D::set_rb_type(PhysicsBody2D::Type type) -> void
{
    b2BodyType b2_type = static_cast<b2BodyType>(type); 
    b2Body_SetType(b2LoadBodyId(body_id), b2_type);
}

auto PhysicsBody2D::linear_velocity() -> glm::vec2
{
    b2Vec2 vec = b2Body_GetLinearVelocity(b2LoadBodyId(body_id));
    return glm::vec2{ vec.x, vec.y };
}

auto PhysicsBody2D::angular_velocity() -> float
{
    return b2Body_GetAngularVelocity(b2LoadBodyId(body_id));
}

auto PhysicsBody2D::set_linear_velocity(const glm::vec2 &vel) -> void
{
    b2Body_SetLinearVelocity(
        b2LoadBodyId(body_id), 
        b2Vec2{ vel.x, vel.y }
    );
}

auto PhysicsBody2D::set_angular_velocity(float vel) -> void
{
    b2Body_SetAngularVelocity(b2LoadBodyId(body_id), vel);
}

auto PhysicsBody2D::apply_force(const glm::vec2 &force, const glm::vec2 &point, bool wake = true) -> void
{
    b2Body_ApplyForce(
        b2LoadBodyId(body_id),
        b2Vec2{ force.x, force.y },
        b2Vec2{ point.x, point.y },
        wake
    );
}

auto PhysicsBody2D::apply_force_to_center(const glm::vec2 &force, bool wake = true) -> void
{
    b2Body_ApplyForceToCenter(
        b2LoadBodyId(body_id),
        b2Vec2{ force.x, force.y },
        wake
    );
}

auto PhysicsBody2D::apply_torque(float torque, bool wake = true) -> void
{
    b2Body_ApplyTorque(
        b2LoadBodyId(body_id),
        torque,
        wake
    );
}

auto PhysicsBody2D::apply_linear_impulse(const glm::vec2 &impulse, const glm::vec2 &point, bool wake = true) -> void
{
    b2Body_ApplyLinearImpulse(
        b2LoadBodyId(body_id),
        b2Vec2{ impulse.x, impulse.y },
        b2Vec2{ point.x, point.y },
        wake
    );
}

auto PhysicsBody2D::apply_linear_impulse_to_center(const glm::vec2 &impulse, bool wake = true) -> void
{
    b2Body_ApplyLinearImpulseToCenter(
        b2LoadBodyId(body_id),
        b2Vec2{ impulse.x, impulse.y },
        wake
    );
}

auto PhysicsBody2D::apply_angular_impulse(float impulse, bool wake = true) -> void
{
    b2Body_ApplyAngularImpulse(
        b2LoadBodyId(body_id),
        impulse,
        wake
    );
}

auto PhysicsBody2D::rotational_inertia() -> float
{
    return b2Body_GetRotationalInertia(b2LoadBodyId(body_id));
}

auto PhysicsBody2D::set_linear_damping(float val) -> void
{
    b2Body_SetLinearDamping(b2LoadBodyId(body_id), val);
}

auto PhysicsBody2D::linear_damping() -> float
{
    return b2Body_GetLinearDamping(b2LoadBodyId(body_id));
}

auto PhysicsBody2D::set_angular_damping(float val) -> void
{
    b2Body_SetAngularDamping(b2LoadBodyId(body_id), val);
}

auto PhysicsBody2D::angular_damping() -> float
{
    return b2Body_GetAngularDamping(b2LoadBodyId(body_id));
}

auto PhysicsBody2D::set_gravity_scale(float val) -> void
{
    b2Body_SetGravityScale(b2LoadBodyId(body_id), val);
}

auto PhysicsBody2D::gravity_scale() -> float
{
    return b2Body_GetGravityScale(b2LoadBodyId(body_id));
}

auto PhysicsBody2D::awake() -> bool
{
    return b2Body_IsAwake(b2LoadBodyId(body_id));
}

auto PhysicsBody2D::set_awake(bool awake) -> void
{
    b2Body_SetAwake(b2LoadBodyId(body_id), awake);
}

auto PhysicsBody2D::wake_touching() -> void
{
    b2Body_WakeTouching(b2LoadBodyId(body_id));
}

auto PhysicsBody2D::enable_sleeping(bool enable) -> void
{
    b2Body_EnableSleep(b2LoadBodyId(body_id), enable);
}

auto PhysicsBody2D::sleep_enabled() -> bool
{
    return b2Body_IsSleepEnabled(b2LoadBodyId(body_id));
}

auto PhysicsBody2D::set_sleep_threshold(float threshold) -> void
{
    b2Body_SetSleepThreshold(b2LoadBodyId(body_id), threshold);
}

auto PhysicsBody2D::sleep_threshold() -> float
{
    return b2Body_GetSleepThreshold(b2LoadBodyId(body_id));
}

auto PhysicsBody2D::enabled() -> bool
{
    return b2Body_IsEnabled(b2LoadBodyId(body_id));
}

auto PhysicsBody2D::enable() -> void
{
    b2Body_Enable(b2LoadBodyId(body_id));
}

auto PhysicsBody2D::disable() -> void
{
    b2Body_Disable(b2LoadBodyId(body_id));
}

auto PhysicsBody2D::set_motion_locks(const PhysicsBody2D::MotionLocks &locks) -> void
{
    b2Body_SetMotionLocks(
        b2LoadBodyId(body_id), 
        b2MotionLocks{ locks.x, locks.y, locks.rotation }
    );
}

auto PhysicsBody2D::motion_locks() -> PhysicsBody2D::MotionLocks
{
    b2MotionLocks locks = b2Body_GetMotionLocks(b2LoadBodyId(body_id));
    return MotionLocks{ locks.linearX, locks.linearY, locks.angularZ };
}
    
} // namespace NoctisEngine::ECS
