#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <map>
#include <vector>
#include <random>
#include <algorithm>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

// Simple Logger
class Logger {
public:
    enum class Level { DEBUG, INFO, WARNING, ERROR_LEVEL };
    
    static void initialize(Level level = Level::INFO) {
        s_level = level;
    }
    
    static void info(const std::string& message) {
        if (s_level <= Level::INFO) {
            std::cout << getTimestamp() << " [INFO] " << message << std::endl;
        }
    }
    
    static void warning(const std::string& message) {
        if (s_level <= Level::WARNING) {
            std::cout << getTimestamp() << " [WARNING] " << message << std::endl;
        }
    }
    
    static void error(const std::string& message) {
        if (s_level <= Level::ERROR_LEVEL) {
            std::cout << getTimestamp() << " [ERROR] " << message << std::endl;
        }
    }

private:
    static Level s_level;
    
    static std::string getTimestamp() {
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch()) % 1000;
        
        std::stringstream ss;
        ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
        ss << '.' << std::setfill('0') << std::setw(3) << ms.count();
        return ss.str();
    }
};

Logger::Level Logger::s_level = Logger::Level::INFO;

// Simple JSON
class SimpleJson {
public:
    enum class Type { Null, String, Number, Boolean, Object, Array };
    
    SimpleJson() : m_type(Type::Null) {}
    SimpleJson(const std::string& value) : m_type(Type::String), m_stringValue(value) {}
    SimpleJson(int value) : m_type(Type::Number), m_numberValue(value) {}
    SimpleJson(double value) : m_type(Type::Number), m_doubleValue(value), m_isDouble(true) {}
    SimpleJson(bool value) : m_type(Type::Boolean), m_boolValue(value) {}
    
    void set(const std::string& key, const std::string& value) {
        m_type = Type::Object;
        m_objectValue[key] = SimpleJson(value);
    }
    
    void set(const std::string& key, int value) {
        m_type = Type::Object;
        m_objectValue[key] = SimpleJson(value);
    }
    
    void set(const std::string& key, double value) {
        m_type = Type::Object;
        m_objectValue[key] = SimpleJson(value);
    }
    
    void set(const std::string& key, bool value) {
        m_type = Type::Object;
        m_objectValue[key] = SimpleJson(value);
    }
    
    void set(const std::string& key, const SimpleJson& value) {
        m_type = Type::Object;
        m_objectValue[key] = value;
    }
    
    void addToArray(const SimpleJson& value) {
        if (m_type != Type::Array) {
            m_type = Type::Array;
            m_arrayValue.clear();
        }
        m_arrayValue.push_back(value);
    }
    
    std::string toString() const {
        switch (m_type) {
            case Type::Null: return "null";
            case Type::String: return "\"" + escapeString(m_stringValue) + "\"";
            case Type::Number: 
                if (m_isDouble) {
                    std::ostringstream oss;
                    oss << std::fixed << std::setprecision(1) << m_doubleValue;
                    return oss.str();
                }
                return std::to_string(m_numberValue);
            case Type::Boolean: return m_boolValue ? "true" : "false";
            case Type::Object: {
                std::ostringstream oss;
                oss << "{";
                bool first = true;
                for (const auto& pair : m_objectValue) {
                    if (!first) oss << ",";
                    oss << "\"" << escapeString(pair.first) << "\":" << pair.second.toString();
                    first = false;
                }
                oss << "}";
                return oss.str();
            }
            case Type::Array: {
                std::ostringstream oss;
                oss << "[";
                bool first = true;
                for (const auto& item : m_arrayValue) {
                    if (!first) oss << ",";
                    oss << item.toString();
                    first = false;
                }
                oss << "]";
                return oss.str();
            }
            default: return "null";
        }
    }

private:
    Type m_type;
    std::string m_stringValue;
    int m_numberValue;
    double m_doubleValue;
    bool m_boolValue;
    bool m_isDouble = false;
    std::map<std::string, SimpleJson> m_objectValue;
    std::vector<SimpleJson> m_arrayValue;
    
