#pragma once
#include "ecs.hpp"
#include "../core/logging.hpp"
#include "../core/exception.hpp"

namespace NoctisEngine
{
    
class Entity {
public:
    Entity() = default;
    Entity(entt::entity entity, entt::registry *reg)
        : entity_(entity)
        , reg_(reg)
    {}

    template <typename T>
    auto add_component(T &&comp) -> void {
        using ComponentType = std::decay_t<T>;
        if (reg_->all_of<ComponentType>(entity_)) {
            Log::Warn("Tried to add the same component two times to one entity. If this is intended behaviour, use Entity::add_component_or_replace.");
            return;
        }

        reg_->emplace<ComponentType>(entity_, std::move(comp));
    }

    template <typename T>
    auto add_component_or_replace(T &comp) -> void {
        using ComponentType = std::decay_t<T>;
        reg_->emplace_or_replace<ComponentType>(entity_, std::move(comp));
    }

    template <typename T>
    auto get_component() -> T & {
        T *ptr = reg_->try_get<T>(entity_);
        
        if (!ptr)
            throw Exception("Tried getting a component from an entity that doesn't own it.");

        return *ptr;
    }

    template <typename T>
    auto get_component() const -> const T & {
        const T *ptr = reg_->try_get<T>(entity_);
        
        if (!ptr)
            throw Exception("Tried getting a component from an entity that doesn't own it.");

        return *ptr;
    }

    template <typename T>
    auto try_get_component() -> T * {
        return reg_->try_get<T>(entity_);
    }

    template <typename T>
    auto try_get_component() const -> const T * {
        return reg_->try_get<T>(entity_);
    }

    template <typename T>
    auto remove_component() -> void {
        reg_->remove<T>(entity_);
    }

    template <typename T>
    auto replace_component(T &comp) -> void {
        reg_->replace<T>(entity_, std::move(comp));
    }

    auto get_raw() const -> const entt::entity & {
        return entity_;
    }

private:
    entt::entity entity_;
    entt::registry *reg_;
};

} // namespace NoctisEngine
