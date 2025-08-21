# app/vulkan/CMakeLists.txt
add_executable(fluid_vk
  main_vk.cpp
)

target_include_directories(fluid_vk
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include
)

target_link_libraries(fluid_vk PRIVATE render_vk)

