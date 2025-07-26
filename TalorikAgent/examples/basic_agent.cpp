#include <iostream>
#include <memory>
#include "agents/Agent.h"
#include "models/Task.h"
#include "utils/Logger.h"

class BasicAgent : public Agent {
public:
    BasicAgent() : Agent("basic_agent", "Basic Example Agent") {
        m_taskCount = 0;
    }

    bool initialize() override {
        Logger::info("BasicAgent initializing...");
        m_initialized = true;
        Logger::info("BasicAgent initialized successfully");
        return true;
    }

    void process() override {
        if (!m_initialized) {
            Logger::error("BasicAgent not initialized");
            return;
        }

        // Process any pending tasks
        for (auto& task : m_pendingTasks) {
            Logger::info("Processing task: " + task.getName());
            
            // Simulate task processing
            task.start();
            
            // Complete the task
            task.complete("Task completed successfully by BasicAgent");
            
            m_taskCount++;
            Logger::info("Task completed. Total tasks processed: " + std::to_string(m_taskCount));
        }
        
        m_pendingTasks.clear();
    }

    void shutdown() override {
        Logger::info("BasicAgent shutting down...");
        m_initialized = false;
        Logger::info("BasicAgent shutdown complete");
    }

    bool assignTask(const Task& task) override {
        Logger::info("BasicAgent received task: " + task.getName());
        m_pendingTasks.push_back(task);
        return true;
    }

private:
    std::vector<Task> m_pendingTasks;
    int m_taskCount;
};

int main() {
    // Initialize logging
    Logger::initialize(LogLevel::INFO);

    // Create and initialize the agent
    auto agent = std::make_unique<BasicAgent>();
    
    if (!agent->initialize()) {
        Logger::error("Failed to initialize BasicAgent");
        return 1;
    }

    // Create a sample task
    Task task("sample_task", "Sample Task", "This is a sample task for the BasicAgent");
    task.setParameter("param1", "value1");
    task.setParameter("param2", "value2");

    // Assign the task to the agent
    agent->assignTask(task);

    // Process the task
    agent->process();

    // Shutdown the agent
    agent->shutdown();

    Logger::info("Basic agent example completed");
    return 0;
} 