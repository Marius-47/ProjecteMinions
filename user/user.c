#include <stdio.h>
#include <string.h>
#include "user.h"
#include "../utils/utils.h"


//Guarda un usuari al fitxer
int saveUser(User u) {
    FILE* f = fopen(USERS_FILE, "ab");
    if (f == NULL) return 0;

    fwrite(&u, sizeof(User), 1, f);
    fclose(f);
    return 1;
}

//Carrega els usuaris hardcodejats al fitxer si no existeix encara
void loadHardcodedUsers(void) {
    if (binFileExists(USERS_FILE)) return;

    User gru;
    gru.id = 1;
    strcpy(gru.name, "Gru");
    strcpy(gru.username, "gru");
    strcpy(gru.password, "gru123");
    strcpy(gru.pin, "0000");
    strcpy(gru.favFruit, "cap");
    gru.role = GRU;
    saveUser(gru);

    User minion;
    minion.id = 2;
    strcpy(minion.name, "Kevin");
    strcpy(minion.username, "kevin");
    strcpy(minion.password, "kevin123");
    strcpy(minion.pin, "1111");
    strcpy(minion.favFruit, "banana");
    minion.role = MINION;
    saveUser(minion);

    User superminion;
    superminion.id = 3;
    strcpy(superminion.name, "Stuart");
    strcpy(superminion.username, "stuart");
    strcpy(superminion.password, "stuart123");
    strcpy(superminion.pin, "2222");
    strcpy(superminion.favFruit, "poma");
    superminion.role = SUPERMINION;
    saveUser(superminion);
}

//Comprovem si un username ja existeix al fitxer
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

//Validar les credencials de l'usuari
User loginUser(void) {
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];
    User empty;
    empty.id = -1;

    clearInputBuffer();

    printf("\nIniciar sessió\n");
    printf("Nom d'usuari: ");
    fgets(username, MAX_USERNAME, stdin);
    trimNewline(username);

    printf("Contrasenya: ");
    fgets(password, MAX_PASSWORD, stdin);
    trimNewline(password);

    FILE* f = fopen(USERS_FILE, "rb");
    if (f == NULL) {
        printf("Error al obrir el fitxer d'usuaris.\n");
        return empty;
    }

    User u;
    while (fread(&u, sizeof(User), 1, f)) {
        if (strcmp(u.username, username) == 0 && strcmp(u.password, password) == 0) {
            fclose(f);
            printf("Benvingut, %s!\n", u.name);
            return u;
        }
    }
    fclose(f);

    printf("Contrasenya incorrecta.\n");
    empty.id = -2; // -2 indica contrasenya incorrecta, cal demanar PIN
    strcpy(empty.username, username);
    return empty;
}

//Es Registra un nou minion
int registerMinion(void) {
    User u;
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];
    char pin[MAX_PIN];
    char name[MAX_NAME];
    char fruit[MAX_FRUIT];

    printf("\nRegistre del nou Minion\n");
    printf("Nom: ");
    fgets(name, MAX_NAME, stdin);
    trimNewline(name);
    strcpy(u.name, name);

    clearInputBuffer();

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