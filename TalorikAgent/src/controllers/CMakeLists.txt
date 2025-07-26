# Controllers module CMakeLists.txt

# Create controllers library
add_library(controllers
    TaskController.cpp
    AgentController.cpp
)

# Set include directories
target_include_directories(controllers PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

# Link dependencies
target_link_libraries(controllers
    agents
    models
    utils
) 