# Config module CMakeLists.txt

# Create config library
add_library(config
    ConfigManager.cpp
)

# Set include directories
target_include_directories(config PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

# Find nlohmann/json
find_package(nlohmann_json REQUIRED)

# Link dependencies
target_link_libraries(config
    nlohmann_json::nlohmann_json
) 