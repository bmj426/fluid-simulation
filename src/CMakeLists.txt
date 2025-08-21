# src/CMakeLists.txt
add_library(fluid_core
  fluid_cpu.c
  fluid_utils.c
)

target_include_directories(fluid_core
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

# 수치계산용이니 m 링크 필요할 수도 있음 (Linux 계열)
if (UNIX AND NOT APPLE)
  target_link_libraries(fluid_core PUBLIC m)
endif()

