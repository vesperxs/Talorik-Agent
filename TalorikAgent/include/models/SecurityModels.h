#pragma once

#include <string>
#include <vector>
#include <chrono>
#include <nlohmann/json.hpp>

// Security Metrics
struct SecurityMetrics {
    int totalThreats;
    int blockedAttacks;
    int activeAlerts;
    double securityScore;
    double uptime;
    std::string lastScan;

    nlohmann::json toJson() const;
    static SecurityMetrics fromJson(const nlohmann::json& json);
};

// Threat Data Point
struct ThreatDataPoint {
    std::string timestamp;
    int total_threats;
    int blocked_threats;
    std::vector<std::string> attack_types;

    nlohmann::json toJson() const;
    static ThreatDataPoint fromJson(const nlohmann::json& json);
};

// Attack Type Distribution
struct AttackTypeDistribution {
    std::string attack_type;
    int count;
    double percentage;

    nlohmann::json toJson() const;
    static AttackTypeDistribution fromJson(const nlohmann::json& json);
};

// Alert
struct Alert {
    int id;
    std::string severity; // "critical", "high", "medium", "low"
    std::string description;
    std::string timestamp;
    std::string source_ip;
    std::string source;

    nlohmann::json toJson() const;
    static Alert fromJson(const nlohmann::json& json);
};

// System Status
struct SystemStatus {
    std::string name;
    std::string status; // "online", "offline", "warning"
    std::string uptime;
    std::string icon;

    nlohmann::json toJson() const;
    static SystemStatus fromJson(const nlohmann::json& json);
};

// Agent Status
struct AgentStatus {
    bool connected;
    std::string lastHeartbeat;
    std::string version;
    std::string uptime;

    nlohmann::json toJson() const;
    static AgentStatus fromJson(const nlohmann::json& json);
};

// Scan Request
struct ScanRequest {
    std::string type; // "full", "quick", "targeted"

    nlohmann::json toJson() const;
    static ScanRequest fromJson(const nlohmann::json& json);
};

// Scan Response
struct ScanResponse {
    std::string scanId;
    std::string status;
    std::string estimatedDuration;

    nlohmann::json toJson() const;
    static ScanResponse fromJson(const nlohmann::json& json);
};

// WebSocket Message
struct WebSocketMessage {
    std::string type; // "threat_update", "alert_new", "metrics_update", "agent_status"
    nlohmann::json payload;

    nlohmann::json toJson() const;
    static WebSocketMessage fromJson(const nlohmann::json& json);
}; 