    static std::string escapeString(const std::string& str) {
        std::string result;
        for (char c : str) {
            switch (c) {
                case '"': result += "\\\""; break;
                case '\\': result += "\\\\"; break;
                case '\n': result += "\\n"; break;
                case '\r': result += "\\r"; break;
                case '\t': result += "\\t"; break;
                default: result += c; break;
            }
        }
        return result;
    }
};

// Security Data Structures
struct ThreatData {
    std::string timestamp;
    int total_threats;
    int blocked_threats;
    std::vector<std::string> attack_types;
};

struct Alert {
    int id;
    std::string severity;
    std::string description;
    std::string timestamp;
    std::string source_ip;
    std::string source;
};

struct SystemStatus {
    std::string name;
    std::string status;
    std::string uptime;
    std::string icon;
};

struct AttackTypeData {
    std::string attack_type;
    int count;
    double percentage;
};

// Security Agent Class
class SecurityAgent {
private:
    std::vector<ThreatData> threatHistory;
    std::vector<Alert> alerts;
    std::vector<SystemStatus> systemStatuses;
    std::vector<AttackTypeData> attackTypes;
    std::chrono::system_clock::time_point startTime;
    std::chrono::system_clock::time_point lastScanTime;
    int totalThreats = 0;
    int blockedAttacks = 0;
    int activeAlerts = 0;
    std::string version = "1.2.3";
    
public:
    SecurityAgent() {
        startTime = std::chrono::system_clock::now();
        lastScanTime = startTime;
        initializeMockData();
    }
    
    void initializeMockData() {
        // Initialize threat history
        auto now = std::chrono::system_clock::now();
        for (int i = 23; i >= 0; i--) {
            auto time = now - std::chrono::hours(i);
            ThreatData threat;
            threat.timestamp = getTimestampString(time);
            threat.total_threats = 10 + (rand() % 20);
            threat.blocked_threats = threat.total_threats - (rand() % 3);
            threat.attack_types = getRandomAttackTypes();
            threatHistory.push_back(threat);
            
            totalThreats += threat.total_threats;
            blockedAttacks += threat.blocked_threats;
        }
        
        // Initialize alerts
        std::vector<std::string> severities = {"critical", "high", "medium", "low"};
        std::vector<std::string> descriptions = {
            "Multiple failed login attempts detected",
            "Suspicious file upload detected",
            "Unusual network traffic pattern",
            "Potential SQL injection attempt",
            "Cross-site scripting attack detected",
            "Brute force attack in progress",
            "DDoS attack detected",
            "Malware signature detected",
            "Unauthorized access attempt",
            "Data exfiltration attempt"
        };
        
        for (int i = 0; i < 10; i++) {
            Alert alert;
            alert.id = i + 1;
            alert.severity = severities[rand() % severities.size()];
            alert.description = descriptions[rand() % descriptions.size()];
            alert.timestamp = getTimestampString(now - std::chrono::minutes(rand() % 60));
            alert.source_ip = "192.168.1." + std::to_string(100 + rand() % 100);
            alert.source = alert.source_ip;
            alerts.push_back(alert);
        }
        
        // Initialize system statuses
        systemStatuses = {
            {"Web Server", "online", "99.9%", "server"},
            {"Database", "online", "99.8%", "database"},
            {"Firewall", "online", "100%", "shield"},
            {"Load Balancer", "online", "99.7%", "balance"},
            {"Cache Server", "online", "99.9%", "cache"}
        };
        
        // Initialize attack types
        std::vector<std::string> attackTypeNames = {"ddos", "sql_injection", "xss", "brute_force", "malware", "phishing"};
        int total = 100;
        for (const auto& type : attackTypeNames) {
            AttackTypeData data;
            data.attack_type = type;
            data.count = 10 + (rand() % 20);
            data.percentage = (double)data.count / total * 100;
            attackTypes.push_back(data);
        }
        
        activeAlerts = alerts.size();
    }
    
