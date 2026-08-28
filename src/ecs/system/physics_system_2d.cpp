#include <noctis_engine/ecs/system/physics_system_2d.hpp>

#include <box2d/box2d.h>


namespace NoctisEngine::ECS
{

Core::Logger PHYSICS_LOGGER{"Noctis Engine", "Physics"};

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
        b2_points.push_back(b2Vec2{glm_point.x, glm_point.y});

    return b2_points;
}

auto entity_from_shape(b2ShapeId shape_id) -> Entity
{
    b2BodyId body_id = b2Shape_GetBody(shape_id);
    // my bad
    return *reinterpret_cast<Entity *>(b2Body_GetUserData(body_id));
}


PhysicsSystem2D::PhysicsSystem2D(std::shared_ptr<World> world)
    : world_{world}
{
    b2WorldDef world_def    = b2DefaultWorldDef();
    b2WorldId world_id      = b2CreateWorld(&world_def);
    physics_world_          = b2StoreWorldId(world_id);
}

PhysicsSystem2D::~PhysicsSystem2D()
{
    b2DestroyWorld(b2LoadWorldId(physics_world_));
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
    const std::vector<CollisionShape2D> &collision_shapes, 
    const ECS::Transform2D &transform,
    std::string_view name
) -> Entity
{
    Entity entity = world_->create_entity();
    PhysicsBody2D physics_body{};

    b2BodyDef body_def = b2DefaultBodyDef();
    body_def.position = b2Vec2{transform.position().x, transform.position().y};
    body_def.rotation = b2MakeRot(transform.rotation());
    body_def.name = name.data();
    body_def.userData = (void *)entity.id();

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
                    box_shape.half_extents.x, box_shape.half_extents.y, 
                    b2Vec2{box_shape.center.x, box_shape.center.y}, 
                    b2MakeRot(glm::radians(box_shape.rotation_deg)), 
                    box_shape.corner_radius
                );

                b2ShapeId shape_id = b2CreatePolygonShape(body_id, &shape_def, &box_polygon);
                if (not b2Shape_IsValid(shape_id))
                {
                    PHYSICS_LOGGER.error("Failed to create box shape for rigidbody \"{}\"", name);
                    world_->destroy_entity(entity);

                    return Entity{};
                }

                shape_store = b2StoreShapeId(shape_id);

                break;
            }

            case CollisionShape2D::ShapeType::CAPSULE:
            {
                const auto &capsule_shape = std::get<CollisionShape2D::Capsule>(collision_shape.shape);

                b2Capsule capsule{
                    .center1 = b2Vec2{capsule_shape.center_1.x, capsule_shape.center_1.y},
                    .center2 = b2Vec2{capsule_shape.center_2.x, capsule_shape.center_2.y},
                    .radius = capsule_shape.radius,
                };

                b2ShapeId  shape_id = b2CreateCapsuleShape(body_id, &shape_def, &capsule);
                if (not b2Shape_IsValid(shape_id))
                {
                    PHYSICS_LOGGER.error("Failed to create capsule shape for rigidbody \"{}\"", name);
                    world_->destroy_entity(entity);

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
                    .count = b2_points.size(),
                    .materials = surface_materials.data(),
                    .materialCount = surface_materials.size(),
                    .filter = shape_def.filter,
                    .isLoop = chain_shape.is_loop,
                    .enableSensorEvents = chain_shape.enable_sensor_events,
                };

                b2ChainId chain_id = b2CreateChain(body_id, &chain_def);
                if (not b2Chain_IsValid(chain_id))
                {
                    PHYSICS_LOGGER.error("Failed to create chain shape for rigidbody \"{}\"", name);
                    world_->destroy_entity(entity);

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
                    .center = b2Vec2{circle_shape.center.x, circle_shape.center.y},
                    .radius = circle_shape.radius,
                };

                b2ShapeId shape_id = b2CreateCircleShape(body_id, &shape_def, &circle);
                if (not b2Shape_IsValid(shape_id))
                {
                    PHYSICS_LOGGER.error("Failed to create circle shape for rigidbody \"{}\"", name);
                    world_->destroy_entity(entity);

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

                    return Entity{};
                }
                
                b2Polygon polygon = b2MakePolygon(&hull, polygon_shape.corner_radius);

                b2ShapeId shape_id = b2CreatePolygonShape(body_id, &shape_def, &polygon);
                shape_store = b2StoreShapeId(shape_id);

                break;
            }

            case CollisionShape2D::ShapeType::SEGMENT:
            {
                const auto &seg_shape = std::get<CollisionShape2D::Segment>(collision_shape.shape);

                b2Segment seg{
                    .point1 = b2Vec2{seg_shape.point_1.x, seg_shape.point_1.y},
                    .point2 = b2Vec2{seg_shape.point_2.x, seg_shape.point_2.y},
                };

                b2ShapeId shape_id = b2CreateSegmentShape(body_id, &shape_def, &seg);
                if (not b2Shape_IsValid(shape_id))
                {
                    PHYSICS_LOGGER.error("Failed to create segment shape for rigidbody \"{}\"", name);
                    world_->destroy_entity(entity);

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

        b2Body_SetTransform(
            b2LoadBodyId(e_pb->body_id), 
            b2Vec2{e_transform->position().x, e_transform->position().y}, 
            b2MakeRot(e_transform->rotation())
        );
    }
}

auto PhysicsSystem2D::update_physics(float dt, float time_step, std::uint16_t substep_count) -> void
{
    float acc = dt;
    while (acc >= time_step)
    {
        b2World_Step(b2LoadWorldId(physics_world_), time_step, substep_count);
        acc -= time_step;
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
    }

    process_contact_events();
    process_sensor_events();
}

auto PhysicsSystem2D::draw_debug(Rendering::DrawList &draw_list) -> void
{
    // TODO
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

} // namespace NoctisEngine::ECS
