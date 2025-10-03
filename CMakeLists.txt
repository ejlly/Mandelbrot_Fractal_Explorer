cmake_minimum_required(VERSION 3.3 FATAL_ERROR)
project(mandel)
set(CMAKE_MODULE_PATH /home/charles/Libs/cmake)
set (CMAKE_CXX_STANDARD 17)


# Add source files
file(GLOB_RECURSE SOURCE_FILES 
	${CMAKE_SOURCE_DIR}/src/*.c
	${CMAKE_SOURCE_DIR}/src/*.cpp
)


# Set optimization or debug flags

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

	
# Add header files
file(GLOB_RECURSE HEADER_FILES 
	${CMAKE_SOURCE_DIR}/src/*.h
	${CMAKE_SOURCE_DIR}/src/*.hpp
)


# Define the executable
add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})


# We need a CMAKE_DIR with some code to find external dependencies
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# OpenGL
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)


set(GLFW_INCLUDE_PATH "" CACHE PATH "The directory that contains GL/glfw.h" )
set(GLFW_LIBRARY "" CACHE FILEPATH "libglfw.a or glfw.lib" )

set(GLEW_INCLUDE_PATH "" CACHE PATH "The directory that contains GL/glew.h" )
set(GLEW_LIBRARY "" CACHE FILEPATH "libglew.a or glew.lib" )

set(GLM_INCLUDE_PATH "" CACHE PATH "The directory that contains glm/glm.hpp" )

## GLM
#find_package(GLM REQUIRED)
#message(STATUS "GLM included at ${GLM_INCLUDE_DIR}")
#
## GLFW
#find_package(GLFW3 REQUIRED)
#message(STATUS "Found GLFW3 in ${GLFW3_INCLUDE_DIR}")
#

# Put all libraries into a variable
set(LIBS glfw GL GLEW)


# Define the include DIRs
include_directories(
	"${CMAKE_SOURCE_DIR}/src"
)

# Define the link libraries
target_link_libraries(${PROJECT_NAME} ${LIBS})