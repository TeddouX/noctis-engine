cmake_minimum_required(VERSION 4.1)
project(noctis_engine VERSION 0.0.1)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED true)

add_subdirectory(thirdparty/entt)
add_subdirectory(thirdparty/glad)
add_subdirectory(thirdparty/glfw)
add_subdirectory(thirdparty/glm)
add_subdirectory(thirdparty/stb)
add_subdirectory(thirdparty/imgui)

add_subdirectory(src)
