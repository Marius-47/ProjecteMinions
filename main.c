#include <stdio.h>
#include "user/user.h"
#include "utils/utils.h"
#include "tool/tool.h"
#include "task/task.h"
#include "gru/gru.h"

void showGruMenu(User *u, int *loggedIn) {
    int option;

    printf("\n1. List tools\n");
    printf("2. List tasks\n");
    printf("3. Create part task\n");
    printf("4. Create tool assembly task\n");
    printf("5. Check production status\n");
    printf("6. Reassign pending task\n");
    printf("7. Cancel pending task\n");
    printf("8. Check minion performance\n");
    printf("9. Modify task planning\n");
    printf("10. Generate production report\n");
    printf("11. Evolve minion\n");
    printf("12. Modify user data\n");
    printf("13. Log out\n");

    printf("Choose an option: ");
    scanf("%d", &option);
    clearInputBuffer();

    switch (option) {
        case 1:
            listTools();
            break;
        case 2:
            listTasks();
            break;
        case 3:
            createPartTask();
            break;
        case 4:
            createToolAssemblyTask();
            break;
        case 5:
            showProductionStatus();
            break;
        case 6:
            reassignPendingTask();
            break;
        case 7:
            cancelPendingTask();
            break;
        case 8:
            showMinionPerformance();
            break;
        case 9:
            modifyTaskPlanning();
            break;
        case 10:
            generateProductionReport();
            break;
        case 11:
            evolveMinion();
            break;
        case 12:
            modifyUserData(u);
            break;
        case 13:
            printf("Logging out...\n");
            *loggedIn = 0;
            break;
        default:
            printf("Invalid option.\n");
            break;
    }
}

void showCommonMenu(User *u, int *loggedIn) {
    int option;

    printf("\n1. List tools\n");
    printf("2. List tasks\n");
    printf("3. Modify user data\n");
    printf("4. Log out\n");
    printf("Choose an option: ");
    scanf("%d", &option);
    clearInputBuffer();

    switch (option) {
        case 1:
            listTools();
            break;
        case 2:
            listTasks();
            break;
        case 3:
            modifyUserData(u);
            break;
        case 4:
            printf("Logging out...\n");
            *loggedIn = 0;
            break;
        default:
            printf("Invalid option.\n");
            break;
    }
}

void showMenu(User u) {
    int loggedIn = 1;

    while (loggedIn) {
        printf("\nLogged in as: %s (", u.name);
        switch (u.role) {
            case GRU: printf("Gru"); break;
            case MINION: printf("Minion"); break;
            case SUPERMINION: printf("Superminion"); break;
            case MINION_ENGINEER: printf("Minion Enginyer"); break;     
        }
        printf(")\n");

        if (u.role == GRU) {
            showGruMenu(&u, &loggedIn);
        } else {
            showCommonMenu(&u, &loggedIn);
        }
    }
}

int main() {
    int option;

    loadHardcodedUsers();
    loadHardcodedTools();

    do {
        printf("\nWelcome to LS Minions Club :) What would you like to do today?\n");
        printf("\t1. Log in\n");
        printf("\t2. Register new Minion\n");
        printf("\t3. Shut down!\n");

        printf("Choose an option: ");
        scanf("%d", &option);
        clearInputBuffer();

        switch (option) {
            case 1: {
                User u = loginUser();
                if (u.id == -2) {
                    u = loginWithPin(u.username);
                }
                if (u.id > 0) {
                    showMenu(u);
                } else {
                    printf("Acces denegat. Tornant al menu principal.\n");
                }
                break;
            }
            case 2:
                registerMinion();
                break;
            case 3:
                printf("Shutting down. Goodbye!\n");
                break;
            default:
                printf("Invalid option. Try again.\n");
                break;
        }

    } while (option != 3);

    return 0;
}