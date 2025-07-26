#include "agents/Agent.h"
#include "utils/Logger.h"

Agent::Agent() : m_initialized(false), m_running(false) {
}

Agent::~Agent() {
    shutdown();
}

bool Agent::initialize() {
    if (m_initialized) {
        return true;
    }
    
    Logger::info("Initializing agent: " + getAgentId());
    m_initialized = true;
    return true;
}

void Agent::run() {
    if (!m_initialized) {
        Logger::error("Cannot run agent " + getAgentId() + " - not initialized");
        return;
    }
    
    Logger::info("Starting agent: " + getAgentId());
    m_running = true;
    
    // Default implementation - just keep running
    while (m_running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void Agent::shutdown() {
    if (!m_running) {
        return;
    }
    
    Logger::info("Shutting down agent: " + getAgentId());
    m_running = false;
}

bool Agent::isInitialized() const {
    return m_initialized;
}

bool Agent::isRunning() const {
    return m_running;
} 