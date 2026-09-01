#include <noctis_engine/rendering/renderer.hpp>

#include <stacktrace>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <noctis_engine/core/exit.hpp>
#include <noctis_engine/core/window.hpp>
#include <noctis_engine/rendering/shader_bindings.hpp>
#include <noctis_engine/rendering/default_shaders.hpp>


namespace NoctisEngine
{
    
struct DrawElementsIndirectCommand 
{
    std::uint32_t   count;
    std::uint32_t   instance_count;
    std::uint32_t   first_index;
    std::int32_t    base_vertex;
    std::uint32_t   base_instance;
};

struct ObjectData
{
    glm::mat4 mat;
};

constexpr BufferMapAccess BUFFER_MAP_ACCESSES = 
    BufferMapAccess::MAP_PERSISTENT_BIT 
    | BufferMapAccess::MAP_COHERENT_BIT 
    | BufferMapAccess::MAP_WRITE_BIT;

static auto glad_enable_disable(bool b, GLenum name) -> void 
{
    if (b) glEnable(name);
    else   glDisable(name);
}

Renderer::Renderer(const glm::ivec2 &framebuffer_size)
    : throw_on_err_{false}
{
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

    glDebugMessageCallback((GLDEBUGPROC)opengl_debug_message_callback, this);

    BufferFlag buf_flags = BufferFlag::MAP_PERSISTENT_BIT | BufferFlag::MAP_COHERENT_BIT | BufferFlag::MAP_WRITE_BIT;
    command_buffer_ = GPUBuffer(1, "renderer_command_buffer", buf_flags);
    objects_ssbo_ = GPUBuffer(1, "renderer_object_buffer", buf_flags);

    command_buffer_.map(BUFFER_MAP_ACCESSES);
    objects_ssbo_.map(BUFFER_MAP_ACCESSES);

    RENDERING_LOGGER.info("GPU: Vendor: \"{}\", Renderer: \"{}\", Version: \"{}\"", 
        (const char *)glGetString(GL_VENDOR), 
        (const char *)glGetString(GL_RENDERER),
        (const char *)glGetString(GL_VERSION)
    );

    world_render_pass_ = RenderPass{
        .name = "World render pass",
        .frame_buffer = FrameBuffer{"world_fb", framebuffer_size.x, framebuffer_size.y},
    };

    ui_render_pass_ = RenderPass{
        .name = "UI render pass",
        .frame_buffer = FrameBuffer{"ui_fb", framebuffer_size.x, framebuffer_size.y},
    };

    composition_render_pass_ = RenderPass{
        .name = "Composition render pas",
        .frame_buffer = std::nullopt,
    };

    composition_program_ = GraphicsProgram::create_helper(
        DefaultShaders::COMPOSITION_VERT_SHADER, 
        DefaultShaders::COMPOSITION_FRAG_SHADER, 
        "default_composition_prog"
    );

    fb_textures_.reserve(4);

    quad_mesh_manager_.upload(MeshData{
        // This quad spans the whole screen
        {
            Vertex{glm::vec3( 1.0f,  1.0f, 0.0f), glm::vec3(0), glm::vec3(0), glm::vec2(1, 1)},
            Vertex{glm::vec3(-1.0f,  1.0f, 0.0f), glm::vec3(0), glm::vec3(0), glm::vec2(0, 1)},
            Vertex{glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(0), glm::vec3(0), glm::vec2(0, 0)},
            Vertex{glm::vec3( 1.0f, -1.0f, 0.0f), glm::vec3(0), glm::vec3(0), glm::vec2(1, 0)},
        },
        {0, 1, 2, 2, 3, 0}
    });
}

auto Renderer::set_backface_culling(bool b) const -> void 
{
    glad_enable_disable(b, GL_CULL_FACE);
}

auto Renderer::set_depth_testing(bool b) const -> void 
{
    glad_enable_disable(b, GL_DEPTH_TEST);
}

auto Renderer::set_throw_on_err(bool b) -> void 
{
    throw_on_err_ = b;
}

auto Renderer::set_blend(bool b) const -> void 
{
    glad_enable_disable(b, GL_BLEND);
}

auto Renderer::set_blend_color(const Color &col)
{
    glBlendColor(
        col.red_f(),
        col.green_f(),
        col.blue_f(),
        col.alpha_f()
    );
}

auto Renderer::set_blend_func(BlendFunc sFactor, BlendFunc dFactor) const -> void 
{
    glBlendFunc(static_cast<GLenum>(sFactor), static_cast<GLenum>(dFactor));
}

auto Renderer::render_pass(DrawList &draw_list, const RenderPass &render_pass) -> void
{
    glPushDebugGroup(
        GL_DEBUG_SOURCE_APPLICATION, 
        0, 
        render_pass.name.size(), 
        render_pass.name.data()
    );

    auto &framebuffer = render_pass.frame_buffer;
    if (framebuffer)
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->gl_handle());

