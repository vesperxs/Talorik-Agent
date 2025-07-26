#include "agents/SecurityAgent.h"
#include "config/ConfigManager.h"
#include "utils/Logger.h"
#include <random>
#include <sstream>
#include <iomanip>
#include <algorithm>

using json = nlohmann::json;

SecurityAgent::SecurityAgent(ConfigManager* configManager)
    : m_configManager(configManager)
    , m_running(false)
    , m_apiServerRunning(false)
    , m_totalThreats(0)
    , m_blockedAttacks(0)
    , m_activeAlerts(0)
    , m_startTime(std::chrono::system_clock::now())
    , m_lastScanTime(std::chrono::system_clock::now()) {
}

SecurityAgent::~SecurityAgent() {
    shutdown();
}

bool SecurityAgent::initialize() {
    Logger::info("Initializing SecurityAgent");
    
    // Initialize simulated data
    generateSimulatedData();
    
    // Start data collection thread
    m_running = true;
    m_dataCollectionThread = std::thread(&SecurityAgent::runDataCollection, this);
    
    Logger::info("SecurityAgent initialized successfully");
    return true;
}

void SecurityAgent::run() {
    Logger::info("SecurityAgent starting API server");
    startApiServer();
}

void SecurityAgent::shutdown() {
    Logger::info("SecurityAgent shutting down");
    
    m_running = false;
    stopApiServer();
    
    if (m_dataCollectionThread.joinable()) {
        m_dataCollectionThread.join();
    }
}

void SecurityAgent::startApiServer() {
    if (m_apiServerRunning) {
        return;
    }
    
    m_apiServerRunning = true;
    m_apiServerThread = std::thread(&SecurityAgent::runApiServer, this);
}

void SecurityAgent::stopApiServer() {
    m_apiServerRunning = false;
    if (m_apiServerThread.joinable()) {
        m_apiServerThread.join();
    }
}

void SecurityAgent::runApiServer() {
    try {
        // Create HTTP server
        m_httpServer = std::make_unique<HttpServer>(8080);
        
        // Setup API routes
        setupApiRoutes();
        
        // Start server
        Logger::info("Starting API server on port 8080");
        m_httpServer->start();
        
    } catch (const std::exception& e) {
        Logger::error("API server error: " + std::string(e.what()));
    }
}

void SecurityAgent::setupApiRoutes() {
    // Security Metrics endpoint
    m_httpServer->addRoute("GET", "/api/security/metrics", 
        [this](const std::string& path, const std::map<std::string, std::string>& params) {
            return handleSecurityMetrics(path, params);
        });
    
    // Threat Data endpoint
    m_httpServer->addRoute("GET", "/api/threats/data", 
        [this](const std::string& path, const std::map<std::string, std::string>& params) {
            return handleThreatData(path, params);
        });
    
    // Attack Types endpoint
    m_httpServer->addRoute("GET", "/api/threats/attack-types", 
        [this](const std::string& path, const std::map<std::string, std::string>& params) {
            return handleAttackTypes(path, params);
        });
    
    // Recent Alerts endpoint
    m_httpServer->addRoute("GET", "/api/alerts/recent", 
        [this](const std::string& path, const std::map<std::string, std::string>& params) {
            return handleRecentAlerts(path, params);
        });
    
    // System Status endpoint
    m_httpServer->addRoute("GET", "/api/system/status", 
        [this](const std::string& path, const std::map<std::string, std::string>& params) {
            return handleSystemStatus(path, params);
        });
    
    // Agent Status endpoint
    m_httpServer->addRoute("GET", "/api/agent/status", 
        [this](const std::string& path, const std::map<std::string, std::string>& params) {
            return handleAgentStatus(path, params);
        });
    
    // Security Scan endpoint
    m_httpServer->addRoute("POST", "/api/security/scan", 
        [this](const std::string& path, const std::map<std::string, std::string>& params) {
            return handleSecurityScan(path, params);
        });
}

void SecurityAgent::runDataCollection() {
    while (m_running) {
        try {
            // Simulate data collection
            generateSimulatedData();
            updateSecurityMetrics();
            
            // Sleep for 30 seconds
            std::this_thread::sleep_for(std::chrono::seconds(30));
            
        } catch (const std::exception& e) {
            Logger::error("Data collection error: " + std::string(e.what()));
        }
    }
}

