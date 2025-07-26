#include "controllers/AgentController.h"
#include "utils/Logger.h"

AgentController::AgentController() {
    Logger::info("AgentController initialized");
}

AgentController::~AgentController() {
    Logger::info("AgentController destroyed");
}

bool AgentController::initialize() {
    Logger::info("AgentController initializing");
    return true;
}

void AgentController::shutdown() {
    Logger::info("AgentController shutting down");
} 