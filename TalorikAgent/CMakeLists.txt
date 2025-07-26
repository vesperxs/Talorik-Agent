cmake_minimum_required(VERSION 3.15)
project(TalorikAgent VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Try to use vcpkg if available, but don't fail if it's not
if(EXISTS "C:/vcpkg/scripts/buildsystems/vcpkg.cmake")
    # Only use vcpkg if we're not doing a minimal build
    if(NOT BUILD_MINIMAL)
        set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
        message(STATUS "Using vcpkg toolchain")
    else()
        message(STATUS "Minimal build requested, skipping vcpkg")
    endif()
else()
    message(STATUS "vcpkg not found, using system packages")
endif()

# Find packages (PkgConfig is optional)
find_package(PkgConfig)

# Add subdirectories for different components (in dependency order)
add_subdirectory(src/utils)
add_subdirectory(src/config)
add_subdirectory(src/models)
add_subdirectory(src/network)
add_subdirectory(src/agents)
add_subdirectory(src/controllers)
add_subdirectory(src/services)

# Option to build minimal version
option(BUILD_MINIMAL "Build minimal version without external dependencies" OFF)

if(BUILD_MINIMAL)
    # Minimal executable
    add_executable(TalorikAgent 
        src/main_simple.cpp
    )
    
    # Link minimal libraries
    target_link_libraries(TalorikAgent
        utils
        models
    )
    
    message(STATUS "Building minimal version")
else()
    # Full executable
    add_executable(TalorikAgent 
        src/main.cpp
    )
    
    # Link all libraries
    target_link_libraries(TalorikAgent
        utils
        agents
        network
        config
        models
        controllers
        services
    )
    
    message(STATUS "Building full version")
endif()

# Enable testing
enable_testing()
add_subdirectory(tests) 