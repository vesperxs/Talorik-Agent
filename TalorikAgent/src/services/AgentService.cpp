#include "services/AgentService.h"
#include "utils/Logger.h"

AgentService::AgentService() {
    Logger::info("AgentService initialized");
}

AgentService::~AgentService() {
    Logger::info("AgentService destroyed");
}

bool AgentService::initialize() {
    Logger::info("AgentService initializing");
    return true;
}

void AgentService::shutdown() {
    Logger::info("AgentService shutting down");
} 