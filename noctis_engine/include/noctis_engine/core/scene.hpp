#pragma once
#include "../ecs/ecs.hpp"
#include "../ecs/entity.hpp"
#include "../rendering/mesh/mesh_manager.hpp"
#include "../rendering/material/material_manager.hpp"
#include "../rendering/gpu_buffer.hpp"
#include "../rendering/renderer.hpp"
#include "../ecs/system/system.hpp"
#include "../ecs/system/system_storage.hpp"

namespace NoctisEngine
{

class NCENG_API Scene {
public:
    Scene();
    ~Scene() = default;

    auto create_entity() -> Entity;
    auto get_entity(entt::entity &rawEntity) -> Entity;

    template <typename System_, typename... Args_>
    requires(std::is_base_of_v<ISystemBase, System_>)
    auto add_update_system(Args_ &&...args) -> void;

    auto update(float dt) -> void;

    auto get_all_entities() -> entt::registry & { return reg_; }
    auto get_all_entities() const -> const entt::registry & { return reg_; }

private:
    entt::registry reg_;

    SystemStorage updateSystems_;
};


template <typename System_, typename... Args_>
requires(std::is_base_of_v<ISystemBase, System_>)
auto Scene::add_update_system(Args_ &&...args) -> void {
    updateSystems_.add_system<System_>(reg_, std::forward<Args_>(args)...);
}

} // namespace NoctisEngine
