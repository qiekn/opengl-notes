cmake_minimum_required(VERSION 3.20)

project(main)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_BUILD_TYPE Debug)

set(APP opgl)

#---------------------------------------------------------------------3
#                               project                               |
#---------------------------------------------------------------------3

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.cpp") # source files
file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/include/*.h") # header files

#---------------------------------------------------------------------3
#                             dependences                             |
#---------------------------------------------------------------------3

add_subdirectory(deps/glfw)
add_subdirectory(deps/glad)
add_subdirectory(deps/glm)
add_subdirectory(deps/stb_image)
add_subdirectory(deps/imgui)
target_link_libraries(imgui PUBLIC glfw)

set(LIBS glfw glad glm stb_image imgui)

if(APPLE)
  list(APPEND LIBS "-framework OpenGL")
elseif(WIN32)
  list(APPEND LIBS "opengl32")
endif()

#---------------------------------------------------------------------3
#                               targets                               |
#---------------------------------------------------------------------3

# LearnOpenGL Examples
add_executable(${APP} ${SOURCE_FILES})
target_include_directories(${APP} PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(${APP} PRIVATE ${LIBS})
