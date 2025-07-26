# TalorikAgent API Documentation

## Overview

This document provides comprehensive API documentation for the TalorikAgent system.

## Core Components

### AgentSystem
The main system class that manages all agents and coordinates their activities.

### Agent
Base class for all agent implementations. Provides common functionality for message handling and task management.

### ConfigManager
Manages system configuration, including loading from JSON files and providing access to configuration values.

### Logger
Static logging utility for system-wide logging with configurable log levels.

## Models

### Message
Represents inter-agent communication messages with support for different message types.

### Task
Represents tasks that can be assigned to agents with full lifecycle management.

## Network

### NetworkManager
Handles network communication between agents and external systems.

## Controllers

### TaskController
Manages task assignment, execution, and monitoring.

### AgentController
Manages agent lifecycle and coordination.

## Services

### TaskService
High-level service for task management operations.

### AgentService
High-level service for agent management operations.

## Usage Examples

### Basic Agent Creation
```cpp
#include "agents/Agent.h"

class MyAgent : public Agent {
public:
    MyAgent() : Agent("my_agent", "My Custom Agent") {}
    
    bool initialize() override {
        // Initialize agent
        return true;
    }
    
    void process() override {
        // Process agent logic
    }
    
    void shutdown() override {
        // Cleanup
    }
};
```

### Configuration Management
```cpp
#include "config/ConfigManager.h"

ConfigManager config;
config.loadConfig("config.json");
std::string value = config.getString("system.name");
```

### Logging
```cpp
#include "utils/Logger.h"

Logger::info("System started");
Logger::error("An error occurred");
``` 