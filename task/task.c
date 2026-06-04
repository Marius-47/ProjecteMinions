#include <stdio.h>
#include <string.h>
#include "task.h"
#include "../user/user.h"
#include "../utils/utils.h"

//Per saber l'estat de la tasca tenim aquesta funcio auxiliar que anirem cridant a loadTasks
// Per saber l'estat de la tasca tenim aquesta funcio auxiliar
// que anirem cridant a loadTasks
void updateTaskStatus(Task *t) {
    DateTime now = getCurrentDateTime();

    int diffToStart = calculateDifferenceInSeconds(now, t->startTime);
    int diffToEnd = diffToStart + t->durationMinutes * 60;

    if (diffToStart > 0) {
        t->status = PENDING;
    } else if (diffToEnd > 0) {
        t->status = IN_PROGRESS;
    } else {
        t->status = COMPLETED;
    }
}

// Carrega totes les tasques del fitxer
int loadTasks(Task tasks[], int *total) {
    FILE* f = fopen(TASKS_FILE, "r");
    int i;
    
    if (f == NULL) {
        *total = 0;
        return 0;
    }

    fscanf(f, "%d\n", total);

    for (i = 0; i < *total; i++) {
        fscanf(f, "ID: %d\n", &tasks[i].id);
        fscanf(f, "DESC: %s\n", tasks[i].description);
        fscanf(f, "USERNAME: %s\n", tasks[i].assignedUsername);
        fscanf(f, "NAME: %s\n", tasks[i].assignedName);
        fscanf(f, "START: %d-%d-%d %d:%d\n",
            &tasks[i].startTime.year,
            &tasks[i].startTime.month,
            &tasks[i].startTime.day,
            &tasks[i].startTime.hour,
            &tasks[i].startTime.minute);
        fscanf(f, "DURATION: %d\n", &tasks[i].durationMinutes);
        fscanf(f, "TYPE: %d\n", (int*)&tasks[i].type);
        fscanf(f, "STATUS: %d\n", (int*)&tasks[i].status);
        updateTaskStatus(&tasks[i]);
    }

    fclose(f);
    return 1;
}

//Guarda totes les tasques al fitxer
void saveTasks(Task tasks[], int total) {
    FILE* f = fopen(TASKS_FILE, "w");
    int i;
    
    if (f == NULL) {
        printf("Error opening tasks file.\n");
        return;
    }

    fprintf(f, "%d\n", total);

    for (i = 0; i < total; i++) {
        fprintf(f, "ID: %d\n", tasks[i].id);
        fprintf(f, "DESC: %s\n", tasks[i].description);
        fprintf(f, "USERNAME: %s\n", tasks[i].assignedUsername);
        fprintf(f, "NAME: %s\n", tasks[i].assignedName);
        fprintf(f, "START: %d-%d-%d %d:%d\n",
            tasks[i].startTime.year,
            tasks[i].startTime.month,
            tasks[i].startTime.day,
            tasks[i].startTime.hour,
            tasks[i].startTime.minute);
        fprintf(f, "DURATION: %d\n", tasks[i].durationMinutes);
        fprintf(f, "TYPE: %d\n", tasks[i].type);
        fprintf(f, "STATUS: %d\n", tasks[i].status);
    }

    fclose(f);
}

//Comprovo que per un usuari no hi hagi solapaments
int hasOverlap(Task tasks[], int total, char* username, DateTime start, int duration) {
    int newStart = DateTimeInSeconds(start);
    int newEnd = newStart + duration * 60;

    for (int i = 0; i < total; i++) {
        if (strcmp(tasks[i].assignedUsername, username) == 0 &&
            tasks[i].status != COMPLETED) {

            int existingStart = DateTimeInSeconds(tasks[i].startTime);
            int existingEnd = existingStart + tasks[i].durationMinutes * 60;

            if (newStart < existingEnd && newEnd > existingStart) {
                return 1;
            }
        }
    }
    return 0;
}

//Creacio de tasca per parts per un minion
void createPartTask(void) {
    Task tasks[MAX_TASKS];
    int total = 0;
    int i;
    Task t;

    loadTasks(tasks, &total);

    //Aqui posem els usuaris per aixi poder fer selecció del minion en concret
    User users[MAX_USERS];
    int totalUsers = 0;
    loadUsers(users, &totalUsers);

    int minionCount = 0;
    int minionIndexes[MAX_USERS];

    // Mostrem llista de minions disponibles
    printf("\n--- Available Minions ---\n");
    for (i = 0; i < totalUsers; i++) {
        if (users[i].role == MINION) {
            minionCount++;
            minionIndexes[minionCount - 1] = i;
            printf("%d. %s (%s)\n", minionCount, users[i].name, users[i].username);
        }
    }

    if (minionCount == 0) {
        printf("No minions available.\n");
        return;
    }

    int choice;
    printf("Select minion number: ");
    scanf("%d", &choice);
    clearInputBuffer();

    if (choice < 1 || choice > minionCount) {
        printf("Invalid selection.\n");
        return;
    }

    int minionIndex = minionIndexes[choice - 1];
    strcpy(t.assignedUsername, users[minionIndex].username);
    strcpy(t.assignedName, users[minionIndex].name);

    // Demanem dades de la tasca
    printf("Description: ");
    fgets(t.description, MAX_TASK_DESC, stdin);
    trimNewline(t.description);

    char dateStr[20];
    int parsedValues;

    printf("Start date (YYYY-MM-DD HH:MM): ");
    fgets(dateStr, 20, stdin);
    trimNewline(dateStr);

    parsedValues = sscanf(dateStr, "%d-%d-%d %d:%d",
        &t.startTime.year,
        &t.startTime.month,
        &t.startTime.day,
        &t.startTime.hour,
        &t.startTime.minute);

    if (parsedValues != 5 || !validateDateTime(t.startTime)) {
        printf("Error: Invalid date.\n");
        return;
    }

    printf("Duration (minutes): ");
    scanf("%d", &t.durationMinutes);
    clearInputBuffer();

    //Aqui cridem la funcio per validar els solapaments
    if (hasOverlap(tasks, total, t.assignedUsername, t.startTime, t.durationMinutes)) {
        printf("Error: This minion already has a task at that time.\n");
        return;
    }

    t.id = total + 1;
    t.type = PART_CREATION;

    updateTaskStatus(&t);

    tasks[total] = t;
    total++;

    saveTasks(tasks, total);
    printf("Task created successfully!\n");
}