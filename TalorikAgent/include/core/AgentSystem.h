#pragma once

#include <memory>
#include <vector>
#include <string>
#include "agents/Agent.h"

class ConfigManager;

class AgentSystem {
public:
    explicit AgentSystem(ConfigManager* configManager);
    ~AgentSystem();

    // Initialize the agent system
    bool initialize();

    // Run the agent system
    void run();

    // Shutdown the agent system
    void shutdown();

    // Add an agent to the system
    void addAgent(std::unique_ptr<Agent> agent);

    // Remove an agent from the system
    void removeAgent(const std::string& agentId);

    // Get agent by ID
    Agent* getAgent(const std::string& agentId);

private:
    ConfigManager* m_configManager;
    std::vector<std::unique_ptr<Agent>> m_agents;
    bool m_initialized;
    bool m_running;

    // Private helper methods
    void processAgents();
    void handleAgentCommunication();
}; 