    bool command_buf_resized = GPUBuffer::resize(
        command_buffer_, 
        draw_list.required_indirect_draw_cmds() * sizeof(DrawElementsIndirectCommand)
    );

    bool object_ssbo_resized = GPUBuffer::resize(
        objects_ssbo_, 
        draw_list.required_indirect_draw_cmds() * sizeof(ObjectData)
    );

    if (command_buf_resized)
        command_buffer_.map(BUFFER_MAP_ACCESSES);

    if (object_ssbo_resized)
        objects_ssbo_.map(BUFFER_MAP_ACCESSES);

    glBindBufferBase(
        GL_SHADER_STORAGE_BUFFER, 
        ShaderBindings::OBJECTS_SSBO, 
        objects_ssbo_.gl_handle()
    );

    glBindBuffer(
        GL_DRAW_INDIRECT_BUFFER, 
        command_buffer_.gl_handle()
    );

    auto command_buf_ptr = static_cast<DrawElementsIndirectCommand *>(command_buffer_.mapped_ptr());
    auto object_ssbo_ptr = static_cast<ObjectData *>(objects_ssbo_.mapped_ptr());

    auto curr_cmd = draw_list.data().begin();
    auto cmds_end = draw_list.data().end();

    std::uint32_t total_commands{0};
    std::uint32_t command_batch_start{0};
    std::uint32_t num_objects{0};

    std::uint32_t last_program_id{0};
    std::uint32_t last_texture_id{0};
    std::uint32_t last_vao_id{0};
    
    auto flush_commands = [&]() -> void
    {
        std::uint32_t count = total_commands - command_batch_start;
        if (count == 0)
            return;

        glMultiDrawElementsIndirect(
            GL_TRIANGLES,
            GL_UNSIGNED_INT,
            (void *)(command_batch_start * sizeof(DrawElementsIndirectCommand)),
            count,
            0
        );

        command_batch_start = total_commands;
    };

