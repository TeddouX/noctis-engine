#include <rendering/renderer.hpp>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <rendering/mesh/mesh_view.hpp>
#include <ecs/component/transform.hpp>
#include <rendering/buffer_utils.hpp>
#include <rendering/shader_bindings.hpp>
#include <core/exception.hpp>

namespace NoctisEngine
{
   
static auto glad_enable_disable(bool b, GLenum name) -> void {
    if (b) glEnable(name);
    else   glDisable(name);
}

Renderer::Renderer(std::shared_ptr<MeshManager> meshManager) 
    : meshManager_(meshManager) 
{
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

    glDebugMessageCallback((GLDEBUGPROC)OpenGLDbgMessCallback, this);

    BufferFlag mappedBufFlags = BufferFlag::MAP_PERSISTENT_BIT | BufferFlag::MAP_WRITE_BIT;
    commandBuf_ = GPUBuffer(1, "renderer_command_buffer", mappedBufFlags);
    objectsSSBO_ = GPUBuffer(1, "renderer_object_buffer", mappedBufFlags);

    commandBuf_.map();
    objectsSSBO_.map();
}

auto Renderer::set_backface_culling(bool b) const -> void {
    glad_enable_disable(b, GL_CULL_FACE);
}

auto Renderer::set_depth_testing(bool b) const -> void {
    glad_enable_disable(b, GL_DEPTH_TEST);
}

auto Renderer::set_clear_screen_color(Color col) -> void {
    glClearColor(
        col.red_f(), 
        col.green_f(), 
        col.blue_f(), 
        col.alpha_f()
    );
}

auto Renderer::set_throw_on_err(bool b) -> void {
    throwOnErr_ = b;
}

void Renderer::clear_screen() const {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

auto Renderer::set_viewport_size(int w, int h) -> void {
    glViewport(0, 0, w, h);
}

auto Renderer::set_blend(bool b) -> void {
    glad_enable_disable(b, GL_BLEND);
}

auto Renderer::set_blend_func(BlendFunc sFactor, BlendFunc dFactor) -> void {
    glBlendFunc(static_cast<GLenum>(sFactor), static_cast<GLenum>(dFactor));
}

auto Renderer::render_entities(entt::registry &reg) -> void {
    renderFence_.wait_sync();

    auto nonIrGroup = reg.group<>(
        entt::get_t<Transform, MeshView, MaterialKey>{},
        entt::exclude_t<InstanceRenderedGroup>{}
    );

    auto irGroup = reg.group<>(
        entt::get_t<Transform, MaterialKey, InstanceRenderedGroup>{},
        entt::exclude_t<>{}
    );

    size_t numCommands = nonIrGroup.size() + irEntitiesData_.size();
    size_t numObjects = nonIrGroup.size() + irGroup.size();

    resize_buffer(commandBuf_, numCommands * sizeof(DrawElementsIndirectCommand));
    resize_buffer(objectsSSBO_, numObjects * sizeof(ObjectData));

    auto commandBufPtr = static_cast<DrawElementsIndirectCommand *>(commandBuf_.get_mapped_ptr());
    auto objectSSBOPtr = static_cast<ObjectData *>(objectsSSBO_.get_mapped_ptr());

    size_t commandIdx = 0;
    size_t objectIdx = 0;
    
    nonIrGroup.each([&](auto &transform, auto &mv, auto &matKey) -> void {
        commandBufPtr[commandIdx++] = DrawElementsIndirectCommand{
            .count = static_cast<GLuint>(mv.indicesCount),
            .instanceCount = 1u,
            .firstIndex = static_cast<GLuint>(mv.indicesOffset),
            .baseVertex = static_cast<GLint>(mv.verticesOffset),
            .baseInstance = static_cast<GLuint>(objectIdx),
        };

        objectSSBOPtr[objectIdx++] = ObjectData{
            .modelMat = transform.model_matrix(),
            .materialIdx = static_cast<GLuint>(matKey.get()),
        };
    });

    // Instanced rendered entities
    if (!irEntitiesData_.empty()) {
        if (irEntitiesData_.size() != irgMeshViews_.size())
            throw Exception("There isn't a mesh view set for every IRG. Make sure to call Renderer::set_irg_mesh_view for every IRG.");

        for (auto &meshView : irgMeshViews_) {
            commandBufPtr[commandIdx++] = DrawElementsIndirectCommand{
                .count         = static_cast<GLuint>(meshView.indicesCount),
                .instanceCount = 0u,
                .firstIndex    = static_cast<GLuint>(meshView.indicesOffset),
                .baseVertex    = static_cast<GLint>(meshView.verticesOffset),
                .baseInstance  = 0, 
            };
        }

        commandIdx -= irgMeshViews_.size();

        size_t idx = commandIdx;
        for (auto &entitiesData : irEntitiesData_) {
            commandBufPtr[idx].baseInstance = objectIdx;

            std::uint32_t instanceCount = 0;
            for (auto &entityData : entitiesData) {
                // const auto &[transform, matKey] = irGroup.get<Transform, MaterialKey>(entity);
                const auto *matKey = entityData.matKey;
                auto *transform = entityData.transform;

                instanceCount++;

                objectSSBOPtr[objectIdx++] = ObjectData{
                    .modelMat    = transform->model_matrix(),
                    .materialIdx = static_cast<GLuint>(matKey->get()),
                };
            }

            commandBufPtr[idx].instanceCount = instanceCount;
            idx++;
        }

    }

    objectsSSBO_.bind_buffer_base(BufferTarget::SHADER_STORAGE_BUFFER, ShaderBindings::OBJECTS_BUFFER_SSBO);

    meshManager_->bind_vertex_array();

    commandBuf_.bind_as(BufferTarget::DRAW_INDIRECT_BUFFER);

    glMemoryBarrier(GL_CLIENT_MAPPED_BUFFER_BARRIER_BIT);
    glMultiDrawElementsIndirect(
        GL_TRIANGLES,
        GL_UNSIGNED_INT,
        (void*)0,
        numCommands,
        0
    );

    renderFence_.sync();
}

auto Renderer::create_irg() -> InstanceRenderedGroup {
    return InstanceRenderedGroup{numInstanceRenderedGroups_++};
}

auto Renderer::set_irg_mesh_view(InstanceRenderedGroup group, MeshView mesh) -> void {
    if (group.get() >= irgMeshViews_.size())
        irgMeshViews_.resize(group.get() + 1);
    irgMeshViews_[group.get()] = mesh;
}

auto Renderer::register_ir_entity(Entity &entity) -> void {
    const auto *irg = entity.try_get_component<InstanceRenderedGroup>();
    const auto *matKey = entity.try_get_component<MaterialKey>();
    auto *transform = entity.try_get_component<Transform>();

    if (irg == nullptr) {
        Log::Warn("Called Renderer::register_ir_entity with an entity that doesn't have an InstanceRenderedGroup");
        return;
    }

    if (transform == nullptr) {
        Log::Warn("Called Renderer::register_ir_entity with an entity that doesn't have an Transform");
        return;
    }

    if (matKey == nullptr) {
        Log::Warn("Called Renderer::register_ir_entity with an entity that doesn't have an MaterialKey");
        return;
    }

    if (!irg->is_valid()) {
        Log::Error("Tried to call Renderer::register_ir_entity with an entity that has an invalid IRG");
        return;
    }

    auto idx = irg->get();
    if (idx >= irEntitiesData_.size())
        irEntitiesData_.resize(idx + 1);

    irEntitiesData_[idx].push_back(IRObjectData{
        .matKey = matKey, 
        .transform = transform
    });
}


void Renderer::OpenGLDbgMessCallback(uint32_t source, uint32_t type, uint32_t id, uint32_t severity,
        int length, const char* message, const void* userParam
) {
    if (severity == GL_DEBUG_SEVERITY_LOW)
        Log::Info("OpenGL: {}", message);
    else if (severity == GL_DEBUG_SEVERITY_MEDIUM) {
        Log::Warn("OpenGL: {}", message);
        Log::Warn("Stacktrace:\n{}", std::stacktrace::current(1));
    }
    else if (severity == GL_DEBUG_SEVERITY_HIGH) {
        auto gh = static_cast<const Renderer *>(userParam);

        if (gh->throwOnErr_) {
            Log::Error("OpenGL Error: {}", message);
            throw Exception("An OpenGL error has been generated and throw on error is enabled, for more info see above.");
        }
        else {
            Log::Error("OpenGL: {}", message);
            Log::Error("Stacktrace:\n{}", std::stacktrace::current(1));
        }
    }
}

} // namespace NoctisEngine
