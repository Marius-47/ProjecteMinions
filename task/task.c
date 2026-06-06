#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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
        fscanf(f, "DESC: ");
        fgets(tasks[i].description, MAX_TASK_DESC, f);
        trimNewline(tasks[i].description);
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

    if (total >= MAX_TASKS) {
    printf("Error: Maximum number of tasks reached.\n");
    return;
}

    //Aqui posem els usuaris per aixi poder fer selecció del minion en concret
    User users[MAX_USERS];
    int totalUsers = 0;
    loadUsers(users, &totalUsers);

    int minionCount = 0;
    int minionIndexes[MAX_USERS];

    // Mostrem llista de minions disponibles
    printf("\n--Available Minions--\n");
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
    printf("\nDescription: ");
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

// Recull les dades d'una tasca d'ensellament i permet seleccionar un superminion
int collectToolAssemblyTaskData(Task *t, Task tasks[], int total) {
    User users[MAX_USERS];
    int totalUsers = 0;
    int superminionCount = 0;
    int superminionIndexes[MAX_USERS];
    int choice;
    int superminionIndex;
    int parsedValues;
    int i;
    char dateStr[20];

    loadUsers(users, &totalUsers);

    // Mostrem només els superminions disponibles
    printf("\n--Available Superminions--\n");

    for (i = 0; i < totalUsers; i++) {
        if (users[i].role == SUPERMINION) {
            superminionIndexes[superminionCount] = i;
            superminionCount++;

            printf("%d. %s (%s)\n",
                superminionCount,
                users[i].name,
                users[i].username);
        }
    }

    if (superminionCount == 0) {
        printf("No superminions available.\n");
        return 0;
    }

    printf("Select superminion number: ");
    scanf("%d", &choice);
    clearInputBuffer();

    if (choice < 1 || choice > superminionCount) {
        printf("Invalid selection.\n");
        return 0;
    }

    superminionIndex = superminionIndexes[choice - 1];

    strcpy(t->assignedUsername, users[superminionIndex].username);
    strcpy(t->assignedName, users[superminionIndex].name);

    // Demanem les dades de la tasca
    printf("\nDescription: ");
    fgets(t->description, MAX_TASK_DESC, stdin);
    trimNewline(t->description);

    printf("Start date (YYYY-MM-DD HH:MM): ");
    fgets(dateStr, 20, stdin);
    trimNewline(dateStr);

    parsedValues = sscanf(dateStr, "%d-%d-%d %d:%d",
        &t->startTime.year,
        &t->startTime.month,
        &t->startTime.day,
        &t->startTime.hour,
        &t->startTime.minute);

    if (parsedValues != 5 || !validateDateTime(t->startTime)) {
        printf("Error: Invalid date.\n");
        return 0;
    }

    printf("Duration (minutes): ");
    scanf("%d", &t->durationMinutes);
    clearInputBuffer();

    if (t->durationMinutes <= 0) {
        printf("Error: Duration must be greater than zero.\n");
        return 0;
    }

    if (hasOverlap(tasks, total, t->assignedUsername,
            t->startTime, t->durationMinutes)) {
        printf("Error: This superminion already has a task at that time.\n");
        return 0;
    }

    return 1;
}

//Com que per guardar a fitxer tenim una TT, ara utilitzem una funcio auxiliar molt facil nomes per printar per pantalla el que s'introdueix, per comprovar q es guarden be els valors
void createToolAssemblyTask(void) {
    Task tasks[MAX_TASKS];
    Task t;
    int total = 0;

    loadTasks(tasks, &total);

    if (total >= MAX_TASKS) {
        printf("Error: Maximum number of tasks reached.\n");
        return;
    }

    if (collectToolAssemblyTaskData(&t, tasks, total)) {
        t.id = total + 1;
        t.type = TOOL_ASSEMBLY;

        updateTaskStatus(&t);

        tasks[total] = t;
        total++;

        saveTasks(tasks, total);

        printf("Tool assembly task created successfully!\n");
    }
}

//Funcio que mostra les tasques que estan pendent i en curs
void listTasks(void) {
    Task tasks[MAX_TASKS];
    int total = 0;
    int tasksShown = 0;

    loadTasks(tasks, &total);

    printf("\n--Pending and In Progress Tasks--\n");
    for (int i = 0; i < total; i++) {
        if (tasks[i].status == PENDING || tasks[i].status == IN_PROGRESS) {
            printf("\nAssigned to: %s (%s)\n",
                tasks[i].assignedName,
                tasks[i].assignedUsername);
            printf("Description: %s\n", tasks[i].description);

            char *startTimeStr = dateTimeToString(tasks[i].startTime);

            if (startTimeStr == NULL) {
                printf("Error converting start date.\n");
            } else {
                printf("Start date: %s\n", startTimeStr);
                free(startTimeStr);
            }

            printf("Duration: %d minutes\n", tasks[i].durationMinutes);

            tasksShown++;
        }
    }

    if (tasksShown == 0) {
        printf("No pending or in progress tasks found.\n");
    }
}

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
static void showPendingTasks(Task tasks[], int total) {
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



//Mostar la funcionalitat en general de les tasques
void showProductionStatus(void) {
    Task tasks[MAX_TASKS];
    int total = 0;

    loadTasks(tasks, &total);

    printf("\n--Production Status--\n");

    showPendingTasks(tasks, total);
    showInProgressTasks(tasks, total);
}