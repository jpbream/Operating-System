#ifndef _TASK_MANAGER_H_
#define _TASK_MANAGER_H_

#include "task.h"
#include "cpu_state.h"

class TaskManager {
private:

    Task* tasks[256];
    int numTasks;
    int currentTask;

public:
    TaskManager();

    void AddTask(Task* task);
    CPUState* Schedule(CPUState* cpuState);
};

#endif