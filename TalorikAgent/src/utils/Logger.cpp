#include "utils/Logger.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <mutex>

// Static member initialization
Logger* Logger::s_instance = nullptr;
std::mutex Logger::s_mutex;

Logger::Logger() : m_level(Level::INFO), m_initialized(false) {
}

Logger::~Logger() {
    if (m_logFile.is_open()) {
        m_logFile.close();
    }
}

void Logger::initialize(Level level, const std::string& logFile) {
    std::lock_guard<std::mutex> lock(s_mutex);
    
    if (s_instance) {
        delete s_instance;
    }
    
    s_instance = new Logger();
    s_instance->m_level = level;
    s_instance->m_initialized = true;
    
    if (!logFile.empty()) {
        s_instance->m_logFile.open(logFile, std::ios::app);
    }
}

void Logger::shutdown() {
    std::lock_guard<std::mutex> lock(s_mutex);
    
    if (s_instance) {
        delete s_instance;
        s_instance = nullptr;
    }
}

void Logger::debug(const std::string& message) {
    if (s_instance && s_instance->isEnabled(Level::DEBUG)) {
        s_instance->log(Level::DEBUG, message);
    }
}

void Logger::info(const std::string& message) {
    if (s_instance && s_instance->isEnabled(Level::INFO)) {
        s_instance->log(Level::INFO, message);
    }
}

void Logger::warning(const std::string& message) {
    if (s_instance && s_instance->isEnabled(Level::WARNING)) {
        s_instance->log(Level::WARNING, message);
    }
}

void Logger::error(const std::string& message) {
    if (s_instance && s_instance->isEnabled(Level::ERROR)) {
        s_instance->log(Level::ERROR, message);
    }
}

void Logger::setLevel(Level level) {
    std::lock_guard<std::mutex> lock(s_mutex);
    if (s_instance) {
        s_instance->m_level = level;
    }
}

bool Logger::isEnabled(Level level) {
    if (!s_instance) return false;
    return static_cast<int>(level) >= static_cast<int>(s_instance->m_level);
}

void Logger::log(Level level, const std::string& message) {
    std::string timestamp = getCurrentTimestamp();
    std::string levelStr = levelToString(level);
    
    std::string logMessage = timestamp + " [" + levelStr + "] " + message + "\n";
    
    // Output to console
    std::cout << logMessage;
    
    // Output to file if open
    if (m_logFile.is_open()) {
        m_logFile << logMessage;
        m_logFile.flush();
    }
}

std::string Logger::getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;
    
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
    ss << '.' << std::setfill('0') << std::setw(3) << ms.count();
    return ss.str();
}

std::string Logger::levelToString(Level level) {
    switch (level) {
        case Level::DEBUG: return "DEBUG";
        case Level::INFO: return "INFO";
        case Level::WARNING: return "WARNING";
        case Level::ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
} 