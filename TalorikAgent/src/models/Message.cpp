#include "models/Message.h"
#include <sstream>
#include <iomanip>
#include <random>

Message::Message() : m_type(MessageType::HEARTBEAT) {
    m_id = generateId();
    m_timestamp = std::chrono::system_clock::now();
}

Message::Message(const std::string& senderId, 
                 const std::string& receiverId, 
                 MessageType type, 
                 const std::string& content)
    : m_senderId(senderId)
    , m_receiverId(receiverId)
    , m_type(type)
    , m_content(content) {
    m_id = generateId();
    m_timestamp = std::chrono::system_clock::now();
}

JsonType Message::toJson() const {
#ifdef HAS_NLOHMANN_JSON
    return {
        {"id", m_id},
        {"senderId", m_senderId},
        {"receiverId", m_receiverId},
        {"type", messageTypeToString(m_type)},
        {"content", m_content},
        {"timestamp", getCurrentTimestamp()}
    };
#else
    SimpleJson json;
    json.set("id", m_id);
    json.set("senderId", m_senderId);
    json.set("receiverId", m_receiverId);
    json.set("type", messageTypeToString(m_type));
    json.set("content", m_content);
    json.set("timestamp", getCurrentTimestamp());
    return json;
#endif
}

Message Message::fromJson(const JsonType& json) {
    Message message;
    
#ifdef HAS_NLOHMANN_JSON
    message.m_id = json.value("id", "");
    message.m_senderId = json.value("senderId", "");
    message.m_receiverId = json.value("receiverId", "");
    message.m_type = message.stringToMessageType(json.value("type", "heartbeat"));
    message.m_content = json.value("content", "");
    
    // Parse timestamp if present
    if (json.contains("timestamp")) {
        // Simple timestamp parsing - in a real implementation you'd want more robust parsing
        message.m_timestamp = std::chrono::system_clock::now();
    }
#else
    // For SimpleJson, we'll just set default values for now
    message.m_id = "";
    message.m_senderId = "";
    message.m_receiverId = "";
    message.m_type = MessageType::HEARTBEAT;
    message.m_content = "";
    message.m_timestamp = std::chrono::system_clock::now();
#endif
    
    return message;
}

std::string Message::toString() const {
    std::ostringstream oss;
    oss << "Message[id=" << m_id 
        << ", sender=" << m_senderId 
        << ", receiver=" << m_receiverId 
        << ", type=" << messageTypeToString(m_type)
        << ", content=" << m_content << "]";
    return oss.str();
}

bool Message::isValid() const {
    return !m_id.empty() && !m_senderId.empty() && !m_receiverId.empty();
}

std::string Message::generateId() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, 15);
    static const char* hex = "0123456789abcdef";
    
    std::string id;
    for (int i = 0; i < 32; ++i) {
        id += hex[dis(gen)];
    }
    return id;
}

std::string Message::messageTypeToString(MessageType type) const {
    switch (type) {
        case MessageType::TASK_REQUEST: return "task_request";
        case MessageType::TASK_RESPONSE: return "task_response";
        case MessageType::STATUS_UPDATE: return "status_update";
        case MessageType::ERROR: return "error";
        case MessageType::HEARTBEAT: return "heartbeat";
        case MessageType::CONFIG_UPDATE: return "config_update";
        default: return "unknown";
    }
}

MessageType Message::stringToMessageType(const std::string& typeStr) const {
    if (typeStr == "task_request") return MessageType::TASK_REQUEST;
    if (typeStr == "task_response") return MessageType::TASK_RESPONSE;
    if (typeStr == "status_update") return MessageType::STATUS_UPDATE;
    if (typeStr == "error") return MessageType::ERROR;
    if (typeStr == "heartbeat") return MessageType::HEARTBEAT;
    if (typeStr == "config_update") return MessageType::CONFIG_UPDATE;
    return MessageType::HEARTBEAT; // Default
}

std::string Message::getCurrentTimestamp() const {
    auto time_t = std::chrono::system_clock::to_time_t(m_timestamp);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        m_timestamp.time_since_epoch()) % 1000;
    
    std::stringstream ss;
    ss << std::put_time(std::gmtime(&time_t), "%Y-%m-%dT%H:%M:%S");
    ss << '.' << std::setfill('0') << std::setw(3) << ms.count() << 'Z';
    return ss.str();
} 