#include <stdio.h>
#include <string.h>
#include "../user/user.h"
#include "../task/task.h"
#include "../utils/utils.h"

void TEST_usernameExists() {
    int resultExisting = usernameExists("bob");
    int resultNotExisting = usernameExists("inexistent");

    printf("usernameExists(\"bob\") = %d, esperat: 1\n", resultExisting);
    printf("usernameExists(\"inexistent\") = %d, esperat: 0\n", resultNotExisting);
}

void TEST_hasOverlap() {
    Task tasks[MAX_TASKS];
    int total;

    strcpy(tasks[0].assignedUsername, "bob");
    tasks[0].startTime.year = 2026;
    tasks[0].startTime.month = 6;
    tasks[0].startTime.day = 15;
    tasks[0].startTime.hour = 10;
    tasks[0].startTime.minute = 0;
    tasks[0].durationMinutes = 60;
    tasks[0].status = PENDING;
    total = 1;

    DateTime overlapping = {30, 10, 15, 6, 2026};
    DateTime notOverlapping = {0, 12, 15, 6, 2026};

    int resultOverlap = hasOverlap(tasks, total, "bob", overlapping, 30);
    int resultNoOverlap = hasOverlap(tasks, total, "bob", notOverlapping, 30);

    printf("hasOverlap (cas amb solapament) = %d, esperat: 1\n", resultOverlap);
    printf("hasOverlap (cas sense solapament) = %d, esperat: 0\n", resultNoOverlap);
}

void TEST_getNextTaskId() {
    Task tasks[MAX_TASKS];

    tasks[0].id = 1;
    tasks[1].id = 3;
    tasks[2].id = 2;

    int resultWithTasks = getNextTaskId(tasks, 3);
    int resultEmpty = getNextTaskId(tasks, 0);

    printf("getNextTaskId (amb tasques 1,3,2) = %d, esperat: 4\n", resultWithTasks);
    printf("getNextTaskId (array que esta buit) = %d, esperat: 1\n", resultEmpty);
}

int main() {
    loadHardcodedUsers();

    printf("\n--Test 1--\n");
    TEST_usernameExists();

    printf("\n--Test 2--\n");
    TEST_hasOverlap();

    printf("\n--Test 3--\n");
    TEST_getNextTaskId();

    return 0;
}