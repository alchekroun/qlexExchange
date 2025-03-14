add_library(finance STATIC LimitOrderBook.cpp)

target_include_directories(finance PUBLIC "${CMAKE_SOURCE_DIR}/include")
