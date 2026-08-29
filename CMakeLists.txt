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

# 3. Build Dear ImGui (+ GLFW/OpenGL3 backends) as its own static library.
#    Compiles once, stays out of our own warning flags, and keeps the app
#    target's source list down to just our code.
add_library(imgui STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

# PUBLIC so these propagate to anything linking imgui (i.e. the app), which
# also needs the imgui headers.
target_include_directories(imgui PUBLIC
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)

# PUBLIC so glfw's headers reach the glfw backend (needs them to compile) and
# our app (transitively linked, so we don't list glfw again below).
target_link_libraries(imgui PUBLIC glfw)

# 4. Define the executable target — just our own sources now.
add_executable(modul_app
    src/main.cpp
    src/core/audio_engine.cpp
    src/core/tape.cpp
    src/gui/main_view.cpp
    src/gui/device_view.cpp
    src/gui/scope_view.cpp
)

# 5. Set include paths (imgui's come through the linked target)
target_include_directories(modul_app PRIVATE
    include
)

# 6. Windows: stop windows.h from defining min/max macros (breaks std::min)
if(WIN32)
    target_compile_definitions(modul_app PRIVATE NOMINMAX WIN32_LEAN_AND_MEAN)
    # The MSVC STL hard-errors (STL1000) when parsed by Clang older than 19.
    # MSVC itself never trips this check, so the define is inert for the real
    # build; it exists so clang-tidy/clangd can read compile_commands.json.
    # Drop it once LLVM here is 19 or newer.
    target_compile_definitions(modul_app PRIVATE _ALLOW_COMPILER_AND_STL_VERSION_MISMATCH)
endif()

# 7. Link libraries cross-platform (glfw arrives transitively via imgui)
if(APPLE)
    target_link_libraries(modul_app PRIVATE imgui "-framework OpenGL" "-framework Cocoa" "-framework IOKit" "-framework CoreVideo")
elseif(WIN32)
    target_link_libraries(modul_app PRIVATE imgui opengl32)
else()
    target_link_libraries(modul_app PRIVATE imgui GL pthread dl)
endif()