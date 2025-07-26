# Agents module CMakeLists.txt

# Create agents library
add_library(agents
    Agent.cpp
    SecurityAgent.cpp
)

# Set include directories
target_include_directories(agents PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

# Link dependencies
target_link_libraries(agents
    models
    utils
    network
) 