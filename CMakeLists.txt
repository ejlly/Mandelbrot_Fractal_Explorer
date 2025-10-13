cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(mandel)
set (CMAKE_CXX_STANDARD 17)

# Add source files
file(GLOB_RECURSE SOURCE_FILES 
	${CMAKE_SOURCE_DIR}/src/*.c
	${CMAKE_SOURCE_DIR}/src/*.cpp
)

# Add header files
file(GLOB_RECURSE HEADER_FILES 
	${CMAKE_SOURCE_DIR}/src/*.h
	${CMAKE_SOURCE_DIR}/src/*.hpp
)

# Set default build type to Release if not specified
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif()

# Set flags for Release and Debug
set(CMAKE_CXX_FLAGS_RELEASE "-O2")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")

# Custom debug target
add_custom_target(debug
	COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Debug -B ${CMAKE_BINARY_DIR}/debug -S ${CMAKE_SOURCE_DIR}
	COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}/debug
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
	COMMENT "Building debug version in ./debug directory"
)

# Define the executable
add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

cmake_policy(SET CMP0072 NEW)
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)

# Define the include DIRs
# Include paths
include_directories(
	"${CMAKE_SOURCE_DIR}/src"
	"${CMAKE_SOURCE_DIR}/external/imgui"
	"${CMAKE_SOURCE_DIR}/external/imgui/backends"
)

# Add ImGui source files
set(IMGUI_SRC
	${CMAKE_SOURCE_DIR}/external/imgui/imgui.cpp
	${CMAKE_SOURCE_DIR}/external/imgui/imgui_draw.cpp
	${CMAKE_SOURCE_DIR}/external/imgui/imgui_tables.cpp
	${CMAKE_SOURCE_DIR}/external/imgui/imgui_widgets.cpp
	${CMAKE_SOURCE_DIR}/external/imgui/backends/imgui_impl_glfw.cpp
	${CMAKE_SOURCE_DIR}/external/imgui/backends/imgui_impl_opengl3.cpp
)

# Add ImGui sources to the target
target_sources(${PROJECT_NAME} PRIVATE ${IMGUI_SRC})

# Link all libraries (remove AntTweakBar)
target_link_libraries(${PROJECT_NAME}
	glfw
	GL
	GLEW
	pthread
	dl
	X11
)



## We need a CMAKE_DIR with some code to find external dependencies
#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
#
## OpenGL
#set(OpenGL_GL_PREFERENCE GLVND)
#find_package(OpenGL REQUIRED)
#
#
#set(GLFW_INCLUDE_PATH "" CACHE PATH "The directory that contains GL/glfw.h" )
#set(GLFW_LIBRARY "" CACHE FILEPATH "libglfw.a or glfw.lib" )
#
#set(GLEW_INCLUDE_PATH "" CACHE PATH "The directory that contains GL/glew.h" )
#set(GLEW_LIBRARY "" CACHE FILEPATH "libglew.a or glew.lib" )
#
#set(GLM_INCLUDE_PATH "" CACHE PATH "The directory that contains glm/glm.hpp" )