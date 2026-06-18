#include <stdio.h>
#include <string.h>
#include "../task/task.h"
#include "../utils/utils.h"

void TEST_hasOverlap() {
    Task tasks[MAX_TASKS];
    int total = 0;
    int i;

    
    strcpy(tasks[0].assignedUsername, "bob");
    tasks[0].startTime.year = 2026;
    tasks[0].startTime.month = 6;
    tasks[0].startTime.day = 15;
    tasks[0].startTime.hour = 10;
    tasks[0].startTime.minute = 0;
    tasks[0].durationMinutes = 60;
    tasks[0].status = PENDING;
    total = 1;

    DateTime overlapping = {30, 10, 15, 6, 2026};   //solapament
    DateTime notOverlapping = {0, 12, 15, 6, 2026}; //no solapament

    int resultOverlap = hasOverlap(tasks, total, "bob", overlapping, 30);
    int resultNoOverlap = hasOverlap(tasks, total, "bob", notOverlapping, 30);

    printf("hasOverlap (cas solapat) = %d, esperat: 1\n", resultOverlap);
    printf("hasOverlap (cas sense solapament) = %d, esperat: 0\n", resultNoOverlap);
}

int main() {
    TEST_hasOverlap();
    return 0;
}