cmake_minimum_required(VERSION 3.16)
project(TraceStudio)

# 使用 C++17
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置源文件
set(IMGUI_DIR ..)

set(SOURCES
    main.cpp
    gui/gui_init.cpp
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

# 包含头文件
include_directories(
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
)

# 查找 GLFW
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW REQUIRED glfw3)

# 添加可执行文件
add_executable(${PROJECT_NAME} ${SOURCES})

# 链接库，一定在 add_executable 之后
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW REQUIRED glfw3)

target_link_libraries(${PROJECT_NAME} PRIVATE
    GL        # OpenGL 库
    ${GLFW_LIBRARIES}
)

# 包含 GLFW 的 include 路径
target_include_directories(${PROJECT_NAME} PRIVATE ${GLFW_INCLUDE_DIRS})

# 编译选项
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -g)
