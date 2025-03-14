cmake_minimum_required(VERSION 3.31)
project(qlexExchange VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Var
set(EXTERN_DIR "${CMAKE_SOURCE_DIR}/extern")
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")

# Add qlexNet as a subdirectory
add_subdirectory("extern/qlexNet")
add_subdirectory(src)

# Create the executable
add_executable(${PROJECT_NAME} "src/main.cpp")

# Link the INTERFACE library
target_link_libraries(${PROJECT_NAME} PRIVATE qlexNet finance)

# Add include directory
target_include_directories(${PROJECT_NAME} PRIVATE "${INCLUDE_DIR}")
target_precompile_headers(${PROJECT_NAME} PRIVATE "${INCLUDE_DIR}/Common.h")