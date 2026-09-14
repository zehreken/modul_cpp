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

# glm — vector/matrix math, GLSL-shaped API. Exports a glm::glm target of its
# own, so unlike the others it needs no wrapping below.
#
# GLM_BUILD_LIBRARY OFF: glm defaults to also compiling a static lib, which is
# only of use if you opt into its extern-template build. We use it header-only,
# and with this off glm::glm is a pure INTERFACE target — nothing to compile.
CPMAddPackage(
    NAME glm
    GITHUB_REPOSITORY g-truc/glm
    GIT_TAG 1.0.3
    OPTIONS "GLM_BUILD_LIBRARY OFF"
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

# 4. Build glad (vendored GL loader) as its own target, so this generated C
#    stays out of our own source tree and compile flags.
add_library(glad STATIC external/glad/src/glad.c)

# PUBLIC so <glad/glad.h> (and the <KHR/khrplatform.h> it includes) reach
# anything linking glad, i.e. the app.
target_include_directories(glad PUBLIC external/glad/include)

# glad dlopen()s libGL on Unix; empty on Windows, where it uses LoadLibraryW.
target_link_libraries(glad PRIVATE ${CMAKE_DL_LIBS})

# 4b. dj_fft (vendored, header-only FFT). INTERFACE: nothing to compile, it
#     just hands its include path to whoever links it.
#
#     Note we deliberately never define DJ_FFT_IMPLEMENTATION — that section
#     is only for dj_fft's GPU path, and it typedefs GLchar/GLenum/... itself,
#     which would clash with glad. The CPU fft1d/2d/3d templates are fully
#     defined in the header and need no such define.
add_library(dj_fft INTERFACE)

# The header is included as <dj_fft/dj_fft.h>, so external/ is the search root.
target_include_directories(dj_fft INTERFACE external)

# 5. Embed GLSL sources into generated headers.
#
# C++20 has no equivalent of Rust's include_str! — that arrives as #embed in
# C++26/C23, which MSVC doesn't ship yet — so the build does it by hand: each
# shader file is read and written back out as a header holding one raw string
# literal, e.g. shaders::basic_vert.
#
# This runs at configure time, and CMAKE_CONFIGURE_DEPENDS registers the shader
# as an input to the build system itself, so touching a .vert/.frag makes CMake
# re-run (regenerating the header) before the next build. Costs a reconfigure
# per shader edit; buys not needing a custom command and a generator script.
function(embed_text SRC VAR)
    file(READ "${CMAKE_CURRENT_SOURCE_DIR}/${SRC}" CONTENT)
    # Note there's no newline between R"GLSL( and the content: GLSL requires
    # #version to be the first thing in the shader. The )GLSL" delimiter can't
    # collide with anything — GLSL has no string literals, so the source can
    # never contain a double quote.
    file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/generated/shaders/${VAR}.hpp"
"#pragma once
// Generated from ${SRC} - do not edit.
namespace shaders {
inline constexpr char ${VAR}[] = R\"GLSL(${CONTENT})GLSL\";
}
")
    set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS
                 "${CMAKE_CURRENT_SOURCE_DIR}/${SRC}")
endfunction()

embed_text(shaders/basic.vert basic_vert)
embed_text(shaders/basic.frag basic_frag)

# 6. Define the executable target — just our own sources now.
add_executable(modul_app
    src/main.cpp
    src/core/audio_engine.cpp
    src/core/tape.cpp
    src/gui/main_view.cpp
    src/gui/device_view.cpp
    src/gui/scope_view.cpp
    src/gui/mixer_view.cpp
    src/gui/metronome_view.cpp
    src/renderer/scene.cpp
    src/core/metronome.cpp
    src/gui/render_view.cpp
)

# 7. Set include paths (imgui's come through the linked target)
target_include_directories(modul_app PRIVATE
    include
    # Where embed_text writes its headers, included as <shaders/basic_vert.hpp>.
    ${CMAKE_CURRENT_BINARY_DIR}/generated
)

# 8. Keep GLFW out of the GL header business — glad declares the GL API instead.
target_compile_definitions(modul_app PRIVATE GLFW_INCLUDE_NONE)

# 9. Windows: stop windows.h from defining min/max macros (breaks std::min)
if(WIN32)
    target_compile_definitions(modul_app PRIVATE NOMINMAX WIN32_LEAN_AND_MEAN)
    # The MSVC STL hard-errors (STL1000) when parsed by Clang older than 19.
    # MSVC itself never trips this check, so the define is inert for the real
    # build; it exists so clang-tidy/clangd can read compile_commands.json.
    # Drop it once LLVM here is 19 or newer.
    target_compile_definitions(modul_app PRIVATE _ALLOW_COMPILER_AND_STL_VERSION_MISMATCH)
endif()

# 10. Link libraries cross-platform (glfw arrives transitively via imgui)
if(APPLE)
    target_link_libraries(modul_app PRIVATE imgui glad dj_fft glm::glm "-framework OpenGL" "-framework Cocoa" "-framework IOKit" "-framework CoreVideo")
elseif(WIN32)
    target_link_libraries(modul_app PRIVATE imgui glad dj_fft glm::glm opengl32)
else()
    target_link_libraries(modul_app PRIVATE imgui glad dj_fft glm::glm GL pthread dl)
endif()