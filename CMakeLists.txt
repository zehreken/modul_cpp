cmake_minimum_required(VERSION 3.20)
project(ModulCpp LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Include package manager script
include(cmake/CPM.cmake)

# 2. Fetch external dependencies
CPMAddPackage(
    NAME glfw
    GITHUB_REPOSITORY glfw/glfw
    GIT_TAG 3.3.8
)

CPMAddPackage(
    NAME imgui
    GITHUB_REPOSITORY ocornut/imgui
    GIT_TAG v1.92.2
)

# 3. Define the executable target
add_executable(modul_app 
    src/main.cpp
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

# 4. Set include paths
target_include_directories(modul_app PRIVATE 
    include
    ${imgui_SOURCE_DIR} 
    ${imgui_SOURCE_DIR}/backends
)

# 5. Link libraries cross-platform
if(APPLE)
    target_link_libraries(modul_app PRIVATE glfw "-framework OpenGL" "-framework Cocoa" "-framework IOKit" "-framework CoreVideo")
elseif(WIN32)
    target_link_libraries(modul_app PRIVATE glfw opengl32)
else()
    target_link_libraries(modul_app PRIVATE glfw GL pthread dl)
endif()