#pragma once
#include <vector>
#include <cstring>

#include "draw_commands.hpp"
#include "gpu_buffer.hpp"
#include "vertex.hpp"
#include "../ecs/entity.hpp"
#include "../ecs/ecs_world.hpp"
#include "../math/color.hpp"


namespace NoctisEngine::Rendering
{

/// @brief This is used to let the renderer know what things to render to the screen and how to render them
class DrawList
{
public:
    /// @brief Default constructor
    DrawList() = default;
    ~DrawList() = default;

    /// @brief Binds a texture
    /// @param tex The OpenGL texture handle
    /// @param bind_point The binding point for the texture
    /// @param tex_name The texture's name
    auto bind_texture(std::uint32_t tex, std::uint32_t bind_point, std::string_view tex_name) -> void;

    /// @brief Binds a buffer to a target
    /// @param buffer The buffer to bind
    /// @param target The target to bind it to
    auto bind_buffer(const GPUBuffer &buffer, BufferTarget target) -> void;
    
    /// @brief Binds a buffer to a target and to a binding point
    /// @param buffer The buffer to bind
    /// @param target The target to bind it to
    /// @param bind_point The binding point
    /// @warning Not all buffers can be bound to a point
    auto bind_buffer_base(const GPUBuffer &buffer, BufferTarget target, std::uint32_t bind_point) -> void;

    /// @brief Binds a shader program
    /// @param prog The OpenGL program handle
    auto bind_program(std::uint32_t prog) -> void;

    /// @brief Sets a uniform from a previously bound program
    /// @param info The uniform's info
    /// @warning This requires a shader to be bound before 
    auto set_uniform(UniformInfo info) -> void;

    /// @brief Binds a VAO (vertex array object) 
    /// @param vao The OpenGL VAO handle
    auto bind_vao(std::uint32_t vao) -> void;

    /// @brief Draws a mesh to a screen
    /// @param mesh_view The mesh view from a mesh manager
    /// @param model_mat The model matrix describing where the mesh should be placed.
    /// This is can be gotten from a transform.
    /// @warning This requires a shader to be bound before
    auto draw_mesh(MeshView mesh_view, const glm::mat4 &model_mat = glm::mat4{1}) -> void;
    
    /// @brief Draws an entity as a sprite
    /// @param entity The entity to draw
    /// @param world The world it belongs to
    /// @warning The entity is required to have a Sprite and a Transform2D components
    auto draw_sprite_entity(ECS::Entity entity, const ECS::World &world) -> void;

    /// @brief Draws triandles from a previously bound VAO
    /// @param first The starting index in the bound VAO. 
    /// @param count The number of vertices to be rendered.
    auto draw_triangles(std::size_t first, std::size_t count) -> void;

    /// @brief Draws lines from a previously bound VAO
    /// @param first The starting index in the bound VAO. 
    /// @param count The number of vertices to be rendered.
    auto draw_lines(std::size_t first, std::size_t count) -> void;
    
    /// @brief Clears the screen
    /// @param color The screen clear's color
    /// @param depth The depth that should be erased from the depth buffer, usually 1.0
    /// @param clear_color Should it clear the screen color?
    /// @param clear_depth Should it clear the screen depth?
    auto clear_screen(const Color &color, double depth = 1.0, bool clear_color = true, bool clear_depth = true) -> void;

    /// @internal
    /// @return The draw list's data. Used internally.
    auto data() const -> const std::vector<std::byte> &;
    
    /// @internal
    /// @returns The required number of draw indirect commands. Used internally. 
    auto required_indirect_draw_cmds() const -> std::size_t;

private:
    std::vector<std::byte> data_;
    std::size_t req_indirect_draw_cmds{0};

    auto write_cmd_type(DrawCommandType type) -> void;

    template<typename T_>
    auto write(const T_ &value) -> void 
    {
        std::size_t offset = data_.size();
        data_.resize(offset + sizeof(T_));
        std::memcpy(data_.data() + offset, &value, sizeof(T_));
    }
};

} // namespace NoctisEngine::Rendering
