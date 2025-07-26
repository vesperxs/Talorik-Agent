#pragma once

#include <string>
#include <map>
#include <vector>

// Simple JSON implementation as fallback
class SimpleJson {
public:
    enum class Type {
        Null,
        String,
        Number,
        Boolean,
        Object,
        Array
    };

    SimpleJson() : m_type(Type::Null) {}
    SimpleJson(const std::string& value) : m_type(Type::String), m_stringValue(value) {}
    SimpleJson(int value) : m_type(Type::Number), m_numberValue(value) {}
    SimpleJson(bool value) : m_type(Type::Boolean), m_boolValue(value) {}

    // Object methods
    void set(const std::string& key, const std::string& value);
    void set(const std::string& key, int value);
    void set(const std::string& key, bool value);
    void set(const std::string& key, const SimpleJson& value);

    // Array methods
    void push_back(const std::string& value);
    void push_back(int value);
    void push_back(bool value);
    void push_back(const SimpleJson& value);

    // Conversion to string
    std::string toString() const;

    // Type checking
    Type getType() const { return m_type; }
    bool isString() const { return m_type == Type::String; }
    bool isNumber() const { return m_type == Type::Number; }
    bool isBoolean() const { return m_type == Type::Boolean; }
    bool isObject() const { return m_type == Type::Object; }
    bool isArray() const { return m_type == Type::Array; }

    // Value access
    std::string getString() const { return m_stringValue; }
    int getNumber() const { return m_numberValue; }
    bool getBoolean() const { return m_boolValue; }

private:
    Type m_type;
    std::string m_stringValue;
    int m_numberValue;
    bool m_boolValue;
    std::map<std::string, SimpleJson> m_objectValue;
    std::vector<SimpleJson> m_arrayValue;

    std::string escapeString(const std::string& str) const;
}; 