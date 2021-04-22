#include "task_manager.h"

TaskManager::TaskManager()
{
    numTasks = 0;
    currentTask = -1;
}

void TaskManager::AddTask(Task* task)
{
    tasks[numTasks++] = task;
}

CPUState* TaskManager::Schedule(CPUState* state)
{
    // the first time this is called, currentTask is -1,
    // so we essentially discard the state of the kernel environment
    // killing any chance of ever going back to it

    if (numTasks == 0) {
        return state;
    }

    if (currentTask >= 0) {
        tasks[currentTask]->cpuState = state;
    }

    if (++currentTask >= numTasks) {
        currentTask %= numTasks;
    }

    return tasks[currentTask]->cpuState;
}