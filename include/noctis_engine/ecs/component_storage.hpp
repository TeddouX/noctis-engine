#pragma once
#include <unordered_map>
#include <vector>
#include <print>
#include <iostream>

#include "entity.hpp"


namespace NoctisEngine
{
    
class IComponentStorage
{
public:
    virtual ~IComponentStorage() = default;

    virtual auto has(Entity entity) const -> bool = 0;
    virtual auto remove(Entity entity) -> void = 0;
private:
};

/// @internal
/// @brief Sparse set that stores components
/// @tparam ComponentType_ The components' this storage stores type
template <typename ComponentType_>
class ComponentStorage : public IComponentStorage
{
public:
    /// @brief Default constructor
    ComponentStorage() = default;
    ~ComponentStorage() = default;

    /// @brief Adds an entity with a component to this storage
    /// @param entity The entity
    /// @param component The component
    auto add(Entity entity, ComponentType_ component) -> void
    {
        sparse_[entity.id()] = dense_.size();
        dense_.push_back(component);
        entity_indices_.push_back(entity);
    }

    /// @brief Removes an entity from this storage
    /// @param entity The entity that should be removed
    auto remove(Entity entity) -> void override
    {
        if (not has(entity))
            return;

        if (dense_.size() > 1)
        {
            std::uint32_t entity_idx = sparse_[entity.id()];
            std::uint32_t last = dense_.size() - 1;
            
            dense_[entity_idx] = dense_[last];

            Entity moved_entity = entity_indices_[last];
            entity_indices_[entity_idx] = moved_entity;
            sparse_[moved_entity.id()] = entity_idx;
        }

        dense_.pop_back();
        entity_indices_.pop_back();

        sparse_.erase(entity.id());
    }

    /// @brief Gets an entity's component
    /// @param entity The entity
    /// @return nullptr if the entity isn't stored in this storage, the component otherwise 
    auto get(Entity entity) -> ComponentType_ *
    {
        auto it = sparse_.find(entity.id());
        if (it == sparse_.end())
            return nullptr;
        return &dense_[it->second];
    }

    /// @brief Gets an entity's component as a constant value
    /// @param entity The entity
    /// @return nullptr if the entity isn't stored in this storage, 
    /// the component otherwise as a const pointer 
    auto get(Entity entity) const -> const ComponentType_ *
    {
        auto it = sparse_.find(entity.id());
        if (it == sparse_.end())
            return nullptr;
        return &dense_[it->second];
    }

    /// @brief Checks if an entity exists in this storage
    /// @param entity The entity
    /// @return True if it exists, false otherwise
    auto has(Entity entity) const -> bool override
    {
        return sparse_.count(entity.id()) > 0;
    }

    /// @return All this storage's components
    auto data() -> std::vector<ComponentType_> &
    {
        return dense_;
    }

    /// @return All this storage's components as a const reference 
    auto data() const -> const std::vector<ComponentType_> &
    {
        return dense_;
    }

    /// @return All the entities stored in this storage 
    auto entities() const -> const std::vector<Entity> &
    {
        return entity_indices_;
    }

private:
    std::unordered_map<std::uint32_t, std::size_t> sparse_;
    std::vector<ComponentType_> dense_;
    std::vector<Entity> entity_indices_;
};

} // namespace NoctisEngine
