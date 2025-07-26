#pragma once

#include <string>
#include <memory>
#include <functional>
#include "models/Message.h"
#include "models/Task.h"

class Agent {
public:
    explicit Agent(const std::string& id, const std::string& name);
    virtual ~Agent() = default;

    // Core agent methods
    virtual bool initialize() = 0;
    virtual void process() = 0;
    virtual void shutdown() = 0;

    // Message handling
    virtual void receiveMessage(const Message& message);
    virtual void sendMessage(const std::string& targetId, const Message& message);

    // Task management
    virtual bool assignTask(const Task& task);
    virtual void completeTask(const std::string& taskId);

    // Getters
    const std::string& getId() const { return m_id; }
    const std::string& getName() const { return m_name; }
    bool isActive() const { return m_active; }

    // Setters
    void setActive(bool active) { m_active = active; }

protected:
    std::string m_id;
    std::string m_name;
    bool m_active;
    bool m_initialized;

    // Protected helper methods
    void logInfo(const std::string& message);
    void logError(const std::string& message);
    void logDebug(const std::string& message);
}; 