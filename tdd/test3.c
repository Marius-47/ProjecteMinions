#include <stdio.h>
#include "../task/task.h"

void TEST_getNextTaskId() {
    Task tasks[MAX_TASKS];
    int total = 3;

    tasks[0].id = 1;
    tasks[1].id = 3;
    tasks[2].id = 2;

    int resultWithTasks = getNextTaskId(tasks, total);
    int resultEmpty = getNextTaskId(tasks, 0);

    printf("getNextTaskId (amb tasques 1,3,2) = %d, esperat: 4\n", resultWithTasks);
    printf("getNextTaskId (amb un array buit) = %d, esperat: 1\n", resultEmpty);
}

int main() {
    TEST_getNextTaskId();
    return 0;
}