    while (curr_cmd < cmds_end)
    {
        auto cmd_type = *reinterpret_cast<const DrawCommandType *>(curr_cmd.base());
        curr_cmd += sizeof(DrawCommandType);

        switch (cmd_type)
        {
            case DrawCommandType::BIND_TEXTURE:
            {
                if (last_program_id <= 0)
                {
                    RENDERING_LOGGER.error("Shader must be bound to use a texture. Shader binding should be done before anything else in the draw list");
                    return;
                }

                auto texture_cmd = reinterpret_cast<const BindTextureCmd *>(curr_cmd.base());
                
                if (last_texture_id != texture_cmd->tex)
                {
                    flush_commands();
                    glActiveTexture(GL_TEXTURE0 + texture_cmd->bind_point);
                    glBindTexture(GL_TEXTURE_2D, texture_cmd->tex);

                    int loc = glGetUniformLocation(last_program_id, texture_cmd->tex_name.data());
                    if (loc < 0) 
                    {
                        RENDERING_LOGGER.error("Uniform for texture \"{}\" couldn't be found", texture_cmd->tex_name);
                        break;
                    }

                    glUniform1i(loc, texture_cmd->bind_point);

                    last_texture_id = texture_cmd->tex;
                }

                curr_cmd += sizeof(BindTextureCmd);
                break;
            }
            
            case DrawCommandType::BIND_PROGRAM:
            {
                auto shader_cmd = reinterpret_cast<const BindProgramCmd *>(curr_cmd.base());
            
                if (last_program_id != shader_cmd->prog)
                {
                    flush_commands();
                    glUseProgram(shader_cmd->prog);

                    last_program_id = shader_cmd->prog;
                }

                curr_cmd += sizeof(BindProgramCmd);
                break;
            }

            case DrawCommandType::BIND_VAO:
            {
                auto vao_cmd = reinterpret_cast<const BindVAOCmd *>(curr_cmd.base());
                
                if (last_vao_id != vao_cmd->vao)
                {
                    flush_commands();
                    glBindVertexArray(vao_cmd->vao);

                    last_vao_id = vao_cmd->vao;
                }

                curr_cmd += sizeof(BindVAOCmd);
                break;
            }

            case DrawCommandType::BIND_BUFFER:
            {
                auto buffer_cmd = reinterpret_cast<const BindBufferCmd *>(curr_cmd.base());
                
                flush_commands();
                glBindBuffer(buffer_cmd->target, buffer_cmd->buffer);
                
                curr_cmd += sizeof(BindBufferCmd);
                break;
            }
            
            case DrawCommandType::BIND_BUFFER_BASE:
            {
                auto buffer_base_cmd = reinterpret_cast<const BindBufferBaseCmd *>(curr_cmd.base());

                flush_commands();
                glBindBufferBase(
                    buffer_base_cmd->target, 
                    buffer_base_cmd->bind_point, 
                    buffer_base_cmd->buffer
                );

                curr_cmd += sizeof(BindBufferBaseCmd);
                break;
            }
            
            case DrawCommandType::SET_UNIFORM:
            {
                if (last_program_id <= 0)
                {
                    RENDERING_LOGGER.error("Shader must be bound to set a uniform. Shader binding should be done before anything else in the draw list");
                    return;
                }

                auto uniform_cmd = reinterpret_cast<const SetUniformCmd *>(curr_cmd.base());
                const UniformInfo &info = uniform_cmd->info;

                flush_commands();
                int loc = glGetUniformLocation(last_program_id, info.name.data());
                if (loc < 0) 
                {
                    RENDERING_LOGGER.error("Uniform {} couldn't be found", info.name);
                    break;
                }

                switch (info.type) 
                {
                    case UniformType::BOOL:  glUniform1i(loc, std::get<bool>(info.val)); break;
                    case UniformType::INT:   glUniform1i(loc, std::get<int>(info.val)); break;
                    case UniformType::FLOAT: glUniform1f(loc, std::get<float>(info.val)); break;
                }

                curr_cmd += sizeof(SetUniformCmd);
                break;
            }
            
            case DrawCommandType::DRAW_MESH:
            {
                if (last_program_id <= 0)
                {
                    RENDERING_LOGGER.error("Shader must be bound to draw a mesh. Shader binding should be done before anything else in the draw list");
                    return;
                }

                auto draw_mesh_cmd = reinterpret_cast<const DrawMeshCmd *>(curr_cmd.base());
                const MeshView &mv = draw_mesh_cmd->mesh_view;

                command_buf_ptr[total_commands++] = DrawElementsIndirectCommand{
                    .count = static_cast<GLuint>(mv.indices_count),
                    .instance_count = 1,
                    .first_index = static_cast<GLuint>(mv.indices_offset),
                    .base_vertex = static_cast<GLint>(mv.vertices_offset),
                    .base_instance = num_objects,
                };

                object_ssbo_ptr[num_objects++] = ObjectData{
                    .mat = draw_mesh_cmd->model_mat
                };

                curr_cmd += sizeof(DrawMeshCmd);
                break;
            }
            
            case DrawCommandType::CLEAR:
            {
                auto clear_cmd = reinterpret_cast<const ClearCmd *>(curr_cmd.base());
                
                GLbitfield mask{0};
                if (clear_cmd->clear_color)
                {
                    const Color &color = clear_cmd->color;
                    glClearColor(
                        color.red_f(),
                        color.green_f(),
                        color.blue_f(),
                        color.alpha_f()
                    );

                    mask |= GL_COLOR_BUFFER_BIT;
                }

                if (clear_cmd->clear_depth)
                {    
                    glClearDepth(clear_cmd->depth);
                    mask |= GL_DEPTH_BUFFER_BIT;
                }

                glClear(mask);

                curr_cmd += sizeof(ClearCmd);
                break;
            }

            case DrawCommandType::DRAW_LINES:
            {
                auto draw_lines_cmd = reinterpret_cast<const DrawLinesCommand *>(curr_cmd.base());

                glDrawArrays(GL_LINES, draw_lines_cmd->first, draw_lines_cmd->count);

                curr_cmd += sizeof(DrawLinesCommand);
                break;
            }

            case DrawCommandType::DRAW_TRIANGLES:
            {
                auto draw_tris_cmd = reinterpret_cast<const DrawTrianglesCommand *>(curr_cmd.base());
                
                glDrawArrays(GL_TRIANGLES, draw_tris_cmd->first, draw_tris_cmd->count);

                curr_cmd += sizeof(DrawTrianglesCommand);
                break;
            }
        }
    }

