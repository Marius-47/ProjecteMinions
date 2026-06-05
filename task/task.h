#ifndef TASK_H
#define TASK_H

#include "../utils/utils.h"

#define MAX_TASK_DESC 200
#define MAX_TASKS 200
#define TASKS_FILE "data/tasks.txt"

typedef enum {
    PART_CREATION,
    TOOL_ASSEMBLY
} TaskType;

typedef enum {
    PENDING,
    IN_PROGRESS,
    COMPLETED
} TaskStatus;

typedef struct {
    int id;
    char description[MAX_TASK_DESC];
    char assignedUsername[50];
    char assignedName[100];
    DateTime startTime;
    int durationMinutes;
    TaskType type;
    TaskStatus status;
} Task;

int loadTasks(Task tasks[], int *total);
void saveTasks(Task tasks[], int total);
int hasOverlap(Task tasks[], int total, char* username, DateTime start, int duration);
void createPartTask(void);
void updateTaskStatus(Task *t);
void createToolAssemblyTask();
int collectToolAssemblyTaskData(Task *t, Task tasks[], int total);

#endif