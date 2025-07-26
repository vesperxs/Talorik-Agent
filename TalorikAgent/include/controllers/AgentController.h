#pragma once

#include <string>

class AgentController {
public:
    AgentController();
    ~AgentController();

    bool initialize();
    void shutdown();
}; 