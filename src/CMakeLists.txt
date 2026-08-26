cmake_minimum_required(VERSION 3.10)

set(NCENG_SOURCES
    "asset/loaders.cpp"
    "core/window.cpp"
    "ecs/component/transform.cpp"
    "rendering/mesh/mesh_manager.cpp"
    "rendering/camera_2d.cpp"
    "rendering/draw_list.cpp"
    "rendering/gpu_buffer.cpp"
    "rendering/renderer.cpp"
    "rendering/shader.cpp"
    "rendering/texture.cpp"
)


if(NCENG_BUILD_SHARED)
    message("Building shared noctis_engine library")
    add_library(noctis_engine SHARED ${NCENG_SOURCES})
else()
    message("Building static noctis_engine library")
    add_library(noctis_engine STATIC ${NCENG_SOURCES})
endif()

if (NCENG_DEBUG)
    message("Building noctis_engine as debug")
    add_compile_definitions(noctis_engine PUBLIC NCENG_DEBUG)
endif()

target_compile_definitions(noctis_engine PUBLIC GLM_ENABLE_EXPERIMENTAL)

add_library(noctis_engine::noctis_engine ALIAS noctis_engine)

target_link_libraries(noctis_engine
    EnTT::EnTT
    glm::glm
    ImGui
    glfw
    Glad
    stb
    stdc++exp
)

target_include_directories(noctis_engine
    PUBLIC
        ${NCENG_INCLUDE_DIR}
    PRIVATE
        ${NCENG_INCLUDE_DIR}/noctis_engine
)