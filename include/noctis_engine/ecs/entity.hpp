#pragma once
#include <cstdint>


namespace NoctisEngine::ECS
{

using EntityID = std::int32_t;

/// @brief Represents an entity in a world
class Entity
{
public:
    /// @brief Default constructor, creates an invalid entity
    Entity()
        : id_{-1}
    {}

    /// @brief Creates an entity from an ID, the created entity isn't 
    /// guaranteed to be valid in any worlds
    explicit Entity(EntityID id)
        : id_{id}
    {}
    
    /// @brief Checks if this entity is valid
    /// @return True if it's valid, false otherwise
    auto is_valid() const -> bool { return id_ >= 0; }

    /// @internal
    /// @brief Gets this entitie's id 
    auto id() -> EntityID { return id_; }

private:
    EntityID id_;
};

} // namespace NoctisEngine::ECS
