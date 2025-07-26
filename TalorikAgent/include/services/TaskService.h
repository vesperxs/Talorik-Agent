#pragma once

#include <string>

class TaskService {
public:
    TaskService();
    ~TaskService();

    bool initialize();
    void shutdown();
}; 