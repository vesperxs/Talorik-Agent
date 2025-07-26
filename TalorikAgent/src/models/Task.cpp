#include "models/Task.h"
#include <sstream>
#include <iomanip>
#include <random>

Task::Task() : m_priority(TaskPriority::NORMAL), m_status(TaskStatus::PENDING) {
    m_id = generateId();
    m_createdAt = std::chrono::system_clock::now();
}

Task::Task(const std::string& id, 
           const std::string& name, 
           const std::string& description,
           TaskPriority priority)
    : m_id(id)
    , m_name(name)
    , m_description(description)
    , m_priority(priority)
    , m_status(TaskStatus::PENDING) {
    if (m_id.empty()) {
        m_id = generateId();
    }
    m_createdAt = std::chrono::system_clock::now();
}

nlohmann::json Task::toJson() const {
    return {
        {"id", m_id},
        {"name", m_name},
        {"description", m_description},
        {"priority", taskPriorityToString(m_priority)},
        {"status", taskStatusToString(m_status)},
        {"createdAt", getCurrentTimestamp()},
        {"parameters", m_parameters}
    };
}

Task Task::fromJson(const nlohmann::json& json) {
    Task task;
    
    task.m_id = json.value("id", "");
    task.m_name = json.value("name", "");
    task.m_description = json.value("description", "");
    task.m_priority = task.stringToTaskPriority(json.value("priority", "normal"));
    task.m_status = task.stringToTaskStatus(json.value("status", "pending"));
    
    if (json.contains("parameters")) {
        task.m_parameters = json["parameters"];
    }
    
    // Parse timestamp if present
    if (json.contains("createdAt")) {
        task.m_createdAt = std::chrono::system_clock::now();
    }
    
    return task;
}

std::string Task::toString() const {
    std::ostringstream oss;
    oss << "Task[id=" << m_id 
        << ", name=" << m_name 
        << ", priority=" << taskPriorityToString(m_priority)
        << ", status=" << taskStatusToString(m_status) << "]";
    return oss.str();
}

bool Task::isValid() const {
    return !m_id.empty() && !m_name.empty();
}

void Task::setParameter(const std::string& key, const std::string& value) {
    m_parameters[key] = value;
}

std::string Task::getParameter(const std::string& key, const std::string& defaultValue) const {
    auto it = m_parameters.find(key);
    return (it != m_parameters.end()) ? it->second : defaultValue;
}

bool Task::hasParameter(const std::string& key) const {
    return m_parameters.find(key) != m_parameters.end();
}

void Task::start() {
    m_status = TaskStatus::RUNNING;
    m_startedAt = std::chrono::system_clock::now();
}

void Task::complete(const std::string& result) {
    m_status = TaskStatus::COMPLETED;
    m_result = result;
    m_completedAt = std::chrono::system_clock::now();
}

void Task::fail(const std::string& errorMessage) {
    m_status = TaskStatus::FAILED;
    m_errorMessage = errorMessage;
    m_completedAt = std::chrono::system_clock::now();
}

void Task::cancel() {
    m_status = TaskStatus::CANCELLED;
    m_completedAt = std::chrono::system_clock::now();
}

bool Task::isCompleted() const {
    return m_status == TaskStatus::COMPLETED;
}

bool Task::isFailed() const {
    return m_status == TaskStatus::FAILED;
}

bool Task::isCancelled() const {
    return m_status == TaskStatus::CANCELLED;
}

std::string Task::generateId() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, 15);
    static const char* hex = "0123456789abcdef";
    
    std::string id;
    for (int i = 0; i < 16; ++i) {
        id += hex[dis(gen)];
    }
    return "task_" + id;
}

std::string Task::taskPriorityToString(TaskPriority priority) const {
    switch (priority) {
        case TaskPriority::LOW: return "low";
        case TaskPriority::NORMAL: return "normal";
        case TaskPriority::HIGH: return "high";
        case TaskPriority::CRITICAL: return "critical";
        default: return "normal";
    }
}

TaskPriority Task::stringToTaskPriority(const std::string& priorityStr) const {
    if (priorityStr == "low") return TaskPriority::LOW;
    if (priorityStr == "normal") return TaskPriority::NORMAL;
    if (priorityStr == "high") return TaskPriority::HIGH;
    if (priorityStr == "critical") return TaskPriority::CRITICAL;
    return TaskPriority::NORMAL; // Default
}

std::string Task::taskStatusToString(TaskStatus status) const {
    switch (status) {
        case TaskStatus::PENDING: return "pending";
        case TaskStatus::RUNNING: return "running";
        case TaskStatus::COMPLETED: return "completed";
        case TaskStatus::FAILED: return "failed";
        case TaskStatus::CANCELLED: return "cancelled";
        default: return "pending";
    }
}

TaskStatus Task::stringToTaskStatus(const std::string& statusStr) const {
    if (statusStr == "pending") return TaskStatus::PENDING;
    if (statusStr == "running") return TaskStatus::RUNNING;
    if (statusStr == "completed") return TaskStatus::COMPLETED;
    if (statusStr == "failed") return TaskStatus::FAILED;
    if (statusStr == "cancelled") return TaskStatus::CANCELLED;
    return TaskStatus::PENDING; // Default
}

std::string Task::getCurrentTimestamp() const {
    auto time_t = std::chrono::system_clock::to_time_t(m_createdAt);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        m_createdAt.time_since_epoch()) % 1000;
    
    std::stringstream ss;
    ss << std::put_time(std::gmtime(&time_t), "%Y-%m-%dT%H:%M:%S");
    ss << '.' << std::setfill('0') << std::setw(3) << ms.count() << 'Z';
    return ss.str();
} 