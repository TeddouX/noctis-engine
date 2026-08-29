#pragma once
#include <cstdint>


namespace NoctisEngine::ECS
{

/// @brief Represents an entity in a world
class Entity
{
public:
    /// @brief Default constructor, creates an invalid entity
    Entity()
        : id_{-1}
    {}
    
    /// @brief Checks if this entity is valid
    /// @return True if it's valid, false otherwise
    auto is_valid() const -> bool { return id_ >= 0; }

    /// @internal
    /// @return This entitie's id 
    auto id() -> std::uint32_t { return id_; }

private:
    friend class World;

    std::int32_t id_;

    explicit Entity(std::int32_t id)
        : id_{id}
    {}
};

} // namespace NoctisEngine::ECS
