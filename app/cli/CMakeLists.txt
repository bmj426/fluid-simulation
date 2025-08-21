# app/cli/CMakeLists.txt
add_executable(fluid_cli
  main_cli.c
)

target_include_directories(fluid_cli
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include
)

target_link_libraries(fluid_cli PRIVATE render_cli)

