#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gru.h"
#include "../task/task.h"
#include "../user/user.h"
#include "../utils/utils.h"


// Mostra les dades d'una tasca dins de l'estat de produccio
void printProductionTask(Task task) {
    char *startTimeStr = dateTimeToString(task.startTime);

    printf("\nAssigned to: %s (%s)\n",
        task.assignedName,
        task.assignedUsername);

    printf("Description: %s\n", task.description);

    if (startTimeStr == NULL) {
        printf("Error converting start date.\n");
    } else {
        printf("Start date: %s\n", startTimeStr);
        free(startTimeStr);
    }

    printf("Duration: %d minutes\n", task.durationMinutes);

    if (task.type == PART_CREATION) {
        printf("Type: Part creation\n");
    } else if (task.type == TOOL_ASSEMBLY) {
        printf("Type: Tool assembly\n");
    }
}

//Funcio per tasques pendents
void showPendingTasks(Task tasks[], int total) {
    int pendingTasksShown = 0;

    printf("\n--Pending Tasks--\n");
    for (int i = 0; i < total; i++) {
        if (tasks[i].status == PENDING) {
            printProductionTask(tasks[i]);
            pendingTasksShown++;
        }
    }

    if (pendingTasksShown == 0) {
        printf("There are no pending task.\n");
    }
}

//Aquesta per les que estan en curs in progress
void showInProgressTasks(Task tasks[], int total) {
    int inProgressTasksShown = 0;

    printf("\n--In Progress Tasks--\n");

    for (int i = 0; i < total; i++) {
        if (tasks[i].status == IN_PROGRESS) {
            printProductionTask(tasks[i]);
            inProgressTasksShown++;
        }
    }

    if (inProgressTasksShown == 0) {
        printf("There is no task in progress.\n");
    }
}

//Mostrem les tasques que ja estan completades, que es en aquest cas es mostra el contador de quantes ja s'han completat
void showCompletedTasksTotal(Task tasks[], int total) {
    int completedTasks = 0;

    for (int i = 0; i < total; i++) {
        if (tasks[i].status == COMPLETED) {
            completedTasks++;
        }
    }

    if (completedTasks == 0) {
        printf("\nNo completed tasks found.\n");
    } else {
        printf("\nCompleted tasks: %d\n", completedTasks);
    }
}

//Mostar la funcionalitat en general de les tasques
void showProductionStatus() {
    Task tasks[MAX_TASKS];
    int total = 0;

    loadTasks(tasks, &total);

    printf("\n--Production Status--\n");

    showPendingTasks(tasks, total);
    showInProgressTasks(tasks, total);
    showCompletedTasksTotal(tasks, total);
}



//Primer tenim la funció que mostra les tasques i les seves caracterisitques, i dsp que et deixa demanar quina tasca seleccionar
int selectPendingTask(Task tasks[], int total) {
    int pendingIndexes[MAX_TASKS];
    int pendingCount = 0;
    int choice;

    printf("\n--Pending Tasks--\n");

    for (int i = 0; i < total; i++) {
        if (tasks[i].status == PENDING) {
            pendingIndexes[pendingCount] = i;
            pendingCount++;

            printf("\n%d. Task ID: %d", pendingCount, tasks[i].id);
            printProductionTask(tasks[i]);
        }
    }

    if (pendingCount == 0) {
        printf("No pending tasks found.\n");
        return -1;
    }

    printf("\nSelect task number: ");
    scanf("%d", &choice);
    clearInputBuffer();

    if (choice < 1 || choice > pendingCount) {
        printf("Invalid selection.\n");
        return -1;
    }

    return pendingIndexes[choice - 1];
}

//Tenim els usuaris del rol que s'ha seleccionat, pot ser o minion o superminion
int selectUserByRole(User users[], int totalUsers, UserRole requiredRole) {
    int userIndexes[MAX_USERS];
    int userCount = 0;
    int choice;

    if (requiredRole == MINION) {
        printf("\n--Available Minions--\n");
    } else {
        printf("\n--Available Superminions--\n");
    }

    for (int i = 0; i < totalUsers; i++) {
        if (users[i].role == requiredRole) {
            userIndexes[userCount] = i;
            userCount++;

            printf("%d. %s (%s)\n",
                userCount,
                users[i].name,
                users[i].username);
        }
    }

    if (userCount == 0) {
        printf("No available users found.\n");
        return -1;
    }

    printf("Select user number: ");
    scanf("%d", &choice);
    clearInputBuffer();

    if (choice < 1 || choice > userCount) {
        printf("Invalid selection.\n");
        return -1;
    }

    return userIndexes[choice - 1];
}


void reassignPendingTask() {
    Task tasks[MAX_TASKS];
    User users[MAX_USERS];
    UserRole requiredRole;
    int total = 0;
    int selectedTaskIndex;
    int totalUsers = 0;
    int selectedUserIndex;
    

    loadTasks(tasks, &total);
    selectedTaskIndex = selectPendingTask(tasks, total);

    if (selectedTaskIndex == -1) {
        return;
    }

    loadUsers(users, &totalUsers);
    if (tasks[selectedTaskIndex].type == PART_CREATION) {
        requiredRole = MINION;
    } else {
        requiredRole = SUPERMINION;
    }

    selectedUserIndex = selectUserByRole(users, totalUsers, requiredRole);

    if (selectedUserIndex == -1) {
        return;
    }


    if (strcmp(tasks[selectedTaskIndex].assignedUsername,users[selectedUserIndex].username) == 0) {

        printf("ERROR: This user is already assigned to the task.\n");
        return;
    }

    if (hasOverlapExcept(tasks,total,users[selectedUserIndex].username,tasks[selectedTaskIndex].startTime,tasks[selectedTaskIndex].durationMinutes,selectedTaskIndex)) {
        printf("ERROR: This user already has a task at that time.\n");
        return;
    }

    strcpy(tasks[selectedTaskIndex].assignedUsername,users[selectedUserIndex].username);

    strcpy(tasks[selectedTaskIndex].assignedName,users[selectedUserIndex].name);

    saveTasks(tasks, total);

    printf("Task reassigned successfully!\n");
}