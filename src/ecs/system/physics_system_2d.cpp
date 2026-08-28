#include <noctis_engine/ecs/system/physics_system_2d.hpp>

#include <box2d/box2d.h>

#include <noctis_engine/rendering/default_shaders.hpp>


namespace NoctisEngine::ECS
{

Core::Logger PHYSICS_LOGGER{"Noctis Engine", "Physics"};

auto phys_mat_to_b2_surface_mat(const PhysicsMaterial2D &phys_mat) -> b2SurfaceMaterial;
auto glm_points_to_b2_points(const std::vector<glm::vec2> &glm_points) -> std::vector<b2Vec2>;
auto entity_from_shape(b2ShapeId shape_id) -> Entity;


PhysicsSystem2D::PhysicsSystem2D(std::shared_ptr<World> world)
    : world_{world}
    , accumulator_{0.0f}
{
    b2WorldDef world_def    = b2DefaultWorldDef();
    b2WorldId world_id      = b2CreateWorld(&world_def);
    physics_world_          = b2StoreWorldId(world_id);
}

PhysicsSystem2D::~PhysicsSystem2D()
{
    b2DestroyWorld(b2LoadWorldId(physics_world_));
}

auto PhysicsSystem2D::enable_debug_rendering() -> void
{
    line_vertex_array_ = Rendering::VertexArray{
        Rendering::DEBUG_VERTEX_ATTRIBUTES,
        "physics_line_vertex_array",
        true,
        true // false
    };

    tri_vertex_array_ = Rendering::VertexArray{
        Rendering::DEBUG_VERTEX_ATTRIBUTES,
        "physics_tri_vertex_array",
        true,
        true // false
    };

    dbg_shader_ = Rendering::Shader{
        Rendering::DefaultShaders::DEBUG_SHADER_2D.data(),
        "phys_shader"
    };
    dbg_shader_.compile();
}

auto PhysicsSystem2D::set_gravity(glm::vec2 gravity) -> void
{
    b2World_SetGravity(
        b2LoadWorldId(physics_world_), 
        b2Vec2{gravity.x, gravity.y}
    );
}

auto PhysicsSystem2D::set_hit_event_threshold(float threshold) -> void
{
    b2World_SetHitEventThreshold(
        b2LoadWorldId(physics_world_), 
        threshold
    );
}

auto PhysicsSystem2D::create_physics_entity(
    const std::vector<CollisionShape2D>    &collision_shapes,
    PhysicsBody2D::Type                     physics_body_type,
    const ECS::Transform2D                 &transform,
    std::string_view                        name
) -> Entity
{
    Entity entity = world_->create_entity();
    PhysicsBody2D physics_body{};

    b2BodyDef body_def = b2DefaultBodyDef();
    body_def.position = b2Vec2{
        transform.position().x * PIXELS_TO_METERS, 
        transform.position().y * PIXELS_TO_METERS
    };

    body_def.rotation = b2MakeRot(transform.rotation());
    body_def.type = static_cast<b2BodyType>(physics_body_type);
    body_def.name = name.data();
    body_def.userData = (void *)(std::uintptr_t)entity.id();

    b2BodyId body_id = b2CreateBody(b2LoadWorldId(physics_world_), &body_def);
    physics_body.body_id = b2StoreBodyId(body_id);

    for (const auto &collision_shape : collision_shapes)
    {
        b2ShapeDef shape_def = b2DefaultShapeDef();
        shape_def.density = collision_shape.density;
        shape_def.enableContactEvents = collision_shape.enable_collision_events;
        shape_def.enableHitEvents = collision_shape.enable_hit_events;
        shape_def.enableSensorEvents = collision_shape.is_sensor;
            
        shape_def.isSensor = collision_shape.is_sensor;
            
        shape_def.filter = b2Filter{ 
            .categoryBits = collision_shape.type_bit, 
            .maskBits = collision_shape.collision_mask_bits 
        };

        const PhysicsMaterial2D &phys_mat = collision_shape.physics_material;
        auto surface_mat = phys_mat_to_b2_surface_mat(phys_mat); 
        shape_def.material = surface_mat;

        collision_callbacks_.push_back(collision_shape.callbacks);
        shape_def.userData = &collision_callbacks_.back();

        bool is_chain = false;
        std::uint64_t shape_store;
        switch (collision_shape.shape_type)
        {
            case CollisionShape2D::ShapeType::BOX:
            {
                const auto &box_shape = std::get<CollisionShape2D::Box>(collision_shape.shape);

                b2Polygon box_polygon = b2MakeOffsetRoundedBox(
                    box_shape.half_extents.x * PIXELS_TO_METERS, box_shape.half_extents.y * PIXELS_TO_METERS, 
                    b2Vec2{box_shape.center.x * PIXELS_TO_METERS, box_shape.center.y * PIXELS_TO_METERS}, 
                    b2MakeRot(glm::radians(box_shape.rotation_deg)), 
                    box_shape.corner_radius * PIXELS_TO_METERS
                );

                b2ShapeId shape_id = b2CreatePolygonShape(body_id, &shape_def, &box_polygon);
                if (not b2Shape_IsValid(shape_id))
                {
                    PHYSICS_LOGGER.error("Failed to create box shape for rigidbody \"{}\"", name);
                    world_->destroy_entity(entity);
                    b2DestroyBody(body_id);

                    return Entity{};
                }

                shape_store = b2StoreShapeId(shape_id);

                break;
            }

            case CollisionShape2D::ShapeType::CAPSULE:
            {
                const auto &capsule_shape = std::get<CollisionShape2D::Capsule>(collision_shape.shape);

                b2Capsule capsule{
                    .center1 = b2Vec2{
                        capsule_shape.center_1.x * PIXELS_TO_METERS, 
                        capsule_shape.center_1.y * PIXELS_TO_METERS
                    },
                    .center2 = b2Vec2{
                        capsule_shape.center_2.x * PIXELS_TO_METERS, 
                        capsule_shape.center_2.y * PIXELS_TO_METERS
                    },
                    .radius = capsule_shape.radius * PIXELS_TO_METERS,
                };

                b2ShapeId  shape_id = b2CreateCapsuleShape(body_id, &shape_def, &capsule);
                if (not b2Shape_IsValid(shape_id))
                {
                    PHYSICS_LOGGER.error("Failed to create capsule shape for rigidbody \"{}\"", name);
                    world_->destroy_entity(entity);
                    b2DestroyBody(body_id);

                    return Entity{};
                }

                shape_store = b2StoreShapeId(shape_id);

                break;
            }

            case CollisionShape2D::ShapeType::CHAIN:
            {
                const auto &chain_shape = std::get<CollisionShape2D::Chain>(collision_shape.shape);

                std::vector<b2Vec2> b2_points = glm_points_to_b2_points(chain_shape.points);
                std::vector<b2SurfaceMaterial> surface_materials;
                
                if (chain_shape.physics_materials.empty())
                {
                    surface_materials.push_back(surface_mat);
                }
                else
                {
                    for (const auto &seg_phys_mat : chain_shape.physics_materials)
                        surface_materials.push_back(phys_mat_to_b2_surface_mat(seg_phys_mat));
                }

                b2ChainDef chain_def{
                    .userData = &collision_callbacks_.back(),
                    .points = b2_points.data(),
                    .count = static_cast<int>(b2_points.size()),
                    .materials = surface_materials.data(),
                    .materialCount = static_cast<int>(surface_materials.size()),
                    .filter = shape_def.filter,
                    .isLoop = chain_shape.is_loop,
                    .enableSensorEvents = chain_shape.enable_sensor_events,
                };

                b2ChainId chain_id = b2CreateChain(body_id, &chain_def);
                if (not b2Chain_IsValid(chain_id))
                {
                    PHYSICS_LOGGER.error("Failed to create chain shape for rigidbody \"{}\"", name);
                    world_->destroy_entity(entity);
                    b2DestroyBody(body_id);

                    return Entity{};
                }

                is_chain = true;
                shape_store = b2StoreChainId(chain_id);

                break;
            }

            case CollisionShape2D::ShapeType::CIRCLE:
            {
                const auto &circle_shape = std::get<CollisionShape2D::Circle>(collision_shape.shape);
                
                b2Circle circle{
                    .center = b2Vec2{
                        circle_shape.center.x * PIXELS_TO_METERS, 
                        circle_shape.center.y * PIXELS_TO_METERS
                    },
                    .radius = circle_shape.radius * PIXELS_TO_METERS,
                };

                b2ShapeId shape_id = b2CreateCircleShape(body_id, &shape_def, &circle);
                if (not b2Shape_IsValid(shape_id))
                {
                    PHYSICS_LOGGER.error("Failed to create circle shape for rigidbody \"{}\"", name);
                    world_->destroy_entity(entity);
                    b2DestroyBody(body_id);

                    return Entity{};
                }
                shape_store = b2StoreShapeId(shape_id);

                break;
            }

            case CollisionShape2D::ShapeType::POLYGON:
            {
                const auto &polygon_shape = std::get<CollisionShape2D::Polygon>(collision_shape.shape);
                
                std::vector<b2Vec2> b2_points = glm_points_to_b2_points(polygon_shape.points);
                b2Hull hull = b2ComputeHull(
                    b2_points.data(), 
                    b2_points.size()
                );

                if (hull.count <= 0)
                {
                    PHYSICS_LOGGER.error("Failed to create polygon shape for rigidbody \"{}\"", name);
                    world_->destroy_entity(entity);
                    b2DestroyBody(body_id);

                    return Entity{};
                }
                
                b2Polygon polygon = b2MakePolygon(&hull, polygon_shape.corner_radius * PIXELS_TO_METERS);

                b2ShapeId shape_id = b2CreatePolygonShape(body_id, &shape_def, &polygon);
                shape_store = b2StoreShapeId(shape_id);

                break;
            }

            case CollisionShape2D::ShapeType::SEGMENT:
            {
                const auto &seg_shape = std::get<CollisionShape2D::Segment>(collision_shape.shape);

                b2Segment seg{
                    .point1 = b2Vec2{
                        seg_shape.point_1.x * PIXELS_TO_METERS, 
                        seg_shape.point_1.y * PIXELS_TO_METERS
                    },
                    .point2 = b2Vec2{
                        seg_shape.point_2.x * PIXELS_TO_METERS, 
                        seg_shape.point_2.y * PIXELS_TO_METERS
                    },
                };

                b2ShapeId shape_id = b2CreateSegmentShape(body_id, &shape_def, &seg);
                if (not b2Shape_IsValid(shape_id))
                {
                    PHYSICS_LOGGER.error("Failed to create segment shape for rigidbody \"{}\"", name);
                    world_->destroy_entity(entity);
                    b2DestroyBody(body_id);

                    return Entity{};
                }

                shape_store = b2StoreShapeId(shape_id);

                break;
            }
        }

        PhysicsBody2D::CollisionShape coll_shape{};
        coll_shape.shape_id = shape_store;
        coll_shape.is_chain = is_chain;

        physics_body.collision_shapes.push_back(coll_shape);
    }

    world_->add_component(entity, physics_body);
    world_->add_component(entity, transform);

    physics_entities_.push_back(entity);

    return entity;
}

auto PhysicsSystem2D::sync_physics_engine_to_ecs() -> void
{
    for (const auto &e : physics_entities_)
    {
        auto e_transform = world_->get_component<Transform2D>(e);
        if (not e_transform)
            continue;

        auto e_pb = world_->get_component<PhysicsBody2D>(e);
        if (not e_pb)
            continue;

        if (e_pb->phys_body_type() != PhysicsBody2D::Type::KINEMATIC)
            continue;

        b2Body_SetTransform(
            b2LoadBodyId(e_pb->body_id), 
            b2Vec2{e_transform->position().x, e_transform->position().y}, 
            b2MakeRot(e_transform->rotation())
        );
    }
}

auto PhysicsSystem2D::update_physics(float dt, float time_step, std::uint16_t substep_count) -> void
{
    accumulator_ += dt;
    while (accumulator_ >= time_step)
    {
        b2World_Step(b2LoadWorldId(physics_world_), time_step, substep_count);
        accumulator_ -= time_step;
    }
}

auto PhysicsSystem2D::sync_ecs_to_physics_engine() -> void
{
    for (const auto &e : physics_entities_)
    {
        auto e_transform = world_->get_component<Transform2D>(e);
        if (not e_transform)
            continue;

        auto e_rb = world_->get_component<PhysicsBody2D>(e);
        if (not e_rb)
            continue;

        b2Transform body_transform = b2Body_GetTransform(
            b2LoadBodyId(e_rb->body_id) 
        );

        e_transform->set_position(glm::vec2{body_transform.p.x, body_transform.p.y});
        e_transform->set_rotation(b2Rot_GetAngle(body_transform.q));

        PHYSICS_LOGGER.debug("pos: {} {}, rot: {}", 
            body_transform.p.x, body_transform.p.y, 
            b2Rot_GetAngle(body_transform.q)
        );
    }

    process_contact_events();
    process_sensor_events();
}

auto PhysicsSystem2D::draw_debug(Rendering::DrawList &draw_list, const DebugDrawSettings &settings) -> void
{
    struct Context
    {
        std::vector<Rendering::DebugVertex> lines_vertices{};
        std::vector<Rendering::DebugVertex> tris_vertices{};
    } ctx;

    b2DebugDraw debug_draw = b2DefaultDebugDraw();
	debug_draw.drawingBounds = b2AABB{
        .lowerBound = b2Vec2{
            settings.lower_draw_bound.x * PIXELS_TO_METERS, 
            settings.lower_draw_bound.y * PIXELS_TO_METERS
        },
	    .upperBound = b2Vec2{
            settings.upper_draw_bound.x * PIXELS_TO_METERS, 
            settings.upper_draw_bound.y * PIXELS_TO_METERS
        },
    };

	debug_draw.forceScale           = settings.force_scale;
	debug_draw.jointScale           = settings.joint_scale;
	debug_draw.drawContacts         = settings.draw_contacts;
	debug_draw.drawAnchorA          = settings.draw_anchor_A;
	debug_draw.drawShapes           = settings.draw_shapes;
	debug_draw.drawChainNormals     = settings.draw_chain_normals;
	debug_draw.drawJoints           = settings.draw_joints;
	debug_draw.drawJointExtras      = settings.draw_joint_extras;
	debug_draw.drawBounds           = settings.draw_bounds;
	debug_draw.drawMass             = settings.draw_mass;
	debug_draw.drawBodyNames        = false;
	debug_draw.drawGraphColors      = false;
	debug_draw.drawContactFeatures  = false;
	debug_draw.drawContactNormals   = settings.draw_contact_normals;
	debug_draw.drawContactForces    = settings.draw_contact_forces;
	debug_draw.drawFrictionForces   = settings.draw_friction_forces;

	/// Option to draw islands as bounding boxes
	bool drawIslands;

    debug_draw.DrawPolygonFcn = [](
        b2WorldTransform    transform, 
        const b2Vec2       *vertices, 
        int                 vertexCount, 
        b2HexColor          color,
        void               *context
    ) -> void
    {
        auto ctx = reinterpret_cast<Context *>(context);

        glm::vec4 color_floats4 = RGBA_to_floats(static_cast<std::uint32_t>(color));
        glm::vec3 color_floats{ color_floats4.r, color_floats4.g, color_floats4.b };

        if (vertexCount < 3)
            return;

        for (int i = 0; i < vertexCount; i++)
        {
            // Wrap back around
            int next = (i + 1) % vertexCount;

            b2Vec2 world_curr = b2TransformPoint(transform, vertices[i]);
            b2Vec2 world_next = b2TransformPoint(transform, vertices[next]);

            ctx->lines_vertices.push_back(Rendering::DebugVertex{ 
                glm::vec3{ world_curr.x, world_curr.y, 0 } * METERS_TO_PIXELS, 
                color_floats
            });

            ctx->lines_vertices.push_back(Rendering::DebugVertex{ 
                glm::vec3{ world_next.x, world_next.y, 0 } * METERS_TO_PIXELS, 
                color_floats
            });
        }
    };

    debug_draw.DrawSolidPolygonFcn = [](
        b2WorldTransform    transform, 
        const b2Vec2       *vertices, 
        int                 vertexCount, 
        float               radius,
		b2HexColor          color, 
        void               *context
    ) -> void
    {
        auto ctx = reinterpret_cast<Context *>(context);
        
        glm::vec4 color_floats4 = RGBA_to_floats(static_cast<std::uint32_t>(color));
        glm::vec3 color_floats{ color_floats4.r, color_floats4.g, color_floats4.b };

        if (vertexCount < 3)
            return;

        std::vector<glm::vec3> world_verts{};
        world_verts.resize(vertexCount);

        for (int i = 0; i < vertexCount; i++)
        {
            b2Vec2 world = b2TransformPoint(transform, vertices[i]);
            world_verts[i] = glm::vec3{ world.x, world.y, 0 } * METERS_TO_PIXELS;
        }

        for (int i = 1; i + 1 < vertexCount; i++)
        {
            ctx->tris_vertices.push_back(Rendering::DebugVertex{ world_verts[0],     color_floats });
            ctx->tris_vertices.push_back(Rendering::DebugVertex{ world_verts[i],     color_floats });
            ctx->tris_vertices.push_back(Rendering::DebugVertex{ world_verts[i + 1], color_floats });
        }
    };

    debug_draw.DrawCircleFcn = [](
        b2Pos       center, 
        float       radius, 
        b2HexColor  color, 
        void       *context
    ) -> void
    {
        auto ctx = reinterpret_cast<Context *>(context);

        glm::vec4 color_floats4 = RGBA_to_floats(static_cast<std::uint32_t>(color));
        glm::vec3 color_floats{ color_floats4.r, color_floats4.g, color_floats4.b };

        constexpr int SEGMENTS = 24;
        constexpr float TAU = glm::tau<float>();

        for (int i = 0; i < SEGMENTS; i++)
        {
            float angle_a = TAU * (float)i / SEGMENTS;
            glm::vec3 point_a{
                center.x + radius * glm::cos(angle_a), 
                center.y + radius * glm::sin(angle_a), 0 
            };

            float angle_b = TAU * (float)(i + 1) / SEGMENTS;
            glm::vec3 point_b{
                center.x + radius * glm::cos(angle_b), 
                center.y + radius * glm::sin(angle_b), 0 
            };

            ctx->lines_vertices.push_back(Rendering::DebugVertex{ point_a * METERS_TO_PIXELS, color_floats });
            ctx->lines_vertices.push_back(Rendering::DebugVertex{ point_b * METERS_TO_PIXELS, color_floats });
        }
    };

    debug_draw.DrawSolidCircleFcn = [](
        b2WorldTransform    transform, 
        b2Vec2              center, 
        float               radius, 
        b2HexColor          color, 
        void               *context
    ) -> void
    {
        auto ctx = reinterpret_cast<Context *>(context);

        glm::vec4 color_floats4 = RGBA_to_floats(static_cast<std::uint32_t>(color));
        glm::vec3 color_floats{ color_floats4.r, color_floats4.g, color_floats4.b };

        constexpr int SEGMENTS = 24;
        constexpr float TAU = glm::tau<float>();

        b2Vec2 center_pos = transform.p;

        std::vector<glm::vec3> rim(SEGMENTS);
        for (int i = 0; i < SEGMENTS; i++)
        {
            float angle = TAU * (float)i / SEGMENTS;

            b2Vec2 local{ 
                radius * glm::cos(angle), 
                radius * glm::sin(angle) 
            };

            b2Vec2 world = b2TransformPoint(transform, local);
            
            rim[i] = glm::vec3{ world.x, world.y, 0 } * METERS_TO_PIXELS;
        }

        glm::vec3 center_world{ center_pos.x, center_pos.y, 0 };
        center_world *= METERS_TO_PIXELS;

        for (int i = 0; i < SEGMENTS; i++)
        {
            int next = (i + 1) % SEGMENTS;

            ctx->tris_vertices.push_back(Rendering::DebugVertex{ center_world, color_floats });
            ctx->tris_vertices.push_back(Rendering::DebugVertex{ rim[i],       color_floats });
            ctx->tris_vertices.push_back(Rendering::DebugVertex{ rim[next],    color_floats });
        }

        // Draw a line so its easier to see rotation
        ctx->lines_vertices.push_back(Rendering::DebugVertex{ center_world, color_floats });
        ctx->lines_vertices.push_back(Rendering::DebugVertex{ rim[0],       color_floats });
    };

    debug_draw.DrawSolidCapsuleFcn = [](
        b2Vec2      p1, 
        b2Vec2      p2, 
        float       radius, 
        b2HexColor  color, 
        void       *context
    ) -> void
    {
        auto ctx = reinterpret_cast<Context *>(context);

        glm::vec4 color_floats4 = RGBA_to_floats(static_cast<std::uint32_t>(color));
        glm::vec3 color_floats{ color_floats4.r, color_floats4.g, color_floats4.b };

        b2Vec2 axis = { p2.x - p1.x, p2.y - p1.y };
        float length = glm::sqrt(axis.x * axis.x + axis.y * axis.y);

        if (length < 1e-6f)
            return;

        float base_angle = glm::atan(axis.y, axis.x);
        constexpr int CAP_SEGMENTS = 12;
        constexpr float PI = glm::pi<float>();

        std::vector<glm::vec3> outline;
        outline.reserve(CAP_SEGMENTS * 2 + 2);

        for (int i = 0; i <= CAP_SEGMENTS; i++)
        {
            float t = (float)i / CAP_SEGMENTS;
            float angle = base_angle - PI * 0.5f + PI * t;

            outline.push_back(glm::vec3{
                p2.x + radius * glm::cos(angle),
                p2.y + radius * glm::sin(angle),
                0
            } * METERS_TO_PIXELS);
        }

        for (int i = 0; i <= CAP_SEGMENTS; i++)
        {
            float t = (float)i / CAP_SEGMENTS;
            float angle = base_angle + PI * 0.5f + PI * t;

            outline.push_back(glm::vec3{
                p1.x + radius * glm::cos(angle),
                p1.y + radius * glm::sin(angle),
                0
            } * METERS_TO_PIXELS);
        }

        glm::vec3 hub{ (p1.x + p2.x) * 0.5f, (p1.y + p2.y) * 0.5f, 0 };
        hub *= METERS_TO_PIXELS;

        int n = (int)outline.size();
        for (int i = 0; i < n; i++)
        {
            int next = (i + 1) % n;
            ctx->tris_vertices.push_back(Rendering::DebugVertex{ hub,            color_floats });
            ctx->tris_vertices.push_back(Rendering::DebugVertex{ outline[i],     color_floats });
            ctx->tris_vertices.push_back(Rendering::DebugVertex{ outline[next],  color_floats });
        }
    };

    debug_draw.DrawLineFcn = [](
        b2Vec2      p1, 
        b2Vec2      p2, 
        b2HexColor  color, 
        void       *context
    ) -> void
    {
        auto ctx = reinterpret_cast<Context *>(context);

        glm::vec4 color_floats4 = RGBA_to_floats(static_cast<std::uint32_t>(color));
        glm::vec3 color_floats{ color_floats4.r, color_floats4.g, color_floats4.b };

        ctx->lines_vertices.push_back(Rendering::DebugVertex{ 
            glm::vec3{ p1.x, p1.y, 0  } * METERS_TO_PIXELS, 
            color_floats 
        });

        ctx->lines_vertices.push_back(Rendering::DebugVertex{ 
            glm::vec3{ p2.x, p2.y, 0  } * METERS_TO_PIXELS, 
            color_floats 
        });
    };

    debug_draw.DrawBoundsFcn = [](
        b2AABB      bounds, 
        b2HexColor  color, 
        void       *context
    ) -> void
    {
        auto ctx = reinterpret_cast<Context *>(context);

        glm::vec4 color_floats4 = RGBA_to_floats(static_cast<std::uint32_t>(color));
        glm::vec3 color_floats{ color_floats4.r, color_floats4.g, color_floats4.b };

        constexpr int NUM_CORNERS = 4;

        glm::vec3 corners[NUM_CORNERS] = {
            glm::vec3{ bounds.lowerBound.x, bounds.lowerBound.y, 0 } * METERS_TO_PIXELS,
            glm::vec3{ bounds.upperBound.x, bounds.lowerBound.y, 0 } * METERS_TO_PIXELS,
            glm::vec3{ bounds.upperBound.x, bounds.upperBound.y, 0 } * METERS_TO_PIXELS,
            glm::vec3{ bounds.lowerBound.x, bounds.upperBound.y, 0 } * METERS_TO_PIXELS,
        };

        for (int i = 0; i < NUM_CORNERS; i++)
        {
            int next = (i + 1) % NUM_CORNERS;
            ctx->lines_vertices.push_back(Rendering::DebugVertex{ corners[i],    color_floats });
            ctx->lines_vertices.push_back(Rendering::DebugVertex{ corners[next], color_floats });
        }
    };

    debug_draw.context = &ctx;

    b2World_Draw(b2LoadWorldId(physics_world_), &debug_draw);

    line_vertex_array_.upload_vertices(
        ctx.lines_vertices.data(), 
        ctx.lines_vertices.size() * sizeof(Rendering::DebugVertex), 
        sizeof(Rendering::DebugVertex)
    ); 

    tri_vertex_array_.upload_vertices(
        ctx.tris_vertices.data(), 
        ctx.tris_vertices.size() * sizeof(Rendering::DebugVertex), 
        sizeof(Rendering::DebugVertex)
    );

    dbg_shader_.use(draw_list);
    
    line_vertex_array_.use(draw_list);
    draw_list.draw_lines(0, ctx.lines_vertices.size());

    tri_vertex_array_.use(draw_list);
    draw_list.draw_lines(0, ctx.tris_vertices.size());
}

auto PhysicsSystem2D::process_contact_events() -> void
{
    b2ContactEvents events = b2World_GetContactEvents(b2LoadWorldId(physics_world_));

    for (std::size_t i = 0; i < events.beginCount; i++)
    {
        const b2ContactBeginTouchEvent &ev = events.beginEvents[i];
        if (!b2Shape_IsValid(ev.shapeIdA) || !b2Shape_IsValid(ev.shapeIdB)) 
            continue;

        auto cb = reinterpret_cast<CollisionShape2D::Callbacks *>(b2Shape_GetUserData(ev.shapeIdA));
        cb->on_collision_begin(
            entity_from_shape(ev.shapeIdA), 
            entity_from_shape(ev.shapeIdB)
        );
    }

    for (std::size_t i = 0; i < events.endCount; i++)
    {
        const b2ContactEndTouchEvent &ev = events.endEvents[i];
        if (!b2Shape_IsValid(ev.shapeIdA) || !b2Shape_IsValid(ev.shapeIdB)) 
            continue;

        auto cb = reinterpret_cast<CollisionShape2D::Callbacks *>(b2Shape_GetUserData(ev.shapeIdA));
        cb->on_collision_end(
            entity_from_shape(ev.shapeIdA), 
            entity_from_shape(ev.shapeIdB)
        );
    }

    for (std::size_t i = 0; i < events.hitCount; i++)
    {
        const b2ContactHitEvent &ev = events.hitEvents[i];
        if (!b2Shape_IsValid(ev.shapeIdA) || !b2Shape_IsValid(ev.shapeIdB)) 
            continue;

        auto cb = reinterpret_cast<CollisionShape2D::Callbacks *>(b2Shape_GetUserData(ev.shapeIdA));
        cb->on_collision_end(
            entity_from_shape(ev.shapeIdA), 
            entity_from_shape(ev.shapeIdB)
        );
    }
}

auto PhysicsSystem2D::process_sensor_events() -> void
{
    b2SensorEvents events = b2World_GetSensorEvents(b2LoadWorldId(physics_world_));
    
    for (std::size_t i = 0; i < events.beginCount; i++)
    {
        const b2SensorBeginTouchEvent &ev = events.beginEvents[i];
        if (!b2Shape_IsValid(ev.sensorShapeId) || !b2Shape_IsValid(ev.visitorShapeId)) 
            continue;

        auto cb = reinterpret_cast<CollisionShape2D::Callbacks *>(b2Shape_GetUserData(ev.sensorShapeId));
        cb->on_sensor_begin_touch(
            entity_from_shape(ev.sensorShapeId), 
            entity_from_shape(ev.visitorShapeId)
        );
    }

    for (std::size_t i = 0; i < events.endCount; i++)
    {
        const b2SensorEndTouchEvent &ev = events.endEvents[i];
        if (!b2Shape_IsValid(ev.sensorShapeId) || !b2Shape_IsValid(ev.visitorShapeId)) 
            continue;

        auto cb = reinterpret_cast<CollisionShape2D::Callbacks *>(b2Shape_GetUserData(ev.sensorShapeId));
        cb->on_sensor_end_touch(
            entity_from_shape(ev.sensorShapeId), 
            entity_from_shape(ev.visitorShapeId)
        );
    }
}

auto phys_mat_to_b2_surface_mat(const PhysicsMaterial2D &phys_mat) -> b2SurfaceMaterial
{
    return b2SurfaceMaterial{
        .friction = phys_mat.friction,
        .restitution = phys_mat.bounciness,
        .rollingResistance = phys_mat.rolling_resistance,
        .tangentSpeed = phys_mat.tangent_speed,
        .customColor = phys_mat.debug_draw_color.to_RGBA_int(),
    };
}

auto glm_points_to_b2_points(const std::vector<glm::vec2> &glm_points) -> std::vector<b2Vec2>
{
    std::vector<b2Vec2> b2_points{};
    b2_points.reserve(glm_points.size());

    for (const auto &glm_point : glm_points)
        b2_points.push_back(b2Vec2{glm_point.x * PIXELS_TO_METERS, glm_point.y * PIXELS_TO_METERS});

    return b2_points;
}

auto entity_from_shape(b2ShapeId shape_id) -> Entity
{
    b2BodyId body_id = b2Shape_GetBody(shape_id);
    // my bad
    return *reinterpret_cast<Entity *>(b2Body_GetUserData(body_id));
}

} // namespace NoctisEngine::ECS