    // API Endpoints
    SimpleJson getSecurityMetrics() {
        SimpleJson response;
        response.set("totalThreats", totalThreats);
        response.set("blockedAttacks", blockedAttacks);
        response.set("activeAlerts", activeAlerts);
        response.set("securityScore", calculateSecurityScore());
        response.set("uptime", getUptime());
        response.set("lastScan", getTimestampString(lastScanTime));
        return response;
    }
    
    SimpleJson getThreatData(const std::string& range) {
        SimpleJson response;
        response = SimpleJson(); // Reset to array type
        
        int hours = 24; // default
        if (range == "1h") hours = 1;
        else if (range == "6h") hours = 6;
        else if (range == "12h") hours = 12;
        else if (range == "7d") hours = 168;
        
        auto now = std::chrono::system_clock::now();
        auto cutoff = now - std::chrono::hours(hours);
        
        for (const auto& threat : threatHistory) {
            auto threatTime = parseTimestamp(threat.timestamp);
            if (threatTime >= cutoff) {
                SimpleJson threatJson;
                threatJson.set("timestamp", threat.timestamp);
                threatJson.set("total_threats", threat.total_threats);
                threatJson.set("blocked_threats", threat.blocked_threats);
                
                SimpleJson attackTypesArray;
                for (const auto& type : threat.attack_types) {
                    attackTypesArray.addToArray(SimpleJson(type));
                }
                threatJson.set("attack_types", attackTypesArray);
                
                response.addToArray(threatJson);
            }
        }
        
        return response;
    }
    
    SimpleJson getAttackTypes() {
        SimpleJson response;
        response = SimpleJson(); // Reset to array type
        
        for (const auto& attack : attackTypes) {
            SimpleJson attackJson;
            attackJson.set("attack_type", attack.attack_type);
            attackJson.set("count", attack.count);
            attackJson.set("percentage", attack.percentage);
            response.addToArray(attackJson);
        }
        
        return response;
    }
    
    SimpleJson getRecentAlerts(int limit = 10) {
        SimpleJson response;
        response = SimpleJson(); // Reset to array type
        
        int count = 0;
        for (const auto& alert : alerts) {
            if (count >= limit) break;
            
            SimpleJson alertJson;
            alertJson.set("id", alert.id);
            alertJson.set("severity", alert.severity);
            alertJson.set("description", alert.description);
            alertJson.set("timestamp", alert.timestamp);
            alertJson.set("source_ip", alert.source_ip);
            alertJson.set("source", alert.source);
            response.addToArray(alertJson);
            count++;
        }
        
        return response;
    }
    
    SimpleJson getSystemStatus() {
        SimpleJson response;
        response = SimpleJson(); // Reset to array type
        
        for (const auto& status : systemStatuses) {
            SimpleJson statusJson;
            statusJson.set("name", status.name);
            statusJson.set("status", status.status);
            statusJson.set("uptime", status.uptime);
            statusJson.set("icon", status.icon);
            response.addToArray(statusJson);
        }
        
        return response;
    }
    
    SimpleJson getAgentStatus() {
        SimpleJson response;
        response.set("connected", true);
        response.set("lastHeartbeat", getTimestampString(std::chrono::system_clock::now()));
        response.set("version", version);
        response.set("uptime", getUptimeString());
        return response;
    }
    
    SimpleJson startSecurityScan(const std::string& type) {
        lastScanTime = std::chrono::system_clock::now();
        
        SimpleJson response;
        response.set("scanId", "scan_" + generateId());
        response.set("status", "started");
        response.set("estimatedDuration", type == "full" ? "5m" : "2m");
        
        Logger::info("Security scan started: " + type);
        return response;
    }

private:
    std::vector<std::string> getRandomAttackTypes() {
        std::vector<std::string> allTypes = {"ddos", "sql_injection", "xss", "brute_force", "malware", "phishing"};
        std::vector<std::string> result;
        int count = 1 + (rand() % 3);
        
        for (int i = 0; i < count; i++) {
            result.push_back(allTypes[rand() % allTypes.size()]);
        }
        
        return result;
    }
    
