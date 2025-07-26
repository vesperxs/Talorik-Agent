#pragma once

#include <string>
#include <map>
#include <memory>
#include <nlohmann/json.hpp>

enum class LogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR
};

class ConfigManager {
public:
    ConfigManager();
    ~ConfigManager() = default;

    // Configuration loading and saving
    bool loadConfig(const std::string& configPath);
    bool saveConfig(const std::string& configPath);
    bool loadConfigFromString(const std::string& configJson);

    // Configuration access
    std::string getString(const std::string& key, const std::string& defaultValue = "") const;
    int getInt(const std::string& key, int defaultValue = 0) const;
    double getDouble(const std::string& key, double defaultValue = 0.0) const;
    bool getBool(const std::string& key, bool defaultValue = false) const;
    LogLevel getLogLevel() const;

    // Configuration modification
    void setString(const std::string& key, const std::string& value);
    void setInt(const std::string& key, int value);
    void setDouble(const std::string& key, double value);
    void setBool(const std::string& key, bool value);

    // Configuration validation
    bool validateConfig() const;

private:
    nlohmann::json m_config;
    std::string m_configPath;
    LogLevel m_logLevel;

    // Private helper methods
    void setDefaultConfig();
    bool parseLogLevel(const std::string& levelStr);
}; 