#pragma once

#include <string>
#include <memory>
#include <fstream>
#include <mutex>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <ctime>

class Logger {
public:
    enum class Level {
        DEBUG,
        INFO,
        WARNING,
        ERROR
    };

    // Initialize the logger
    static void initialize(Level level = Level::INFO, 
                          const std::string& logFile = "talorik_agent.log");

    // Shutdown the logger
    static void shutdown();

    // Logging methods
    static void debug(const std::string& message);
    static void info(const std::string& message);
    static void warning(const std::string& message);
    static void error(const std::string& message);

    // Set log level
    static void setLevel(Level level);

    // Check if logging is enabled for a level
    static bool isEnabled(Level level);

private:
    static Logger* s_instance;
    static std::mutex s_mutex;

    Level m_level;
    std::ofstream m_logFile;
    bool m_initialized;

    Logger();
    ~Logger();

    void log(Level level, const std::string& message);
    std::string getCurrentTimestamp();
    std::string levelToString(Level level);
}; 