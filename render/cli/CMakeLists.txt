# render/cli/CMakeLists.txt
add_library(render_cli
  render_cli.c
  ansi.c
  frame_rate.c
)

target_include_directories(render_cli
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include
	${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(render_cli PUBLIC fluid_core)

