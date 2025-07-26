# Models module CMakeLists.txt

# Create models library
add_library(models
    Message.cpp
    Task.cpp
    SecurityModels.cpp
)

# Set include directories
target_include_directories(models PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

# Find nlohmann/json (optional)
find_package(nlohmann_json QUIET)
if(nlohmann_json_FOUND)
    target_link_libraries(models nlohmann_json::nlohmann_json)
    target_compile_definitions(models PRIVATE HAS_NLOHMANN_JSON)
    message(STATUS "Using nlohmann/json from package manager")
else()
    message(STATUS "nlohmann/json not found, using SimpleJson fallback")
    # No additional linking needed for SimpleJson as it's part of utils
endif() 