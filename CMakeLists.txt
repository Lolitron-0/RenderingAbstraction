cmake_minimum_required (VERSION 3.8)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project (RenderingAbstraction)

file(GLOB SOURCE_FILES
  "${PROJECT_SOURCE_DIR}/include/*.hpp"
  "${PROJECT_SOURCE_DIR}/include/*.h"
  "${PROJECT_SOURCE_DIR}/include/OpenGL/*.hpp"
  "${PROJECT_SOURCE_DIR}/src/*.cpp"
  "${PROJECT_SOURCE_DIR}/src/OpenGL/*.cpp"
  "${PROJECT_SOURCE_DIR}/thirdparty/Glad/include/glad/*.h"
  "${PROJECT_SOURCE_DIR}/thirdparty/Glad/src/*.c"
)

add_library (RenderingAbstraction STATIC ${SOURCE_FILES})

target_include_directories(RenderingAbstraction PUBLIC
  "${PROJECT_SOURCE_DIR}/include"
  "${PROJECT_SOURCE_DIR}/thirdparty/assimp/include"
  "${PROJECT_SOURCE_DIR}/thirdparty/stb_image"
  "${PROJECT_SOURCE_DIR}/thirdparty/glm"
  "${PROJECT_SOURCE_DIR}/thirdparty/glfw/include"
  "${PROJECT_SOURCE_DIR}/thirdparty/Glad/include"
  "${PROJECT_SOURCE_DIR}/thirdparty/Profiler/include"
)

target_compile_definitions(RenderingAbstraction PUBLIC
  "RA_DEBUG=$<CONFIG:Debug>"
  "PROFILER_ENABLE=$<CONFIG:Debug>"
)

if(WIN32)
	target_compile_definitions(RenderingAbstraction PUBLIC "RA_WINDOWS")
elseif(UNIX)
	target_compile_definitions(RenderingAbstraction PUBLIC "RA_UNIX")
else()
	target_compile_definitions(RenderingAbstraction PUBLIC "RA_UNIX")
endif()

target_compile_definitions(RenderingAbstraction PUBLIC "GLFW_INCLUDE_NONE")

#target_precompile_headers(RenderingAbstraction PUBLIC "${PROJECT_SOURCE_DIR}/include/rapch.h")

# DEPENDENCIES
include(FetchContent)

add_subdirectory(thirdparty/assimp)
target_link_libraries(RenderingAbstraction PUBLIC assimp)

add_subdirectory(thirdparty/glm)
target_link_libraries(RenderingAbstraction PUBLIC glm)

# set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
# set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
# set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
# set(GLFW_BUILD_WAYLAND ON)

find_package(glfw3 REQUIRED)
target_link_libraries(RenderingAbstraction PUBLIC glfw)

find_package(OpenGL REQUIRED)
target_link_libraries(RenderingAbstraction PUBLIC OpenGL::GL)


FetchContent_Declare(GSL
    GIT_REPOSITORY "https://github.com/microsoft/GSL"
    GIT_TAG "v4.0.0"
    GIT_SHALLOW ON
)
FetchContent_MakeAvailable(GSL)

target_link_libraries(RenderingAbstraction PUBLIC Microsoft.GSL::GSL)


if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET RenderingAbstraction PROPERTY CXX_STANDARD 20)
endif()
