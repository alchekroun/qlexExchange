cmake_minimum_required(VERSION 3.31)
project(qlexExchange VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add qlexNet as a subdirectory
add_subdirectory("extern/qlexNet")

# Create the executable
add_executable(${PROJECT_NAME} "src/main.cpp")

# Link the INTERFACE library
target_link_libraries(${PROJECT_NAME} PRIVATE qlexNet)
