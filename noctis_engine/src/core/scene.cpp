#include <core/scene.hpp>

#include <ecs/component/transform.hpp>

namespace NoctisEngine
{

Scene::Scene() {
    // Essential storages
    reg_.storage<MeshView>();
    reg_.storage<Transform>();
    reg_.storage<MaterialKey>();
    reg_.storage<InstanceRenderedGroup>();

    // Groups used during rendering, partial owning to avoid headaches later
    // TODO: find a way to make them full owning
    reg_.group<>(
        entt::get_t<Transform, MeshView, MaterialKey>{},
        entt::exclude_t<InstanceRenderedGroup>{}
    );
}

auto Scene::create_entity() -> Entity {
    entt::entity e = reg_.create();
    return Entity{e, &reg_};
}

auto Scene::get_entity(entt::entity &rawEntity) -> Entity {
    return Entity{rawEntity, &reg_};
}

auto Scene::update(float dt) -> void {
    for (const auto &fun : updateSystems_.get_update_functions())
        fun(dt);
}

} // namespace NoctisEngine
