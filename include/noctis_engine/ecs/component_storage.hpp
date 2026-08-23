#pragma once
#include <unordered_map>
#include <vector>
#include <print>
#include <iostream>

#include "entity.hpp"


namespace NoctisEngine
{
    
template <typename ComponentType_>
class ComponentStorage
{
public:
    ComponentStorage() = default;
    ~ComponentStorage() = default;

    auto add(Entity e, ComponentType_ component) -> void
    {
        sparse_[e.id()] = dense_.size();
        dense_.push_back(component);
        entity_indices_.push_back(e);
    }

    auto remove(Entity e) -> void
    {
        if (dense_.size() > 1)
        {
            std::uint32_t entity_idx = sparse_[e.id()];
            std::uint32_t last = dense_.size() - 1;
            
            dense_[entity_idx] = dense_[last];

            Entity moved_entity = entity_indices_[last];
            entity_indices_[entity_idx] = moved_entity;
            sparse_[moved_entity.id()] = entity_idx;
        }

        dense_.pop_back();
        entity_indices_.pop_back();

        sparse_.erase(e.id());
    }

    auto get(Entity e) -> ComponentType_ *
    {
        auto it = sparse_.find(e.id());
        if (it == sparse_.end())
            return nullptr;
        return &dense_[it->second];
    }

    auto get(Entity e) const -> const ComponentType_ *
    {
        auto it = sparse_.find(e.id());
        if (it == sparse_.end())
            return nullptr;
        return &dense_[it->second];
    }

    auto has(Entity e) const -> bool
    {
        return sparse_.count(e.id()) > 0;
    }

    auto data() -> std::vector<ComponentType_> &
    {
        return dense_;
    }

    auto data() const -> const std::vector<ComponentType_> &
    {
        return dense_;
    }

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
