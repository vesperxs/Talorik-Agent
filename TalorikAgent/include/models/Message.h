#pragma once

#include <string>
#include <chrono>

// Try to include nlohmann/json, fallback to SimpleJson if not available
#ifdef HAS_NLOHMANN_JSON
#include <nlohmann/json.hpp>
using JsonType = nlohmann::json;
#else
#include "utils/SimpleJson.h"
using JsonType = SimpleJson;
#endif

enum class MessageType {
    TASK_REQUEST,
    TASK_RESPONSE,
    STATUS_UPDATE,
    ERROR,
    HEARTBEAT,
    CONFIG_UPDATE
};

class Message {
public:
    Message();
    Message(const std::string& senderId, 
            const std::string& receiverId, 
            MessageType type, 
            const std::string& content);

    // Getters
    const std::string& getId() const { return m_id; }
    const std::string& getSenderId() const { return m_senderId; }
    const std::string& getReceiverId() const { return m_receiverId; }
    MessageType getType() const { return m_type; }
    const std::string& getContent() const { return m_content; }
    const std::chrono::system_clock::time_point& getTimestamp() const { return m_timestamp; }

    // Setters
    void setSenderId(const std::string& senderId) { m_senderId = senderId; }
    void setReceiverId(const std::string& receiverId) { m_receiverId = receiverId; }
    void setType(MessageType type) { m_type = type; }
    void setContent(const std::string& content) { m_content = content; }

    // Serialization
    JsonType toJson() const;
    static Message fromJson(const JsonType& json);

    // Utility methods
    std::string toString() const;
    bool isValid() const;

private:
    std::string m_id;
    std::string m_senderId;
    std::string m_receiverId;
    MessageType m_type;
    std::string m_content;
    std::chrono::system_clock::time_point m_timestamp;

    // Private helper methods
    std::string generateId();
    std::string messageTypeToString(MessageType type) const;
    MessageType stringToMessageType(const std::string& typeStr) const;
    std::string getCurrentTimestamp() const;
}; 