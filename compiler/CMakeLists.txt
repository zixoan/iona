cmake_minimum_required(VERSION 3.8)

project("compiler")

set(CMAKE_CXX_STANDARD 17)

include_directories(${PROJECT_SOURCE_DIR}/include)
file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_executable(compiler ${SRC_FILES})

set_target_properties(compiler PROPERTIES OUTPUT_NAME "ionac")