#include <stdio.h>
#include <string.h>
#include "user.h"
#include "../utils/utils.h"

// Carrega tots els usuaris del fitxer a un array
int loadUsers(User users[], int *total) {
    FILE* f = fopen(USERS_FILE, "r");
    if (f == NULL) {
        *total = 0;
        return 0;
    }

    fscanf(f, "%d\n", total);

    for (int i = 0; i < *total; i++) {
        fscanf(f, "ID: %d\n", &users[i].id);
        fscanf(f, "NOM: %s\n", users[i].name);
        fscanf(f, "USERNAME: %s\n", users[i].username);
        fscanf(f, "PASSWORD: %s\n", users[i].password);
        fscanf(f, "PIN: %s\n", users[i].pin);
        fscanf(f, "FRUITA: %s\n", users[i].favFruit);
        fscanf(f, "ROL: %d\n", (int*)&users[i].role);
    }

    fclose(f);
    return 1;
}

//Guarda tots els usuaris de l'array al fitxer de text
void saveUsers(User users[], int total) {
    FILE* f = fopen(USERS_FILE, "w");
    if (f == NULL) {
        printf("Error al obrir el fitxer d'usuaris.\n");
        return;
    }

    fprintf(f, "%d\n", total);

    for (int i = 0; i < total; i++) {
        fprintf(f, "ID: %d\n", users[i].id);
        fprintf(f, "NOM: %s\n", users[i].name);
        fprintf(f, "USERNAME: %s\n", users[i].username);
        fprintf(f, "PASSWORD: %s\n", users[i].password);
        fprintf(f, "PIN: %s\n", users[i].pin);
        fprintf(f, "FRUITA: %s\n", users[i].favFruit);
        fprintf(f, "ROL: %d\n", users[i].role);
    }

    fclose(f);
}

//Carrega els usuaris hardcodejats al fitxer si no existeix encara, serien els tres primers ID per quan es crea de nou
void loadHardcodedUsers(void) {
    if (textfileExists(USERS_FILE)) return;

    User users[MAX_USERS];
    int total = 0;

    users[0].id = 1;
    strcpy(users[0].name, "Gru");
    strcpy(users[0].username, "gru");
    strcpy(users[0].password, "gru123");
    strcpy(users[0].pin, "0000");
    strcpy(users[0].favFruit, "cap");
    users[0].role = GRU;
    total++;

    users[1].id = 2;
    strcpy(users[1].name, "Kevin");
    strcpy(users[1].username, "kevin");
    strcpy(users[1].password, "kevin123");
    strcpy(users[1].pin, "1111");
    strcpy(users[1].favFruit, "banana");
    users[1].role = MINION;
    total++;

    users[2].id = 3;
    strcpy(users[2].name, "Stuart");
    strcpy(users[2].username, "stuart");
    strcpy(users[2].password, "stuart123");
    strcpy(users[2].pin, "2222");
    strcpy(users[2].favFruit, "poma");
    users[2].role = SUPERMINION;
    total++;

    saveUsers(users, total);
}

//Comprova si un username ja existeix
int usernameExists(char* username) {
    User users[MAX_USERS];
    int total = 0;

    loadUsers(users, &total);

    for (int i = 0; i < total; i++) {
        if (strcmp(users[i].username, username) == 0) {
            return 1;
        }
    }
    return 0;
}

//El que fa es validar les credencials de l'usuari
User loginUser(void) {
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];
    User empty;
    empty.id = -1;

    User users[MAX_USERS];
    int total = 0;

    loadUsers(users, &total);

    printf("\nIniciar sessio\n");
    printf("Nom d'usuari: ");
    fgets(username, MAX_USERNAME, stdin);
    trimNewline(username);

    printf("Contrasenya: ");
    fgets(password, MAX_PASSWORD, stdin);
    trimNewline(password);

    for (int i = 0; i < total; i++) {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
            printf("\nBenvingut, %s!\n", users[i].name);
            return users[i];
        }
    }

    printf("Contrasenya incorrecta.\n");
    empty.id = -2;
    strcpy(empty.username, username);
    return empty;
}

//Valida el PIN si la contrasenya es incorrecta
User loginWithPin(char* username) {
    char pin[MAX_PIN];
    User empty;
    empty.id = -1;

    User users[MAX_USERS];
    int total = 0;

    loadUsers(users, &total);

    printf("Introdueix el PIN: ");
    fgets(pin, MAX_PIN, stdin);
    trimNewline(pin);

    for (int i = 0; i < total; i++) {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].pin, pin) == 0) {
            printf("\nBenvingut, %s!\n", users[i].name);
            return users[i];
        }
    }

    printf("PIN incorrecte. Acces denegat.\n");
    return empty;
}

//Per registrar un minion nou
int registerMinion(void) {
    User users[MAX_USERS];
    int total = 0;
    User u;

    loadUsers(users, &total);

    if (total >= MAX_USERS) {
        printf("No es poden afegir mes usuaris.\n");
        return 0;
    }

    printf("\nRegistre del nou Minion\n");

    printf("Nom: ");
    fgets(u.name, MAX_NAME, stdin);
    trimNewline(u.name);


    printf("Nom d'usuari: ");
    fgets(u.username, MAX_USERNAME, stdin);
    trimNewline(u.username);

    if (usernameExists(u.username)) {
        printf("Aquest nom d'usuari ja existeix.\n");
        return 0;
    }

    printf("Contrasenya: ");
    fgets(u.password, MAX_PASSWORD, stdin);
    trimNewline(u.password);

    printf("PIN: ");
    fgets(u.pin, MAX_PIN, stdin);
    trimNewline(u.pin);

    printf("Fruita preferida: ");
    fgets(u.favFruit, MAX_FRUIT, stdin);
    trimNewline(u.favFruit);

    u.id = total + 1;
    u.role = MINION;

    users[total] = u;
    total++;

    saveUsers(users, total);
    printf("Minion registrat correctament!\n");
    return 1;
}