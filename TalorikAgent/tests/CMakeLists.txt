# Tests module CMakeLists.txt

# Enable testing
enable_testing()

# Find Google Test
find_package(GTest REQUIRED)

# Unit tests (only if directory exists)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/unit")
    add_subdirectory(unit)
endif()

# Integration tests (only if directory exists)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/integration")
    add_subdirectory(integration)
endif() 