#pragma once

#include <string>

class TaskController {
public:
    TaskController();
    ~TaskController();

    bool initialize();
    void shutdown();
}; 