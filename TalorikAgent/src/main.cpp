#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include "agents/SecurityAgent.h"
#include "config/ConfigManager.h"
#include "utils/Logger.h"

int main(int argc, char* argv[])
{
    try {
        // Initialize configuration
        auto configManager = std::make_unique<ConfigManager>();
        if (!configManager->loadConfig("resources/config/agent_config.json")) {
            std::cerr << "Failed to load configuration" << std::endl;
            return 1;
        }

        // Initialize logging
        Logger::initialize(configManager->getLogLevel());

        // Create and start security agent
        auto securityAgent = std::make_unique<SecurityAgent>(configManager.get());
        
        if (!securityAgent->initialize()) {
            Logger::error("Failed to initialize security agent");
            return 1;
        }

        Logger::info("TalorikAgent Security Agent started successfully");
        Logger::info("API server will be available at http://localhost:8080");
        Logger::info("Available endpoints:");
        Logger::info("  GET  /api/security/metrics");
        Logger::info("  GET  /api/threats/data?range=24h");
        Logger::info("  GET  /api/threats/attack-types");
        Logger::info("  GET  /api/alerts/recent?limit=10");
        Logger::info("  GET  /api/system/status");
        Logger::info("  GET  /api/agent/status");
        Logger::info("  POST /api/security/scan");
        
        // Run the security agent
        securityAgent->run();

        // Keep the main thread alive
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        Logger::info("TalorikAgent shutting down");
        
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
} 