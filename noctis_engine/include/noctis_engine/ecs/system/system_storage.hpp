#pragma once
#include "system.hpp"

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>


namespace NoctisEngine
{

template<
    template<typename...> class ISystem_Base_, 
    typename System_, 
    typename T = typename System_::base_type
>
struct InheritedArgs;

template<
    template<typename...> class ISystem_Base_, 
    typename System_, 
    typename... OwnedComponents_,
    typename... GetComponents_,
    typename... ExcludeComponents_
>
struct InheritedArgs<
    ISystem_Base_, 
    System_, 
    ISystem_Base_<
        OwnedComponents<OwnedComponents_...>, 
        GetComponents<GetComponents_...>, 
        ExcludeComponents<ExcludeComponents_...>
    >
> {
    using owned_components_tuple = std::tuple<OwnedComponents_...>;
    using get_components_tuple = std::tuple<GetComponents_...>;
    using exclude_components_tuple = std::tuple<ExcludeComponents_...>;
};

class NCENG_API SystemStorage {
public:
    using Systems = std::vector<std::shared_ptr<ISystemBase>>;
    using UpdateFuncStorage = std::vector<std::function<void (float)>>;

    SystemStorage() = default;
    ~SystemStorage() = default;

    template <typename System_, typename... Args_>
    requires(std::is_base_of_v<ISystemBase, System_>)
    auto add_system(
        entt::registry &reg, 
        Args_ &&...args
    ) -> void;

    auto get_update_functions() -> UpdateFuncStorage &;

private:
    Systems systems_;
    UpdateFuncStorage updateFunctions_;

    template <
        typename System_, 
        typename... OwnedComponents_, 
        typename... GetComponents_, 
        typename... ExcludeComponents_
    >
    auto register_system(
        entt::registry &reg,
        std::shared_ptr<System_> system,
        std::tuple<OwnedComponents_...>,
        std::tuple<GetComponents_...>,
        std::tuple<ExcludeComponents_...>
    ) -> void;
};


template <typename System_, typename... Args_>
requires(std::is_base_of_v<ISystemBase, System_>)
auto SystemStorage::add_system(
    entt::registry &reg, 
    Args_ &&...args
) -> void {
    using OwnedComponentsTuple = typename InheritedArgs<ISystem, System_>::owned_components_tuple;
    using GetComponentsTuple = typename InheritedArgs<ISystem, System_>::get_components_tuple;
    using ExcludeComponentsTuple = typename InheritedArgs<ISystem, System_>::exclude_components_tuple;

    auto system = std::make_shared<System_>(std::forward<Args_>(args)...);
    register_system(
        reg, system, 
        OwnedComponentsTuple{}, 
        GetComponentsTuple{}, 
        ExcludeComponentsTuple{}
    );
}

template <
    typename System_, 
    typename... OwnedComponents_, 
    typename... GetComponents_, 
    typename... ExcludeComponents_
>
auto SystemStorage::register_system(
    entt::registry &reg,
    std::shared_ptr<System_> system,
    std::tuple<OwnedComponents_...>,
    std::tuple<GetComponents_...>,
    std::tuple<ExcludeComponents_...>
) -> void {
    auto group = reg.group<OwnedComponents_...>(
        entt::get_t<GetComponents_...>{},
        entt::exclude_t<ExcludeComponents_...>{}
    );

    systems_.push_back(system);

    auto updateFunc = [system, group](float dt) -> void {
        system->update(dt, group);
    };

    updateFunctions_.push_back(updateFunc);
}

} // namespace NoctisEngine
