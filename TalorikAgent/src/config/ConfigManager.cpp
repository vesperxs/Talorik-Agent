#include "config/ConfigManager.h"
#include "utils/Logger.h"
#include <fstream>
#include <iostream>

ConfigManager::ConfigManager() : m_logLevel(LogLevel::INFO) {
    setDefaultConfig();
}

bool ConfigManager::loadConfig(const std::string& configPath) {
    try {
        std::ifstream file(configPath);
        if (!file.is_open()) {
            Logger::warning("Could not open config file: " + configPath + ", using defaults");
            return false;
        }
        
        std::string content((std::istreambuf_iterator<char>(file)),
                           std::istreambuf_iterator<char>());
        
        return loadConfigFromString(content);
        
    } catch (const std::exception& e) {
        Logger::error("Error loading config: " + std::string(e.what()));
        return false;
    }
}

bool ConfigManager::saveConfig(const std::string& configPath) {
    try {
        std::ofstream file(configPath);
        if (!file.is_open()) {
            Logger::error("Could not open config file for writing: " + configPath);
            return false;
        }
        
        file << m_config.dump(2);
        return true;
        
    } catch (const std::exception& e) {
        Logger::error("Error saving config: " + std::string(e.what()));
        return false;
    }
}

bool ConfigManager::loadConfigFromString(const std::string& configJson) {
    try {
        m_config = nlohmann::json::parse(configJson);
        
        // Parse log level
        std::string logLevelStr = getString("logging.level", "info");
        parseLogLevel(logLevelStr);
        
        return validateConfig();
        
    } catch (const std::exception& e) {
        Logger::error("Error parsing config JSON: " + std::string(e.what()));
        return false;
    }
}

std::string ConfigManager::getString(const std::string& key, const std::string& defaultValue) const {
    try {
        return m_config.value(key, defaultValue);
    } catch (...) {
        return defaultValue;
    }
}

int ConfigManager::getInt(const std::string& key, int defaultValue) const {
    try {
        return m_config.value(key, defaultValue);
    } catch (...) {
        return defaultValue;
    }
}

double ConfigManager::getDouble(const std::string& key, double defaultValue) const {
    try {
        return m_config.value(key, defaultValue);
    } catch (...) {
        return defaultValue;
    }
}

bool ConfigManager::getBool(const std::string& key, bool defaultValue) const {
    try {
        return m_config.value(key, defaultValue);
    } catch (...) {
        return defaultValue;
    }
}

LogLevel ConfigManager::getLogLevel() const {
    return m_logLevel;
}

void ConfigManager::setString(const std::string& key, const std::string& value) {
    m_config[key] = value;
}

void ConfigManager::setInt(const std::string& key, int value) {
    m_config[key] = value;
}

void ConfigManager::setDouble(const std::string& key, double value) {
    m_config[key] = value;
}

void ConfigManager::setBool(const std::string& key, bool value) {
    m_config[key] = value;
}

bool ConfigManager::validateConfig() const {
    // Basic validation - check if required fields exist
    if (m_config.empty()) {
        Logger::warning("Config is empty, using defaults");
        return false;
    }
    
    return true;
}

void ConfigManager::setDefaultConfig() {
    m_config = {
        {"logging", {
            {"level", "info"},
            {"file", "talorik_agent.log"}
        }},
        {"api", {
            {"port", 8080},
            {"host", "0.0.0.0"}
        }},
        {"security", {
            {"dataCollectionInterval", 30},
            {"maxThreatHistory", 1000},
            {"maxAlerts", 100}
        }}
    };
}

bool ConfigManager::parseLogLevel(const std::string& levelStr) {
    std::string level = levelStr;
    std::transform(level.begin(), level.end(), level.begin(), ::tolower);
    
    if (level == "debug") {
        m_logLevel = LogLevel::DEBUG;
    } else if (level == "info") {
        m_logLevel = LogLevel::INFO;
    } else if (level == "warning" || level == "warn") {
        m_logLevel = LogLevel::WARNING;
    } else if (level == "error") {
        m_logLevel = LogLevel::ERROR;
    } else {
        Logger::warning("Unknown log level: " + levelStr + ", using INFO");
        m_logLevel = LogLevel::INFO;
        return false;
    }
    
    return true;
} 