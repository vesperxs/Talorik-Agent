#include "services/TaskService.h"
#include "utils/Logger.h"

TaskService::TaskService() {
    Logger::info("TaskService initialized");
}

TaskService::~TaskService() {
    Logger::info("TaskService destroyed");
}

bool TaskService::initialize() {
    Logger::info("TaskService initializing");
    return true;
}

void TaskService::shutdown() {
    Logger::info("TaskService shutting down");
} 