#include <stdio.h>
#include "user/user.h"
#include "utils/utils.h"
#include "tool/tool.h"

void showGruMenu(User u, int *loggedIn) {
    char option;

    printf("\n1. List tools\n");
    printf("2. List tasks\n");
    printf("3. Log out\n");

    printf("Choose an option: ");
    scanf(" %c", &option);
    clearInputBuffer();

    switch (option) {
        case '1':
            listTools();
            break;
        case '2':
            printf("Coming soon!\n");
            break;
        case '3':
            printf("Logging out...\n");
            *loggedIn = 0;
            break;
        default:
            printf("Invalid option.\n");
            break;
    }
}

void showMenu(User u) {
    char option;
    int loggedIn = 1;

    while (loggedIn) {
        printf("Logged in as: %s (", u.name);
        switch (u.role) {
            case GRU: printf("Gru"); break;
            case MINION: printf("Minion"); break;
            case SUPERMINION: printf("Superminion"); break;
            case MINION_ENGINEER: printf("Minion Enginyer"); break;
        }
        printf(")\n");

        switch (u.role) {
            case GRU:
                showGruMenu(u, &loggedIn);
                break;

            default:
                printf("\nFunctions are not implemented yet.\n");
                printf("1. Log out\n");
                
                printf("Choose an option: ");
                scanf(" %c", &option);
                clearInputBuffer();

                if (option == '1') {
                    printf("Logging out...\n");
                    loggedIn = 0;
                }
                break;
        }
    }
}

int main() {
    char option;

    loadHardcodedUsers();
    loadHardcodedTools();

    do {
        printf("\nWelcome to LS Minions Club :) What would you like to do today?\n");
        printf("\t1. Log in\n");
        printf("\t2. Register new Minion\n");
        printf("\t3. Shut down!\n");

        printf("Choose an option: ");
        scanf(" %c", &option);
        clearInputBuffer();

        switch (option) {
            case '1': {
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
            case '2':
                registerMinion();
                break;
            case '3':
                printf("Shutting down. Goodbye!\n");
                break;
            default:
                printf("Invalid option. Try again.\n");
                break;
        }

    } while (option != '3');

    return 0;
}