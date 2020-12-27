cmake_minimum_required(VERSION 3.8)

project("interpreter")

set(CMAKE_CXX_STANDARD 17)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include/semantic)
include_directories(${PROJECT_SOURCE_DIR}/include/ast)
include_directories(${PROJECT_SOURCE_DIR}/include/ast/literal)

file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp 
					${PROJECT_SOURCE_DIR}/src/ast/*.cpp
					${PROJECT_SOURCE_DIR}/src/semantic/*.cpp
					${PROJECT_SOURCE_DIR}/src/ast/literal/*.cpp)

add_executable(interpreter ${SRC_FILES})

#target_compile_options(interpreter PUBLIC /MT)
#target_link_options(interpreter PUBLIC /INCREMENTAL:NO /NODEFAULTLIB:MSVCRT)

set_target_properties(interpreter PROPERTIES OUTPUT_NAME "ionai")