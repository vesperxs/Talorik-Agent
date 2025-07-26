# Services module CMakeLists.txt

# Create services library
add_library(services
    TaskService.cpp
    AgentService.cpp
)

# Set include directories
target_include_directories(services PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

# Link dependencies
target_link_libraries(services
    controllers
    network
    utils
) 