# Utils module CMakeLists.txt

# Create utils library
add_library(utils
    Logger.cpp
    SimpleJson.cpp
)

# Set include directories
target_include_directories(utils PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

# Link dependencies
# Note: utils doesn't depend on config, removing the link 