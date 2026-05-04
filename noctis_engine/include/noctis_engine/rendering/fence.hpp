#pragma once
#include "../noctis_engine.hpp"

#include <cstdint>

namespace NoctisEngine
{
    
enum class FenceStatus : std::uint32_t {
    FENCE_NOT_SET = 0,
    ALREADY_SIGNALED = 0x911A,
    TIMEOUT_EXPIRED = 0x911B,
    CONDITION_SATISFIED = 0x911C,
    WAIT_FAILED = 0x911D,
};

class NCENG_API Fence {
public:
    Fence();

    auto sync() -> void;
    auto wait_sync() -> FenceStatus;

private:
    void *fence_;
};

} // namespace NoctisEngine
