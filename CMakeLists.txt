cmake_minimum_required(VERSION 3.20)

project(main)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#---------------------------------------------------------------------3
#                               project                               |
#---------------------------------------------------------------------3

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.cpp") # source files
file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/include/*.h") # header files

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE
  ${CMAKE_SOURCE_DIR}/include
)

#---------------------------------------------------------------------3
#                             dependences                             |
#---------------------------------------------------------------------3

add_subdirectory(deps/glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE
  glfw
)

add_custom_target(format 
    COMMAND clang-format -i ${SOURCE_FILES} ${HEADER_FILES}
    COMMENT "Formatting all source and header files with clang-format"
)
