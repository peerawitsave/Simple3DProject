cmake_minimum_required(VERSION 3.16)
project(Simple3DProject)

set(CMAKE_CXX_STANDARD 17)

# Set vcpkg toolchain if not auto-detected
if(DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

# Find packages
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

include_directories(
    ${CMAKE_SOURCE_DIR}/third_party/imgui
    ${CMAKE_SOURCE_DIR}/third_party/imgui/backends
)


# Source files
add_executable(Simple3DProject
    main.cpp
    Mesh.cpp

    # ImGui Core
    third_party/imgui/imgui.cpp
    third_party/imgui/imgui_draw.cpp
    third_party/imgui/imgui_tables.cpp
    third_party/imgui/imgui_widgets.cpp

    # ImGui Backends
    third_party/imgui/backends/imgui_impl_glfw.cpp
    third_party/imgui/backends/imgui_impl_opengl3.cpp
)

# Link libraries
target_link_libraries(Simple3DProject PRIVATE
    OpenGL::GL
    GLEW::GLEW
    glfw
    glm::glm
)

add_custom_command(TARGET Simple3DProject POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/shaders
    $<TARGET_FILE_DIR:Simple3DProject>/shaders
)


find_package(imgui CONFIG REQUIRED)

target_link_libraries(Simple3DProject PRIVATE imgui::imgui)
