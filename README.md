# TalorikAgent

A sophisticated C++ agent system for intelligent task execution and automation, with a focus on cybersecurity monitoring and threat detection.

## Features

- **Security Monitoring**: Real-time threat detection and analysis
- **REST API**: Complete API for React dashboard integration
- **WebSocket Support**: Real-time updates and notifications
- **Configurable**: Flexible configuration system
- **Cross-platform**: Windows, Linux, and macOS support

## Quick Start

### Prerequisites
- Visual Studio 2019 or later (Windows)
- CMake 3.15 or later
- C++17 compatible compiler
- vcpkg for dependency management

### Build and Run

1. **Install dependencies:**
   ```bash
   vcpkg install nlohmann-json crow spdlog asio
   ```

2. **Build the project:**
   ```bash
   # Windows
   scripts/build_windows.bat
   
   # Linux/macOS
   mkdir build && cd build
   cmake .. -DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
   cmake --build .
   ```

3. **Run the security agent:**
   ```bash
   ./build/bin/TalorikAgent
   ```

4. **Test the API:**
   ```bash
   pip install -r scripts/requirements.txt
   python scripts/test_api.py
   ```

The security agent will start on `http://localhost:8080` and provide REST API endpoints for cybersecurity dashboard integration.

## Project Structure

```
TalorikAgent/
├── src/                    # Source files
│   ├── core/              # Core system components
│   ├── utils/             # Utility functions and helpers
│   ├── agents/            # Agent implementations
│   ├── network/           # Network communication
│   ├── config/            # Configuration management
│   ├── models/            # Data models and structures
│   ├── controllers/       # Control logic
│   └── services/          # Service layer
├── include/               # Header files
│   ├── core/              # Core headers
│   ├── utils/             # Utility headers
│   ├── agents/            # Agent headers
│   ├── network/           # Network headers
│   ├── config/            # Configuration headers
│   ├── models/            # Model headers
│   ├── controllers/       # Controller headers
│   └── services/          # Service headers
├── tests/                 # Test files
│   ├── unit/              # Unit tests
│   ├── integration/       # Integration tests
│   └── mocks/             # Mock objects
├── docs/                  # Documentation
│   ├── api/               # API documentation
│   └── user_guide/        # User guides
├── build/                 # Build artifacts
├── scripts/               # Build and utility scripts
├── tools/                 # Development tools
├── third_party/           # Third-party dependencies
├── resources/             # Resources
│   ├── data/              # Data files
│   └── config/            # Configuration files
└── examples/              # Example code
```

## Building the Project

### Prerequisites
- Visual Studio 2019 or later
- CMake 3.15 or later
- C++17 compatible compiler

### Build Instructions
1. Clone the repository
2. Open the solution in Visual Studio
3. Build the project (Ctrl+Shift+B)

## Security Agent API

The TalorikAgent includes a comprehensive security monitoring system with REST API endpoints for React dashboard integration.

### Available Endpoints

- `GET /api/security/metrics` - Security metrics and statistics
- `GET /api/threats/data?range=24h` - Time series threat data
- `GET /api/threats/attack-types` - Attack type distribution
- `GET /api/alerts/recent?limit=10` - Recent security alerts
- `GET /api/system/status` - System component status
- `GET /api/agent/status` - Agent health and status
- `POST /api/security/scan` - Trigger security scans

### Integration with React Dashboard

The agent is designed to work seamlessly with React cybersecurity dashboards. See `docs/SECURITY_AGENT_API.md` for detailed integration instructions.

### Testing

Test the API endpoints using the provided Python script:
```bash
python scripts/test_api.py
```

Or use curl:
```bash
curl http://localhost:8080/api/security/metrics
curl http://localhost:8080/api/threats/data?range=24h
```

## Development

### Adding New Components
1. Create source files in appropriate `src/` subdirectory
2. Create header files in corresponding `include/` subdirectory
3. Add tests in `tests/` directory
4. Update documentation as needed

### Code Style
- Follow C++ Core Guidelines
- Use meaningful variable and function names
- Add comments for complex logic
- Include proper error handling

### Adding New API Endpoints

1. Add the endpoint handler in `SecurityAgent::setupApiRoutes()`
2. Implement the corresponding data method
3. Update the test script in `scripts/test_api.py`
4. Update documentation in `docs/SECURITY_AGENT_API.md`

## Testing

Run tests using the test framework of your choice. Unit tests are located in `tests/unit/` and integration tests in `tests/integration/`.

## Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Add tests for new functionality
5. Submit a pull request

## License

[Add your license information here] 
