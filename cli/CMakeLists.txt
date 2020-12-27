cmake_minimum_required(VERSION 3.8)

project("cli")

set(CMAKE_CXX_STANDARD 17)

file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_executable(cli ${SRC_FILES})

#target_compile_options(cli PRIVATE /MT)
#target_link_options(cli PRIVATE /INCREMENTAL:NO /NODEFAULTLIB:MSVCRT)

set_target_properties(cli PROPERTIES OUTPUT_NAME "iona")