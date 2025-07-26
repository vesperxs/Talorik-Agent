#include "utils/SimpleJson.h"
#include <sstream>
#include <iomanip>

void SimpleJson::set(const std::string& key, const std::string& value) {
    m_type = Type::Object;
    m_objectValue[key] = SimpleJson(value);
}

void SimpleJson::set(const std::string& key, int value) {
    m_type = Type::Object;
    m_objectValue[key] = SimpleJson(value);
}

void SimpleJson::set(const std::string& key, bool value) {
    m_type = Type::Object;
    m_objectValue[key] = SimpleJson(value);
}

void SimpleJson::set(const std::string& key, const SimpleJson& value) {
    m_type = Type::Object;
    m_objectValue[key] = value;
}

void SimpleJson::push_back(const std::string& value) {
    m_type = Type::Array;
    m_arrayValue.push_back(SimpleJson(value));
}

void SimpleJson::push_back(int value) {
    m_type = Type::Array;
    m_arrayValue.push_back(SimpleJson(value));
}

void SimpleJson::push_back(bool value) {
    m_type = Type::Array;
    m_arrayValue.push_back(SimpleJson(value));
}

void SimpleJson::push_back(const SimpleJson& value) {
    m_type = Type::Array;
    m_arrayValue.push_back(value);
}

std::string SimpleJson::toString() const {
    switch (m_type) {
        case Type::Null:
            return "null";
        case Type::String:
            return "\"" + escapeString(m_stringValue) + "\"";
        case Type::Number:
            return std::to_string(m_numberValue);
        case Type::Boolean:
            return m_boolValue ? "true" : "false";
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
        default:
            return "null";
    }
}

std::string SimpleJson::escapeString(const std::string& str) const {
    std::string result;
    for (char c : str) {
        switch (c) {
            case '"': result += "\\\""; break;
            case '\\': result += "\\\\"; break;
            case '\b': result += "\\b"; break;
            case '\f': result += "\\f"; break;
            case '\n': result += "\\n"; break;
            case '\r': result += "\\r"; break;
            case '\t': result += "\\t"; break;
            default: result += c; break;
        }
    }
    return result;
} 