void SecurityAgent::generateSimulatedData() {
    std::lock_guard<std::mutex> lock(m_dataMutex);
    
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> threatDist(5, 50);
    static std::uniform_int_distribution<> alertDist(0, 5);
    
    // Generate threat data point
    ThreatDataPoint point;
    point.timestamp = getCurrentTimestamp();
    point.total_threats = threatDist(gen);
    point.blocked_threats = point.total_threats - std::uniform_int_distribution<>(0, 3)(gen);
    
    // Random attack types
    std::vector<std::string> attackTypes = {"ddos", "sql_injection", "xss", "brute_force", "malware"};
    std::shuffle(attackTypes.begin(), attackTypes.end(), gen);
    int numTypes = std::uniform_int_distribution<>(1, 3)(gen);
    for (int i = 0; i < numTypes; ++i) {
        point.attack_types.push_back(attackTypes[i]);
    }
    
    m_threatHistory.push_back(point);
    
    // Keep only last 1000 points
    if (m_threatHistory.size() > 1000) {
        m_threatHistory.erase(m_threatHistory.begin());
    }
    
    // Generate random alerts
    if (alertDist(gen) == 0) {
        Alert alert;
        alert.id = m_alerts.size() + 1;
        alert.severity = (std::uniform_int_distribution<>(0, 3)(gen) == 0) ? "critical" : 
                        (std::uniform_int_distribution<>(0, 2)(gen) == 0) ? "high" : "medium";
        alert.description = "Simulated security alert #" + std::to_string(alert.id);
        alert.timestamp = getCurrentTimestamp();
        alert.source_ip = "192.168.1." + std::to_string(std::uniform_int_distribution<>(1, 254)(gen));
        alert.source = alert.source_ip;
        
        m_alerts.push_back(alert);
        
        // Keep only last 100 alerts
        if (m_alerts.size() > 100) {
            m_alerts.erase(m_alerts.begin());
        }
    }
}

void SecurityAgent::updateSecurityMetrics() {
    std::lock_guard<std::mutex> lock(m_dataMutex);
    
    // Update statistics
    m_totalThreats = 0;
    m_blockedAttacks = 0;
    
    for (const auto& point : m_threatHistory) {
        m_totalThreats += point.total_threats;
        m_blockedAttacks += point.blocked_threats;
    }
    
    m_activeAlerts = m_alerts.size();
}

SecurityMetrics SecurityAgent::getSecurityMetrics() const {
    std::lock_guard<std::mutex> lock(m_dataMutex);
    
    SecurityMetrics metrics;
    metrics.totalThreats = m_totalThreats.load();
    metrics.blockedAttacks = m_blockedAttacks.load();
    metrics.activeAlerts = m_activeAlerts.load();
    metrics.securityScore = calculateSecurityScore();
    metrics.uptime = 99.9; // Simulated
    metrics.lastScan = getCurrentTimestamp();
    
    return metrics;
}

std::vector<ThreatDataPoint> SecurityAgent::getThreatData(const std::string& range) const {
    std::lock_guard<std::mutex> lock(m_dataMutex);
    
    // For demo, return last 24 points (simulating hourly data)
    size_t count = 24;
    if (range == "1h") count = 1;
    else if (range == "6h") count = 6;
    else if (range == "12h") count = 12;
    else if (range == "7d") count = 168; // 7 * 24
    
    if (m_threatHistory.size() <= count) {
        return m_threatHistory;
    }
    
    return std::vector<ThreatDataPoint>(
        m_threatHistory.end() - count,
        m_threatHistory.end()
    );
}

std::vector<AttackTypeDistribution> SecurityAgent::getAttackTypeDistribution() const {
    std::lock_guard<std::mutex> lock(m_dataMutex);
    
    std::map<std::string, int> attackCounts;
    int totalAttacks = 0;
    
    // Count attack types from recent data
    for (const auto& point : m_threatHistory) {
        for (const auto& type : point.attack_types) {
            attackCounts[type]++;
            totalAttacks++;
        }
    }
    
    std::vector<AttackTypeDistribution> distribution;
    for (const auto& [type, count] : attackCounts) {
        AttackTypeDistribution dist;
        dist.attack_type = type;
        dist.count = count;
        dist.percentage = totalAttacks > 0 ? (count * 100.0 / totalAttacks) : 0.0;
        distribution.push_back(dist);
    }
    
    // Sort by count descending
    std::sort(distribution.begin(), distribution.end(),
              [](const AttackTypeDistribution& a, const AttackTypeDistribution& b) {
                  return a.count > b.count;
              });
    
    return distribution;
}

std::vector<Alert> SecurityAgent::getRecentAlerts(int limit) const {
    std::lock_guard<std::mutex> lock(m_dataMutex);
    
    if (m_alerts.size() <= limit) {
        return m_alerts;
    }
    
    return std::vector<Alert>(
        m_alerts.end() - limit,
        m_alerts.end()
    );
}

std::vector<SystemStatus> SecurityAgent::getSystemStatus() const {
    std::lock_guard<std::mutex> lock(m_dataMutex);
    
    if (m_systemStatus.empty()) {
        // Initialize system status
        m_systemStatus = {
            {"Web Server", "online", "99.9%", "server"},
            {"Database", "online", "99.8%", "database"},
            {"Firewall", "online", "100%", "shield"},
            {"Load Balancer", "online", "99.7%", "balance-scale"},
            {"Monitoring", "online", "99.9%", "eye"}
        };
    }
    
    return m_systemStatus;
}

