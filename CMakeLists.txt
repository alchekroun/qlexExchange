cmake_minimum_required(VERSION 3.31)
project(qlexExchange VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add qlexNet as a subdirectory
add_subdirectory("extern/qlexNet")

# Create the executable
add_executable(${PROJECT_NAME} "src/main.cpp")

# Add include directory
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/include")
target_precompile_headers(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/include/Common.h")

# Link the INTERFACE library
target_link_libraries(${PROJECT_NAME} PRIVATE qlexNet)
