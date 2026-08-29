#pragma once
#include <typeindex>
#include <memory>
#include <print>

#include "ecs_fwd.hpp"
#include "component_storage.hpp"


namespace NoctisEngine::ECS
{
    
/// @brief This function holds references to all components 
/// and entities that were created
/// @warning Any references or pointers returned by the member functions of this class shouldn't 
/// be stored as they can be invalidated when the world is modified 
class World
{
public:
    /// @brief Creates a world
    World()
        : next_entity_id_{1}
    {}

    ~World() = default;

    /// @brief Creates an entity in this world
    /// @return The created entity
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

    /// @brief Destroys an entity, and its associated components
    /// @param entity The entity that should be destroyed
    /// @warning This can be expensive, so use sparringly
    auto destroy_entity(Entity entity) -> void
    {
        free_ids_.push_back(entity.id());

        for (const auto &[_, storage] : storages_)
        {
            if (storage->has(entity))
                storage->remove(entity);
        }
    }

    /// @brief Adds a component to an entity
    /// @tparam T The component's type
    /// @param entity The entity the component should be added to
    /// @param component The component
    template <typename T>
    auto add_component(Entity entity, T component) -> void
    {
        get_storage<T>()->add(entity, component);
    }

    /// @brief Gets a component from an entity
    /// @tparam T The component's type
    /// @param entity The entity the component should gotten from
    /// @return nullptr if the entity doesn't have that component, a pointer 
    /// to the component otherwise
    template <typename T>
    auto get_component(Entity entity) -> T *
    {
        return get_storage<T>()->get(entity);
    }

    /// @brief Gets a constant component from an entity
    /// @tparam T The component's type
    /// @param entity The entity the component should gotten from
    /// @return nullptr if the entity doesn't have that component, a constant pointer 
    /// to the component otherwise
    template <typename T>
    auto get_component(Entity entity) const -> const T *
    {
        if (const auto storage = get_const_storage<T>())
            return storage->get(entity);
        return nullptr;
    }

    /// @brief Removes a component from anReference  entity
    /// @tparam T The component's type
    /// @param entity The entity the component should be removed from
    template <typename T>
    auto remove_component(Entity entity) -> void
    {
        get_storage<T>()->remove(entity);
    }

    /// @brief Checks if an entity has a component
    /// @tparam T The component's type
    /// @param entity The entity that should be checked
    /// @return True if the entity has that component, false otherwise
    template <typename T>
    auto has_component(Entity entity) const -> bool
    {
        if (const auto storage = get_const_storage<T>())
            return storage->has(entity);
        return false;
    }

    /// @brief Gets all entities that have a component
    /// @tparam T The component's type
    /// @return nullptr if no entities have that component, a constant 
    /// pointer to all the entities that have this component
    template <typename T>
    auto all_entities() const -> const std::vector<Entity> *
    {
        if (const auto storage = get_const_storage<T>())
            return &storage->entities();
        return nullptr;
    }

    /// @brief Gets all components of one type
    /// @tparam T The component's type
    /// @return A vector containing all the components of that type, can be empty
    template <typename T>
    auto all() -> std::vector<T> &
    {
        return get_storage<T>()->data();
    }

    /// @brief Gets a constant pointer to all components of one type
    /// @tparam T The component's type
    /// @return A constant pointer to the vector containing all the components
    /// of that type
    template <typename T>
    auto all() const -> const std::vector<T> *
    {
        if (const auto storage = get_const_storage<T>())
            return &storage->data();
        return nullptr;
    }

    /// @brief Gets a component storage as a shared_ptr for one type of component
    /// @tparam T The component's type
    /// @return A shared pointer to the component storage. This one can be stored
    template <typename T>
    auto get_storage() -> std::shared_ptr<ComponentStorage<T>>
    {
        std::type_index type_idx{typeid(T)};
        if (storages_.find(type_idx) == storages_.end())
            storages_[type_idx] = std::make_shared<ComponentStorage<T>>();
        return std::dynamic_pointer_cast<ComponentStorage<T>>(storages_[type_idx]);
    }

    /// @brief Gets a component storage as a constant shared_ptr for one type of component.
    /// @tparam T The component's type
    /// @return A shared pointer to the component storage. This one can be stored
    template <typename T>
    auto get_const_storage() const -> const std::shared_ptr<ComponentStorage<T>>
    {
        std::type_index type_idx{typeid(T)};
        if (storages_.find(type_idx) == storages_.end())
            return nullptr;
        return std::dynamic_pointer_cast<ComponentStorage<T>>(storages_.at(type_idx));
    }

    /// @brief Creates a view of all components provided in the template argument
    /// @tparam ...Components_ All the components this view will access
    /// @return The created view
    template <typename... Components_>
    auto query() -> View<Components_...> 
    {
        return View<Components_...>{*this};
    }

    /// @brief Gets the smallest storage's entities among multiple component storages
    /// @tparam ...Components_ The components' types
    /// @return The smallest storage's entities
    template <typename... Components_>
    auto smallest_storage() const -> const std::vector<Entity> *
    {
        const std::vector<Entity> *smallest = nullptr;

        auto check = [&](const std::vector<Entity> *entities) -> void 
        {
            if (not entities)
                return;

            if (not smallest || entities->size() < smallest->size())
                smallest = entities;
        };

        (check(all_entities<Components_>()), ...);

        return smallest;
    }

private:
    std::unordered_map<std::type_index, std::shared_ptr<IComponentStorage>> storages_;
    std::int32_t next_entity_id_;
    std::vector<std::int32_t> free_ids_;
};

template <typename... Components_>
auto View<Components_...>::each(std::function<void(Entity, Components_ &...)> func) -> void
{
    auto *storage = world_.template smallest_storage<Components_...>();
    if (not storage)
        return;

    for (const auto &entity : *storage)
    {
        if ((world_.template has_component<Components_>(entity) && ...))
            func(entity, *world_.template get_component<Components_>(entity)...);
    }
}


} // namespace NoctisEngine::ECS
