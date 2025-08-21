# render/vk/CMakeLists.txt
cmake_minimum_required(VERSION 3.20)

# Vulkan
find_package(Vulkan REQUIRED)

# GLFW (FetchContent로 자동 다운로드/빌드)
include(FetchContent)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG        3.4
)
FetchContent_MakeAvailable(glfw)

add_library(render_vk
  render_vk.cpp
  vk_app.cpp
)

target_include_directories(render_vk
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include
)

target_link_libraries(render_vk
  PUBLIC
    fluid_core
    glfw
    Vulkan::Vulkan
)

# 셰이더는 빌드시 컴파일하고 싶다면 add_custom_command로 glslc 호출하거나,
# runtime 로드한다면 assets 경로만 복사하세요.

