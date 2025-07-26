#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include "utils/Logger.h"
#include "models/Message.h"
#include "models/Task.h"

int main(int argc, char* argv[])
{
    try {
        // Initialize logging
        Logger::initialize(Logger::Level::INFO);
        
        Logger::info("TalorikAgent Security Agent - Simple Test Version");
        Logger::info("Testing basic functionality...");

        // Test Message creation
        Message msg("agent1", "agent2", MessageType::HEARTBEAT, "ping");
        Logger::info("Created message: " + msg.toString());

        // Test Task creation
        Task task("task1", "Security Scan", "Perform security scan", TaskPriority::HIGH);
        Logger::info("Created task: " + task.toString());

        // Test JSON serialization
        try {
            auto msgJson = msg.toJson();
            auto taskJson = task.toJson();
#ifdef HAS_NLOHMANN_JSON
            Logger::info("JSON serialization working (nlohmann/json)");
#else
            Logger::info("JSON serialization working (SimpleJson fallback)");
#endif
        } catch (const std::exception& e) {
            Logger::warning("JSON serialization failed: " + std::string(e.what()));
        }

        Logger::info("Basic functionality test completed successfully");
        Logger::info("Press Ctrl+C to exit");

        // Keep the main thread alive
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(5));
            Logger::info("Agent running...");
        }

    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
} 