cmake_minimum_required(VERSION 3.20)
project(fluid LANGUAGES C CXX)

# ---- 옵션 ----
option(BUILD_CLI "Build ASCII/CLI renderer app" ON)
option(BUILD_VULKAN "Build Vulkan renderer app" OFF)  # 필요할 때 ON
option(ENABLE_WARNINGS "Enable extra warnings" ON)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---- 경고 설정 (선택) ----
if (ENABLE_WARNINGS)
  if (MSVC)
    add_compile_options(/W4 /permissive-)
  else()
    add_compile_options(-Wall -Wextra -Wpedantic)
  endif()
endif()

# ---- 서브디렉토리 ----
add_subdirectory(include)   # 헤더 설치용(선택)
add_subdirectory(src)       # fluid_core 라이브러리
if (BUILD_CLI)
  add_subdirectory(render/cli)
  add_subdirectory(app/cli)
endif()

if (BUILD_VULKAN)
  add_subdirectory(render/vulkan)   # 의존성 자동 처리 포함
  add_subdirectory(app/vk)
endif()

# ---- 설치 규칙(선택) ----
include(GNUInstallDirs)
install(TARGETS fluid_core
        EXPORT fluidTargets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# 패키지 구성(선택): find_package(fluid CONFIG) 지원하고 싶다면 아래 구성 패키징 추가

