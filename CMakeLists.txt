cmake_minimum_required(VERSION 3.10)
project(noctis_engine VERSION 0.0.1)

set(CMAKE_CXX_STANDARD          23)
set(CMAKE_CXX_STANDARD_REQUIRED true)

option(NCENG_BUILD_SHARED   "Build as a shared lib" OFF)
option(NCENG_DEBUG          "Build as debug"        OFF)

add_subdirectory(thirdparty/glad)
add_subdirectory(thirdparty/glfw)
add_subdirectory(thirdparty/glm)
add_subdirectory(thirdparty/stb)
add_subdirectory(thirdparty/entt)
add_subdirectory(thirdparty/imgui)

set(NCENG_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_subdirectory(src)
