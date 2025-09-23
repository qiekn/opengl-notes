cmake_minimum_required(VERSION 3.20)

project(main)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(OPGL_PATH src/learn_opengl)
set(CHERNO_PATH src/the_cherno)

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
endif()

#---------------------------------------------------------------------3
#                               targets                               |
#---------------------------------------------------------------------3

# LearnOpenGL Examples
add_executable(opgl ${OPGL_PATH}/main.cpp)
target_link_libraries(opgl PRIVATE ${LIBS})

# The Cherno Tutorials
file(GLOB_RECURSE CHERNO_SRCS CONFIGURE_DEPENDS "${CHERNO_PATH}/src/*.cpp") # source files
add_executable(cherno
  ${CHERNO_SRCS}
)
target_include_directories(cherno PRIVATE ${CHERNO_PATH}/include/)
target_link_libraries(cherno PRIVATE ${LIBS})
