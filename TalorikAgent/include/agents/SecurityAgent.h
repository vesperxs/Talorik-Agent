#pragma once

#include <memory>
#include <vector>
#include <string>
#include <chrono>
#include <thread>
#include <atomic>
#include <mutex>
#include "agents/Agent.h"
#include "models/SecurityModels.h"
#include "utils/Logger.h"
#include "network/HttpServer.h"

// Forward declarations
class ConfigManager;

class SecurityAgent : public Agent {
public:
    explicit SecurityAgent(ConfigManager* configManager);
    ~SecurityAgent() override;

    // Agent interface implementation
    bool initialize() override;
    void run() override;
    void shutdown() override;
    std::string getAgentId() const override { return "security_agent"; }
    std::string getAgentType() const override { return "security"; }

    // Security-specific methods
    void startApiServer();
    void stopApiServer();
    
    // Data collection methods (simulated for demo)
    SecurityMetrics getSecurityMetrics() const;
    std::vector<ThreatDataPoint> getThreatData(const std::string& range) const;
    std::vector<AttackTypeDistribution> getAttackTypeDistribution() const;
    std::vector<Alert> getRecentAlerts(int limit = 10) const;
    std::vector<SystemStatus> getSystemStatus() const;
    AgentStatus getAgentStatus() const;
    ScanResponse triggerSecurityScan(const ScanRequest& request);

    // WebSocket support
    void broadcastWebSocketMessage(const WebSocketMessage& message);

private:
    ConfigManager* m_configManager;
    std::atomic<bool> m_running;
    std::atomic<bool> m_apiServerRunning;
    std::thread m_apiServerThread;
    std::thread m_dataCollectionThread;
    
    // Simulated data storage
    mutable std::mutex m_dataMutex;
    std::vector<ThreatDataPoint> m_threatHistory;
    std::vector<Alert> m_alerts;
    std::vector<SystemStatus> m_systemStatus;
    
    // Statistics
    std::atomic<int> m_totalThreats;
    std::atomic<int> m_blockedAttacks;
    std::atomic<int> m_activeAlerts;
    std::chrono::system_clock::time_point m_startTime;
    std::chrono::system_clock::time_point m_lastScanTime;
    
    // Private helper methods
    void runApiServer();
    void runDataCollection();
    void generateSimulatedData();
    void updateSecurityMetrics();
    std::string getCurrentTimestamp() const;
    std::string formatUptime() const;
    double calculateSecurityScore() const;
    
    // API endpoint handlers
    void setupApiRoutes();
    std::string handleSecurityMetrics(const std::string& path, const std::map<std::string, std::string>& params);
    std::string handleThreatData(const std::string& path, const std::map<std::string, std::string>& params);
    std::string handleAttackTypes(const std::string& path, const std::map<std::string, std::string>& params);
    std::string handleRecentAlerts(const std::string& path, const std::map<std::string, std::string>& params);
    std::string handleSystemStatus(const std::string& path, const std::map<std::string, std::string>& params);
    std::string handleAgentStatus(const std::string& path, const std::map<std::string, std::string>& params);
    std::string handleSecurityScan(const std::string& path, const std::map<std::string, std::string>& params);
    
    // HTTP server
    std::unique_ptr<HttpServer> m_httpServer;
}; 