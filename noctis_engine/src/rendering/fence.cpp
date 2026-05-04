#include <rendering/fence.hpp>

#include <glad/gl.h>

namespace NoctisEngine
{
    
Fence::Fence()
    : fence_{nullptr}
{}

auto Fence::sync() -> void {
    fence_ = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
}

auto Fence::wait_sync() -> FenceStatus {
    FenceStatus res = FenceStatus::FENCE_NOT_SET;

    if (fence_) {
        res = static_cast<FenceStatus>(glClientWaitSync(
            (GLsync)fence_, 
            GL_SYNC_FLUSH_COMMANDS_BIT, 
            GL_TIMEOUT_IGNORED
        ));

        glDeleteSync((GLsync)fence_);
        fence_ = nullptr;
    }
    
    return res;
}

} // namespace NoctisEngine