AgentStatus SecurityAgent::getAgentStatus() const {
    AgentStatus status;
    status.connected = m_running.load();
    status.lastHeartbeat = getCurrentTimestamp();
    status.version = "1.2.3";
    status.uptime = formatUptime();
    return status;
}

ScanResponse SecurityAgent::triggerSecurityScan(const ScanRequest& request) {
    Logger::info("Triggering security scan: " + request.type);
    
    ScanResponse response;
    response.scanId = "scan_" + std::to_string(std::chrono::system_clock::now().time_since_epoch().count());
    response.status = "started";
    
    if (request.type == "full") {
        response.estimatedDuration = "5m";
    } else if (request.type == "quick") {
        response.estimatedDuration = "1m";
    } else {
        response.estimatedDuration = "3m";
    }
    
    m_lastScanTime = std::chrono::system_clock::now();
    
    return response;
}

void SecurityAgent::broadcastWebSocketMessage(const WebSocketMessage& message) {
    // WebSocket broadcasting would be implemented here
    Logger::info("Broadcasting WebSocket message: " + message.type);
}

std::string SecurityAgent::getCurrentTimestamp() const {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;
    
    std::stringstream ss;
    ss << std::put_time(std::gmtime(&time_t), "%Y-%m-%dT%H:%M:%S");
    ss << '.' << std::setfill('0') << std::setw(3) << ms.count() << 'Z';
    return ss.str();
}

std::string SecurityAgent::formatUptime() const {
    auto now = std::chrono::system_clock::now();
    auto duration = now - m_startTime;
    
    auto hours = std::chrono::duration_cast<std::chrono::hours>(duration);
    auto minutes = std::chrono::duration_cast<std::chrono::minutes>(duration) % 60;
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration) % 60;
    
    std::stringstream ss;
    ss << hours.count() << "h " << minutes.count() << "m " << seconds.count() << "s";
    return ss.str();
}

double SecurityAgent::calculateSecurityScore() const {
    if (m_totalThreats.load() == 0) {
        return 100.0;
    }
    
    double blockedRatio = static_cast<double>(m_blockedAttacks.load()) / m_totalThreats.load();
    double baseScore = blockedRatio * 100.0;
    
    // Penalize for active alerts
    double alertPenalty = m_activeAlerts.load() * 2.0;
    
    return std::max(0.0, std::min(100.0, baseScore - alertPenalty));
}

// API Handler implementations
std::string SecurityAgent::handleSecurityMetrics(const std::string& path, const std::map<std::string, std::string>& params) {
    auto metrics = getSecurityMetrics();
    return metrics.toJson().dump();
}

std::string SecurityAgent::handleThreatData(const std::string& path, const std::map<std::string, std::string>& params) {
    std::string range = "24h";
    auto it = params.find("range");
    if (it != params.end()) {
        range = it->second;
    }
    
    auto data = getThreatData(range);
    json response = json::array();
    for (const auto& point : data) {
        response.push_back(point.toJson());
    }
    return response.dump();
}

std::string SecurityAgent::handleAttackTypes(const std::string& path, const std::map<std::string, std::string>& params) {
    auto data = getAttackTypeDistribution();
    json response = json::array();
    for (const auto& dist : data) {
        response.push_back(dist.toJson());
    }
    return response.dump();
}

std::string SecurityAgent::handleRecentAlerts(const std::string& path, const std::map<std::string, std::string>& params) {
    int limit = 10;
    auto it = params.find("limit");
    if (it != params.end()) {
        try {
            limit = std::stoi(it->second);
        } catch (...) {
            limit = 10;
        }
    }
    
    auto alerts = getRecentAlerts(limit);
    json response = json::array();
    for (const auto& alert : alerts) {
        response.push_back(alert.toJson());
    }
    return response.dump();
}

std::string SecurityAgent::handleSystemStatus(const std::string& path, const std::map<std::string, std::string>& params) {
    auto status = getSystemStatus();
    json response = json::array();
    for (const auto& sys : status) {
        response.push_back(sys.toJson());
    }
    return response.dump();
}

std::string SecurityAgent::handleAgentStatus(const std::string& path, const std::map<std::string, std::string>& params) {
    auto status = getAgentStatus();
    return status.toJson().dump();
}

std::string SecurityAgent::handleSecurityScan(const std::string& path, const std::map<std::string, std::string>& params) {
    try {
        // For POST requests, we would need to parse the body
        // For now, we'll use a default scan type
        ScanRequest scanRequest;
        scanRequest.type = "full";
        
        auto response = triggerSecurityScan(scanRequest);
        return response.toJson().dump();
    } catch (const std::exception& e) {
        return "{\"error\": \"Invalid request format\"}";
    }
} 