    double calculateSecurityScore() {
        if (totalThreats == 0) return 100.0;
        double blockedRate = (double)blockedAttacks / totalThreats;
        return std::min(100.0, blockedRate * 100 + 20); // Base score of 20
    }
    
    double getUptime() {
        auto now = std::chrono::system_clock::now();
        auto uptime = std::chrono::duration_cast<std::chrono::hours>(now - startTime);
        return 99.9; // Mock uptime
    }
    
    std::string getUptimeString() {
        auto now = std::chrono::system_clock::now();
        auto uptime = now - startTime;
        auto hours = std::chrono::duration_cast<std::chrono::hours>(uptime);
        auto minutes = std::chrono::duration_cast<std::chrono::minutes>(uptime) % 60;
        auto seconds = std::chrono::duration_cast<std::chrono::seconds>(uptime) % 60;
        
        std::ostringstream oss;
        oss << hours.count() << "h " << minutes.count() << "m " << seconds.count() << "s";
        return oss.str();
    }
    
    std::string getTimestampString(const std::chrono::system_clock::time_point& time) {
        auto time_t = std::chrono::system_clock::to_time_t(time);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            time.time_since_epoch()) % 1000;
        
        std::stringstream ss;
        ss << std::put_time(std::gmtime(&time_t), "%Y-%m-%dT%H:%M:%S");
        ss << '.' << std::setfill('0') << std::setw(3) << ms.count() << 'Z';
        return ss.str();
    }
    
    std::chrono::system_clock::time_point parseTimestamp(const std::string& timestamp) {
        std::tm tm = {};
        std::istringstream ss(timestamp);
        ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%S");
        return std::chrono::system_clock::from_time_t(std::mktime(&tm));
    }
    
    std::string generateId() {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<> dis(10000, 99999);
        return std::to_string(dis(gen));
    }
};

// HTTP Server with API Routes
class SecurityApiServer {
public:
    SecurityApiServer(int port = 8080) : m_port(port), m_running(false), m_agent() {
        // Initialize Winsock
        WSADATA wsaData;
        int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (result != 0) {
            Logger::error("WSAStartup failed: " + std::to_string(result));
            throw std::runtime_error("WSAStartup failed");
        }
    }
    
    ~SecurityApiServer() {
        stop();
        WSACleanup();
    }
    
    void start() {
        m_running = true;
        m_serverThread = std::thread(&SecurityApiServer::runServer, this);
        Logger::info("Security API server started on port " + std::to_string(m_port));
        Logger::info("Available endpoints:");
        Logger::info("  GET  /api/security/metrics");
        Logger::info("  GET  /api/threats/data?range=24h");
        Logger::info("  GET  /api/threats/attack-types");
        Logger::info("  GET  /api/alerts/recent?limit=10");
        Logger::info("  GET  /api/system/status");
        Logger::info("  GET  /api/agent/status");
        Logger::info("  POST /api/security/scan");
    }
    
    void stop() {
        m_running = false;
        if (m_serverThread.joinable()) {
            m_serverThread.join();
        }
        Logger::info("Security API server stopped");
    }

private:
    int m_port;
    bool m_running;
    std::thread m_serverThread;
    SecurityAgent m_agent;
    
    void runServer() {
        SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSocket == INVALID_SOCKET) {
            Logger::error("Failed to create socket");
            return;
        }
        
        sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = INADDR_ANY;
        serverAddr.sin_port = htons(m_port);
        
