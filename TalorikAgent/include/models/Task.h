#pragma once

#include <string>
#include <chrono>
#include <vector>
#include <map>
#include <nlohmann/json.hpp>

enum class TaskStatus {
    PENDING,
    RUNNING,
    COMPLETED,
    FAILED,
    CANCELLED
};

enum class TaskPriority {
    LOW,
    NORMAL,
    HIGH,
    CRITICAL
};

class Task {
public:
    Task();
    Task(const std::string& id, 
         const std::string& name, 
         const std::string& description,
         TaskPriority priority = TaskPriority::NORMAL);

    // Getters
    const std::string& getId() const { return m_id; }
    const std::string& getName() const { return m_name; }
    const std::string& getDescription() const { return m_description; }
    TaskStatus getStatus() const { return m_status; }
    TaskPriority getPriority() const { return m_priority; }
    const std::string& getAssignedAgent() const { return m_assignedAgent; }
    const std::chrono::system_clock::time_point& getCreatedAt() const { return m_createdAt; }
    const std::chrono::system_clock::time_point& getStartedAt() const { return m_startedAt; }
    const std::chrono::system_clock::time_point& getCompletedAt() const { return m_completedAt; }
    const std::map<std::string, std::string>& getParameters() const { return m_parameters; }
    const std::string& getResult() const { return m_result; }
    const std::string& getErrorMessage() const { return m_errorMessage; }

    // Setters
    void setName(const std::string& name) { m_name = name; }
    void setDescription(const std::string& description) { m_description = description; }
    void setStatus(TaskStatus status) { m_status = status; }
    void setPriority(TaskPriority priority) { m_priority = priority; }
    void setAssignedAgent(const std::string& agentId) { m_assignedAgent = agentId; }
    void setResult(const std::string& result) { m_result = result; }
    void setErrorMessage(const std::string& error) { m_errorMessage = error; }

    // Parameter management
    void setParameter(const std::string& key, const std::string& value);
    std::string getParameter(const std::string& key, const std::string& defaultValue = "") const;
    bool hasParameter(const std::string& key) const;

    // Task lifecycle methods
    void start();
    void complete(const std::string& result = "");
    void fail(const std::string& errorMessage);
    void cancel();

    // Serialization
    nlohmann::json toJson() const;
    static Task fromJson(const nlohmann::json& json);

    // Utility methods
    std::string toString() const;
    bool isValid() const;
    bool isCompleted() const;
    bool isFailed() const;
    bool isCancelled() const;

private:
    std::string m_id;
    std::string m_name;
    std::string m_description;
    TaskStatus m_status;
    TaskPriority m_priority;
    std::string m_assignedAgent;
    std::chrono::system_clock::time_point m_createdAt;
    std::chrono::system_clock::time_point m_startedAt;
    std::chrono::system_clock::time_point m_completedAt;
    std::map<std::string, std::string> m_parameters;
    std::string m_result;
    std::string m_errorMessage;

    // Private helper methods
    std::string generateId();
    std::string taskStatusToString(TaskStatus status) const;
    TaskStatus stringToTaskStatus(const std::string& statusStr) const;
    std::string taskPriorityToString(TaskPriority priority) const;
    TaskPriority stringToTaskPriority(const std::string& priorityStr) const;
    std::string getCurrentTimestamp() const;
}; 