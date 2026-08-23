#pragma once
#include <cstdint>


namespace NoctisEngine
{
    
class Entity
{
public:
    Entity()
        : id_{-1}
    {}
    
    auto is_valid() const -> bool { return id_ >= 0; }
    auto id() -> std::uint32_t { return id_; }

private:
    friend class ECSWorld;

    std::int32_t id_;

    explicit Entity(std::int32_t id)
        : id_{id}
    {}
};

} // namespace NoctisEngine
