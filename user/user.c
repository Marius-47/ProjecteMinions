#include <stdio.h>
#include <string.h>
#include "user.h"
#include "../utils/utils.h"

// Comprova si un username ja existeix al fitxer
int usernameExists(char* username) {
    FILE* f = fopen(USERS_FILE, "rb");
    if (f == NULL) return 0;

    User u;
    while (fread(&u, sizeof(User), 1, f)) {
        if (strcmp(u.username, username) == 0) {
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

// Registra un nou minion (llegir i validar dades)
int registerMinion(void) {
    User u;
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];
    char pin[MAX_PIN];
    char name[MAX_NAME];
    char fruit[MAX_FRUIT];

    printf("\nRegistre de nou Minion\n");
    printf("Nom: ");
    fgets(name, MAX_NAME, stdin);
    trimNewline(name);
    strcpy(u.name, name);

    printf("Nom d'usuari: ");
    fgets(username, MAX_USERNAME, stdin);
    trimNewline(username);

    if (usernameExists(username)) {
        printf("Aquest nom d'usuari ja existeix.\n");
        return 0;
    }
    strcpy(u.username, username);

    printf("Contrasenya: ");
    fgets(password, MAX_PASSWORD, stdin);
    trimNewline(password);
    strcpy(u.password, password);

    printf("PIN: ");
    fgets(pin, MAX_PIN, stdin);
    trimNewline(pin);
    strcpy(u.pin, pin);

    printf("Fruita preferida: ");
    fgets(fruit, MAX_FRUIT, stdin);
    trimNewline(fruit);
    strcpy(u.favFruit, fruit);

    u.role = MINION;

    FILE* f = fopen(USERS_FILE, "rb");
    int count = 0;
    if (f != NULL) {
        User tmp;
        while (fread(&tmp, sizeof(User), 1, f)) count++;
        fclose(f);
    }
    u.id = count + 1;

    return saveUser(u);
}