#include "models/SecurityModels.h"
#include <iomanip>
#include <sstream>

// SecurityMetrics implementation
nlohmann::json SecurityMetrics::toJson() const {
    return {
        {"totalThreats", totalThreats},
        {"blockedAttacks", blockedAttacks},
        {"activeAlerts", activeAlerts},
        {"securityScore", securityScore},
        {"uptime", uptime},
        {"lastScan", lastScan}
    };
}

SecurityMetrics SecurityMetrics::fromJson(const nlohmann::json& json) {
    SecurityMetrics metrics;
    metrics.totalThreats = json.value("totalThreats", 0);
    metrics.blockedAttacks = json.value("blockedAttacks", 0);
    metrics.activeAlerts = json.value("activeAlerts", 0);
    metrics.securityScore = json.value("securityScore", 0.0);
    metrics.uptime = json.value("uptime", 0.0);
    metrics.lastScan = json.value("lastScan", "");
    return metrics;
}

// ThreatDataPoint implementation
nlohmann::json ThreatDataPoint::toJson() const {
    return {
        {"timestamp", timestamp},
        {"total_threats", total_threats},
        {"blocked_threats", blocked_threats},
        {"attack_types", attack_types}
    };
}

ThreatDataPoint ThreatDataPoint::fromJson(const nlohmann::json& json) {
    ThreatDataPoint point;
    point.timestamp = json.value("timestamp", "");
    point.total_threats = json.value("total_threats", 0);
    point.blocked_threats = json.value("blocked_threats", 0);
    
    if (json.contains("attack_types") && json["attack_types"].is_array()) {
        for (const auto& type : json["attack_types"]) {
            point.attack_types.push_back(type.get<std::string>());
        }
    }
    
    return point;
}

// AttackTypeDistribution implementation
nlohmann::json AttackTypeDistribution::toJson() const {
    return {
        {"attack_type", attack_type},
        {"count", count},
        {"percentage", percentage}
    };
}

AttackTypeDistribution AttackTypeDistribution::fromJson(const nlohmann::json& json) {
    AttackTypeDistribution dist;
    dist.attack_type = json.value("attack_type", "");
    dist.count = json.value("count", 0);
    dist.percentage = json.value("percentage", 0.0);
    return dist;
}

// Alert implementation
nlohmann::json Alert::toJson() const {
    return {
        {"id", id},
        {"severity", severity},
        {"description", description},
        {"timestamp", timestamp},
        {"source_ip", source_ip},
        {"source", source}
    };
}

Alert Alert::fromJson(const nlohmann::json& json) {
    Alert alert;
    alert.id = json.value("id", 0);
    alert.severity = json.value("severity", "");
    alert.description = json.value("description", "");
    alert.timestamp = json.value("timestamp", "");
    alert.source_ip = json.value("source_ip", "");
    alert.source = json.value("source", "");
    return alert;
}

// SystemStatus implementation
nlohmann::json SystemStatus::toJson() const {
    return {
        {"name", name},
        {"status", status},
        {"uptime", uptime},
        {"icon", icon}
    };
}

SystemStatus SystemStatus::fromJson(const nlohmann::json& json) {
    SystemStatus status;
    status.name = json.value("name", "");
    status.status = json.value("status", "");
    status.uptime = json.value("uptime", "");
    status.icon = json.value("icon", "");
    return status;
}

// AgentStatus implementation
nlohmann::json AgentStatus::toJson() const {
    return {
        {"connected", connected},
        {"lastHeartbeat", lastHeartbeat},
        {"version", version},
        {"uptime", uptime}
    };
}

AgentStatus AgentStatus::fromJson(const nlohmann::json& json) {
    AgentStatus status;
    status.connected = json.value("connected", false);
    status.lastHeartbeat = json.value("lastHeartbeat", "");
    status.version = json.value("version", "");
    status.uptime = json.value("uptime", "");
    return status;
}

// ScanRequest implementation
nlohmann::json ScanRequest::toJson() const {
    return {
        {"type", type}
    };
}

ScanRequest ScanRequest::fromJson(const nlohmann::json& json) {
    ScanRequest request;
    request.type = json.value("type", "full");
    return request;
}

// ScanResponse implementation
nlohmann::json ScanResponse::toJson() const {
    return {
        {"scanId", scanId},
        {"status", status},
        {"estimatedDuration", estimatedDuration}
    };
}

ScanResponse ScanResponse::fromJson(const nlohmann::json& json) {
    ScanResponse response;
    response.scanId = json.value("scanId", "");
    response.status = json.value("status", "");
    response.estimatedDuration = json.value("estimatedDuration", "");
    return response;
}

// WebSocketMessage implementation
nlohmann::json WebSocketMessage::toJson() const {
    return {
        {"type", type},
        {"payload", payload}
    };
}

WebSocketMessage WebSocketMessage::fromJson(const nlohmann::json& json) {
    WebSocketMessage message;
    message.type = json.value("type", "");
    message.payload = json.value("payload", nlohmann::json::object());
    return message;
} 