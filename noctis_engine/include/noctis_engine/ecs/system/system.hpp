#pragma once
#include <string>

#include "../ecs.hpp"
#include "../../noctis_engine.hpp"

namespace NoctisEngine
{

class NCENG_API ISystemBase {
public:
    virtual ~ISystemBase() = default; 

    virtual auto init() -> void {};
};

template <typename... Components_>
struct OwnedComponents {};

template <typename... Components_>
struct GetComponents {};

template <typename... Components_>
struct ExcludeComponents {};


template <typename Owned, typename Get, typename Exclude>
class ISystem;

template <typename... OwnedComponents_, typename... GetComponents_, typename... ExcludeComponents_>
class ISystem<
    OwnedComponents<OwnedComponents_...>,
    GetComponents<GetComponents_...>, 
    ExcludeComponents<ExcludeComponents_...>
> : public virtual ISystemBase 
{
public:
    using base_type = ISystem<
        OwnedComponents<OwnedComponents_...>, 
        GetComponents<GetComponents_...>, 
        ExcludeComponents<ExcludeComponents_...>
    >;

    virtual auto update(float dt, entt::group<
            entt::owned_t<OwnedComponents_...>, 
            entt::get_t<GetComponents_...>, 
            entt::exclude_t<ExcludeComponents_...>
        > group
    ) -> void {};
};

} // namespace NoctisEngine
