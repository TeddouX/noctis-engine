#include <gtest/gtest.h>

#include <noctis_engine/ecs/ecs_world.hpp>


using namespace NoctisEngine;


struct TestComponent {};


TEST(ECSTests, AddComponentTest) 
{
    ECS::World world{};
    ECS::Entity e = world.create_entity();
    world.add_component(e, TestComponent{});

    ASSERT_TRUE(world.has_component<TestComponent>(e));
}

TEST(ECSTests, GetComponentTest) 
{
    ECS::World world{};
    ECS::Entity e = world.create_entity();
    world.add_component(e, TestComponent{});

    ASSERT_TRUE(world.get_component<TestComponent>(e) != nullptr);
}

TEST(ECSTests, RemoveComponent) 
{
    ECS::World world{};
    ECS::Entity e = world.create_entity();
    world.add_component(e, TestComponent{});

    ASSERT_TRUE(world.has_component<TestComponent>(e));
    world.remove_component<TestComponent>(e);
    ASSERT_FALSE(world.has_component<TestComponent>(e));
}

TEST(ECSTests, RemoveComponentTestPopAndReplace) 
{
    ECS::World world{};
    ECS::Entity e = world.create_entity();
    world.add_component(e, TestComponent{});
    world.add_component(world.create_entity(), TestComponent{});

    ASSERT_TRUE(world.has_component<TestComponent>(e));
    world.remove_component<TestComponent>(e);
    ASSERT_FALSE(world.has_component<TestComponent>(e));
}

TEST(ECSTests, ReusesIDs) 
{
    ECS::World world{};
    ECS::Entity e = world.create_entity();
    ASSERT_EQ(e.id(), 1);

    world.destroy_entity(e);

    ECS::Entity e1 = world.create_entity();

    ASSERT_EQ(e1.id(), 1);
}
