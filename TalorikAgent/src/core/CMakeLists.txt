# Core module CMakeLists.txt

# Create core library
add_library(core
    AgentSystem.cpp
)

# Set include directories
target_include_directories(core PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

# Link dependencies
target_link_libraries(core
    config
    utils
    agents
) 