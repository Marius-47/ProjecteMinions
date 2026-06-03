#include <stdio.h>
#include "user/user.h"

int main() {
    char option;

    loadHardcodedUsers();

    do {
        printf("\nWelcome to LS Minions Club :) What would you like to do today?\n");
        printf("\t1. Log in\n");
        printf("\t2. Register new Minion\n");
        printf("\t3. Shut down!\n");
        printf("Choose an option: ");

        scanf(" %c", &option);

        switch (option) {
            case '1':
                //ST-01 login s'ha d'afegir encara
                break;
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