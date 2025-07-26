# Network module CMakeLists.txt

# Create network library
add_library(network
    HttpServer.cpp
)

# Set include directories
target_include_directories(network PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

# Find asio (optional)
find_package(asio QUIET)
if(asio_FOUND)
    target_link_libraries(network utils asio::asio)
    target_compile_definitions(network PRIVATE HAS_ASIO)
    message(STATUS "Using asio from package manager")
else()
    target_link_libraries(network utils)
    message(STATUS "asio not found, network functionality may be limited")
endif() 