        if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
            Logger::error("Bind failed");
            closesocket(serverSocket);
            return;
        }
        
        if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
            Logger::error("Listen failed");
            closesocket(serverSocket);
            return;
        }
        
        Logger::info("Server listening on port " + std::to_string(m_port));
        
        while (m_running) {
            SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
            if (clientSocket != INVALID_SOCKET) {
                std::thread(&SecurityApiServer::handleClient, this, clientSocket).detach();
            }
        }
        
        closesocket(serverSocket);
    }
    
    void handleClient(SOCKET clientSocket) {
        char buffer[4096];
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        
        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';
            std::string request(buffer);
            
            // Parse request and generate response
            std::string response = processRequest(request);
            
            // Send response
            send(clientSocket, response.c_str(), response.length(), 0);
        }
        
        closesocket(clientSocket);
    }
    
    std::string processRequest(const std::string& request) {
        // Simple HTTP request parsing
        std::istringstream iss(request);
        std::string method, path, version;
        iss >> method >> path >> version;
        
        Logger::info("Request: " + method + " " + path);
        
        SimpleJson response;
        std::string contentType = "application/json";
        int statusCode = 200;
        
        try {
            if (path == "/api/security/metrics" && method == "GET") {
                response = m_agent.getSecurityMetrics();
            }
            else if (path.substr(0, 18) == "/api/threats/data" && method == "GET") {
                std::string range = "24h"; // default
                size_t pos = path.find("range=");
                if (pos != std::string::npos) {
                    range = path.substr(pos + 6);
                }
                response = m_agent.getThreatData(range);
            }
            else if (path == "/api/threats/attack-types" && method == "GET") {
                response = m_agent.getAttackTypes();
            }
            else if (path.substr(0, 18) == "/api/alerts/recent" && method == "GET") {
                int limit = 10; // default
                size_t pos = path.find("limit=");
                if (pos != std::string::npos) {
                    limit = std::stoi(path.substr(pos + 6));
                }
                response = m_agent.getRecentAlerts(limit);
            }
            else if (path == "/api/system/status" && method == "GET") {
                response = m_agent.getSystemStatus();
            }
            else if (path == "/api/agent/status" && method == "GET") {
                response = m_agent.getAgentStatus();
            }
            else if (path == "/api/security/scan" && method == "POST") {
                response = m_agent.startSecurityScan("full");
            }
            else {
                // 404 Not Found
                response.set("error", "Endpoint not found");
                response.set("message", "The requested endpoint does not exist");
                statusCode = 404;
            }
        } catch (const std::exception& e) {
            response.set("error", "Internal server error");
            response.set("message", e.what());
            statusCode = 500;
        }
        
        std::string jsonResponse = response.toString();
        
        std::ostringstream httpResponse;
        httpResponse << "HTTP/1.1 " << statusCode << " " << getStatusText(statusCode) << "\r\n";
        httpResponse << "Content-Type: " << contentType << "\r\n";
        httpResponse << "Access-Control-Allow-Origin: *\r\n";
        httpResponse << "Access-Control-Allow-Methods: GET, POST, OPTIONS\r\n";
        httpResponse << "Access-Control-Allow-Headers: Content-Type\r\n";
        httpResponse << "Content-Length: " << jsonResponse.length() << "\r\n";
        httpResponse << "\r\n";
        httpResponse << jsonResponse;
        
        return httpResponse.str();
    }
    
    std::string getStatusText(int statusCode) {
        switch (statusCode) {
            case 200: return "OK";
            case 404: return "Not Found";
            case 500: return "Internal Server Error";
            default: return "Unknown";
        }
    }
};

// Main function
int main() {
    try {
        // Initialize logging
        Logger::initialize(Logger::Level::INFO);
        
        Logger::info("=== TalorikAgent Security Agent - React Dashboard API ===");
        Logger::info("Initializing security agent...");

        // Start Security API Server
        SecurityApiServer server(8080);
        server.start();

        Logger::info("=== Security Agent Ready ===");
        Logger::info("API endpoints available at http://localhost:8080/api/");
        Logger::info("Press Ctrl+C to exit");

        // Keep the main thread alive
        int counter = 0;
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(30));
            counter++;
            Logger::info("Security agent running... (cycle " + std::to_string(counter) + ")");
        }

    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
} 