#pragma once
#include <typeindex>
#include <memory>

#include "component_storage.hpp"


namespace NoctisEngine::ECS
{
    
class World
{
public:
    World()
        : next_entity_id_{1}
    {}

    ~World() = default;

    auto create_entity() -> Entity
    {
        if (not free_ids_.empty())
        {
            std::int32_t id = free_ids_.back();
            free_ids_.pop_back();
            return Entity{id};
        }

        return Entity{next_entity_id_++};
    }

    auto destroy_entity(Entity e)
    {
        free_ids_.push_back(e.id());
    }

    template <typename T>
    auto add_component(Entity e, T component) -> void
    {
        get_storage<T>().add(e, component);
    }

    template <typename T>
    auto get_component(Entity e) -> T *
    {
        return get_storage<T>().get(e);
    }

    template <typename T>
    auto get_component(Entity e) const -> const T *
    {
        if (auto *storage = get_const_storage<T>())
            return storage->get(e);
        return nullptr;
    }

    template <typename T>
    auto remove_component(Entity e) -> void
    {
        get_storage<T>().remove(e);
    }

    template <typename T>
    auto has_component(Entity e) const -> bool
    {
        if (auto *storage = get_const_storage<T>())
            return storage->has(e);
        return false;
    }

    template <typename T>
    auto all_entities() const -> const std::vector<Entity> *
    {
        if (auto *storage = get_const_storage<T>())
            return &storage->entities();
        return nullptr;
    }

    template <typename T>
    auto all() -> std::vector<T> &
    {
        return get_storage<T>().data();
    }

    template <typename T>
    auto all() const -> const std::vector<T> *
    {
        if (auto *storage = get_const_storage<T>())
            return &storage->data();
        return nullptr;
    }

private:
    std::unordered_map<std::type_index, std::shared_ptr<void>> storages_;
    std::int32_t next_entity_id_;
    std::vector<std::int32_t> free_ids_;

    template <typename T>
    auto get_storage() -> ComponentStorage<T> &
    {
        std::type_index type_idx{typeid(T)};
        if (storages_.find(type_idx) == storages_.end())
            storages_[type_idx] = std::make_shared<ComponentStorage<T>>();
        return *static_cast<ComponentStorage<T> *>(storages_[type_idx].get());
    }

    template <typename T>
    auto get_const_storage() const -> const ComponentStorage<T> *
    {
        std::type_index type_idx{typeid(T)};
        if (storages_.find(type_idx) == storages_.end())
            return nullptr;
        return static_cast<ComponentStorage<T> *>(storages_.at(type_idx).get());
    }
};

} // namespace NoctisEngine::ECS
