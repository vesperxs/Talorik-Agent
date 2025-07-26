# TalorikAgent Project Structure

## Overview
This document provides a detailed overview of the TalorikAgent project structure and organization.

## Directory Structure

```
TalorikAgent/
├── src/                          # Source code files
│   ├── core/                     # Core system components
│   │   ├── AgentSystem.cpp       # Main system class implementation
│   │   └── CMakeLists.txt        # Build configuration for core
│   ├── utils/                    # Utility functions and helpers
│   │   ├── Logger.cpp            # Logging system implementation
│   │   └── CMakeLists.txt        # Build configuration for utils
│   ├── agents/                   # Agent implementations
│   │   ├── Agent.cpp             # Base agent class implementation
│   │   └── CMakeLists.txt        # Build configuration for agents
│   ├── network/                  # Network communication
│   │   ├── NetworkManager.cpp    # Network management implementation
│   │   └── CMakeLists.txt        # Build configuration for network
│   ├── config/                   # Configuration management
│   │   ├── ConfigManager.cpp     # Configuration system implementation
│   │   └── CMakeLists.txt        # Build configuration for config
│   ├── models/                   # Data models and structures
│   │   ├── Message.cpp           # Message model implementation
│   │   ├── Task.cpp              # Task model implementation
│   │   └── CMakeLists.txt        # Build configuration for models
│   ├── controllers/              # Control logic
│   │   ├── TaskController.cpp    # Task control implementation
│   │   ├── AgentController.cpp   # Agent control implementation
│   │   └── CMakeLists.txt        # Build configuration for controllers
│   ├── services/                 # Service layer
│   │   ├── TaskService.cpp       # Task service implementation
│   │   ├── AgentService.cpp      # Agent service implementation
│   │   └── CMakeLists.txt        # Build configuration for services
│   └── main.cpp                  # Main application entry point
├── include/                      # Header files
│   ├── core/                     # Core system headers
│   │   └── AgentSystem.h         # Main system class header
│   ├── utils/                    # Utility headers
│   │   └── Logger.h              # Logging system header
│   ├── agents/                   # Agent headers
│   │   └── Agent.h               # Base agent class header
│   ├── network/                  # Network headers
│   │   └── NetworkManager.h      # Network management header
│   ├── config/                   # Configuration headers
│   │   └── ConfigManager.h       # Configuration system header
│   ├── models/                   # Model headers
│   │   ├── Message.h             # Message model header
│   │   └── Task.h                # Task model header
│   ├── controllers/              # Controller headers
│   │   ├── TaskController.h      # Task control header
│   │   └── AgentController.h     # Agent control header
│   └── services/                 # Service headers
│       ├── TaskService.h         # Task service header
│       └── AgentService.h        # Agent service header
├── tests/                        # Test files
│   ├── unit/                     # Unit tests
│   │   ├── test_agent.cpp        # Agent unit tests
│   │   ├── test_config.cpp       # Configuration unit tests
│   │   └── CMakeLists.txt        # Unit test build configuration
│   ├── integration/              # Integration tests
│   │   ├── test_system.cpp       # System integration tests
│   │   └── CMakeLists.txt        # Integration test build configuration
│   ├── mocks/                    # Mock objects for testing
│   │   └── MockAgent.h           # Mock agent for testing
│   └── CMakeLists.txt            # Test build configuration
├── docs/                         # Documentation
│   ├── api/                      # API documentation
│   │   └── README.md             # API overview and examples
│   └── user_guide/               # User guides
│       └── getting_started.md    # Getting started guide
├── build/                        # Build artifacts (generated)
├── scripts/                      # Build and utility scripts
│   ├── build.bat                 # Windows build script
│   └── build.sh                  # Unix build script
├── tools/                        # Development tools
│   └── code_generator.py         # Code generation tool
├── third_party/                  # Third-party dependencies
│   └── README.md                 # Third-party dependency info
├── resources/                    # Resources
│   ├── data/                     # Data files
│   │   └── sample_data.json      # Sample data for testing
│   └── config/                   # Configuration files
│       └── agent_config.json     # Default agent configuration
├── examples/                     # Example code
│   └── basic_agent.cpp           # Basic agent example
├── CMakeLists.txt                # Main CMake configuration
├── vcpkg.json                    # Dependency management
├── .gitignore                    # Git ignore rules
├── README.md                     # Project overview
└── PROJECT_STRUCTURE.md          # This file
```

## Component Descriptions

### Core Components
- **AgentSystem**: Main orchestrator that manages all agents and coordinates system activities
- **Agent**: Base class for all agent implementations with common functionality

### Utilities
- **Logger**: Static logging utility with configurable levels and file output
- **ConfigManager**: JSON-based configuration management system

### Models
- **Message**: Inter-agent communication with type safety and serialization
- **Task**: Task representation with full lifecycle management

### Network
- **NetworkManager**: Handles network communication between agents and external systems

### Controllers
- **TaskController**: Manages task assignment, execution, and monitoring
- **AgentController**: Manages agent lifecycle and coordination

### Services
- **TaskService**: High-level service for task management operations
- **AgentService**: High-level service for agent management operations

## Build System

The project uses CMake as the primary build system with the following features:
- Modular library structure
- Dependency management via vcpkg
- Support for both Visual Studio and Unix-like systems
- Integrated testing framework

## Development Workflow

1. **Adding New Components**:
   - Create header in appropriate `include/` subdirectory
   - Create implementation in corresponding `src/` subdirectory
   - Update CMakeLists.txt for the module
   - Add tests in `tests/` directory

2. **Building**:
   - Windows: Run `scripts/build.bat`
   - Unix: Run `scripts/build.sh`
   - Or use CMake directly: `cmake -B build && cmake --build build`

3. **Testing**:
   - Unit tests: `ctest --test-dir build/tests/unit`
   - Integration tests: `ctest --test-dir build/tests/integration`

## Dependencies

- **nlohmann-json**: JSON parsing and serialization
- **gtest**: Unit testing framework
- **spdlog**: Advanced logging (optional)
- **asio**: Network communication (optional)
- **sqlite3**: Database storage (optional)

## Configuration

The system uses JSON-based configuration files located in `resources/config/`. The main configuration file is `agent_config.json` which contains:
- System settings
- Agent configuration
- Network settings
- Task management settings
- Security settings
- Database configuration 