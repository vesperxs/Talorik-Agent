#pragma once

#include <string>

class AgentService {
public:
    AgentService();
    ~AgentService();

    bool initialize();
    void shutdown();
}; 