    if (framebuffer)
    {
        fb_textures_.push_back(&framebuffer->color_tex());
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    flush_commands();

    glPopDebugGroup();
}

auto Renderer::render_world(DrawList &draw_list) -> void
{
    render_pass(draw_list, world_render_pass_);
}

auto Renderer::render_ui(DrawList &draw_list) -> void
{
    render_pass(draw_list, ui_render_pass_);
}

auto Renderer::show_final_image() -> void
{
    DrawList draw_list{};

    draw_list.clear_screen(Color{0, 0, 0, 0}, 1.0f, true, true);
    quad_mesh_manager_.use(draw_list);
    composition_program_.bind(draw_list);

    for (std::size_t i = 0; i < fb_textures_.size(); i++)
    {
        const Texture *tex = fb_textures_[i];
        tex->bind(draw_list, i, tex->name());
    }

    render_pass(draw_list, composition_render_pass_);
}

auto Renderer::resize_framebuffer(int new_width, int new_height) -> void
{
    world_render_pass_.frame_buffer->resize(new_width, new_height);
    ui_render_pass_.frame_buffer->resize(new_width, new_height);
    composition_render_pass_.frame_buffer->resize(new_width, new_height);
}

auto Renderer::opengl_debug_message_callback(        
    std::uint32_t source, 
    std::uint32_t type, 
    std::uint32_t id, 
    std::uint32_t severity,
    int length, 
    const char *message, 
    const void *userParam) -> void
{
    if (severity == GL_DEBUG_SEVERITY_LOW)
    {
        RENDERING_LOGGER.info("OpenGL: {}", message);
    }
    else if (severity == GL_DEBUG_SEVERITY_MEDIUM) 
    {
        RENDERING_LOGGER.warn("OpenGL: {}", message);
        RENDERING_LOGGER.warn("Stacktrace:\n{}", std::stacktrace::current(1));
    }
    else if (severity == GL_DEBUG_SEVERITY_HIGH) 
    {
        auto gh = static_cast<const Renderer *>(userParam);

        RENDERING_LOGGER.critical("OpenGL: {}", message);
        RENDERING_LOGGER.critical("Stacktrace:\n{}", std::stacktrace::current(1));

        if (gh->throw_on_err_)
        {
            RENDERING_LOGGER.critical("An OpenGL error was encountered, and throw_on_err is enabled.", std::stacktrace::current(1));
            exit_program_failure();
        }
    }
}

} // namespace NoctisEngine
