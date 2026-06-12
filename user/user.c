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
        fscanf(f, "NAME: %s\n", users[i].name);
        fscanf(f, "USERNAME: %s\n", users[i].username);
        fscanf(f, "PASSWORD: %s\n", users[i].password);
        fscanf(f, "PIN: %s\n", users[i].pin);
        fscanf(f, "FRUIT: %s\n", users[i].favFruit);
        fscanf(f, "ROLE: %d\n", (int*)&users[i].role);
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
        fprintf(f, "NAME: %s\n", users[i].name);
        fprintf(f, "USERNAME: %s\n", users[i].username);
        fprintf(f, "PASSWORD: %s\n", users[i].password);
        fprintf(f, "PIN: %s\n", users[i].pin);
        fprintf(f, "FRUIT: %s\n", users[i].favFruit);
        fprintf(f, "ROLE: %d\n", users[i].role);
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
    strcpy(users[1].name, "Bob");
    strcpy(users[1].username, "bob");
    strcpy(users[1].password, "bob123");
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


//Utilitzem una altre funcio perque aquesta el que va es ignorar al usuari que esta conectat
int usernameExistsExcept(char* username, int currentUserId) {
    User users[MAX_USERS];
    int total = 0;

    loadUsers(users, &total);

    for (int i = 0; i < total; i++) {
        if (users[i].id != currentUserId &&
            strcmp(users[i].username, username) == 0) {
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

    printf("\nLog in\n");
    printf("Username: ");
    fgets(username, MAX_USERNAME, stdin);
    trimNewline(username);

    printf("Password: ");
    fgets(password, MAX_PASSWORD, stdin);
    trimNewline(password);

    for (int i = 0; i < total; i++) {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
            printf("\nWelcome %s!\n", users[i].name);
            return users[i];
        }
    }

    printf("Incorrect password.\n");
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

    printf("Introduce the PIN: ");
    fgets(pin, MAX_PIN, stdin);
    trimNewline(pin);

    for (int i = 0; i < total; i++) {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].pin, pin) == 0) {
            printf("\nWelcome %s!\n", users[i].name);
            return users[i];
        }
    }

    printf("Incorrect PIN. Acces denied.\n");
    return empty;
}

//Per registrar un minion nou
int registerMinion(void) {
    User users[MAX_USERS];
    int total = 0;
    User u;

    loadUsers(users, &total);

    if (total >= MAX_USERS) {
        printf("No more users can be added.\n");
        return 0;
    }

    printf("\nNew Minion Register\n");

    printf("Name: ");
    fgets(u.name, MAX_NAME, stdin);
    trimNewline(u.name);


    printf("Username: ");
    fgets(u.username, MAX_USERNAME, stdin);
    trimNewline(u.username);

    if (usernameExists(u.username)) {
        printf("This name already exist.\n");
        return 0;
    }

    printf("Password: ");
    fgets(u.password, MAX_PASSWORD, stdin);
    trimNewline(u.password);

    printf("PIN: ");
    fgets(u.pin, MAX_PIN, stdin);
    trimNewline(u.pin);

    printf("Favorite fruit: ");
    fgets(u.favFruit, MAX_FRUIT, stdin);
    trimNewline(u.favFruit);

    u.id = total + 1;
    u.role = MINION;

    users[total] = u;
    total++;

    saveUsers(users, total);
    printf("Minion added successfully!\n");
    return 1;
}


//Les opcions que es poden modificar segons el rol
void modifyUserData(User *loggedUser) {
    int option;
    char newUsername[MAX_USERNAME];
    
    printf("\n--Modify User Data--\n");

    if (loggedUser->role == GRU) {
        printf("1. Change username\n");
        printf("2. Change password\n");
        printf("3. Change PIN\n");
        printf("4. Change favorite fruit\n");
        printf("5. Go back\n");

        printf("Choose an option: ");
        scanf("%d", &option);
        clearInputBuffer();

        switch (option) {
            case 1:
                printf("New username: ");
                fgets(newUsername, MAX_USERNAME, stdin);
                trimNewline(newUsername);

                if (usernameExistsExcept(newUsername, loggedUser->id)) {
                    printf("Error: This username already exists.\n");
                    return;
                }

                strcpy(loggedUser->username, newUsername);
                break;

            case 2:
                printf("New password: ");
                fgets(loggedUser->password, MAX_PASSWORD, stdin);
                trimNewline(loggedUser->password);
                break;

            case 3:
                printf("New PIN: ");
                fgets(loggedUser->pin, MAX_PIN, stdin);
                trimNewline(loggedUser->pin);
                break;

            case 4:
                printf("New favorite fruit: ");
                fgets(loggedUser->favFruit, MAX_FRUIT, stdin);
                trimNewline(loggedUser->favFruit);
                break;

            case 5:
                return;

            default:
                printf("Invalid option.\n");
                return;
        }

    } else {
        printf("1. Change name\n");
        printf("2. Change username\n");
        printf("3. Change password\n");
        printf("4. Change PIN\n");
        printf("5. Change favorite fruit\n");
        printf("6. Go back\n");

        printf("Choose an option: ");
        scanf("%d", &option);
        clearInputBuffer();

        switch (option) {
            case 1:
                printf("New name: ");
                fgets(loggedUser->name, MAX_NAME, stdin);
                trimNewline(loggedUser->name);
                break;

            case 2:
                printf("New username: ");
                fgets(newUsername, MAX_USERNAME, stdin);
                trimNewline(newUsername);

                if (usernameExistsExcept(newUsername, loggedUser->id)) {
                    printf("Error: This username already exists.\n");
                    return;
                }

                strcpy(loggedUser->username, newUsername);
                break;

            case 3:
                printf("New password: ");
                fgets(loggedUser->password, MAX_PASSWORD, stdin);
                trimNewline(loggedUser->password);
                break;

            case 4:
                printf("New PIN: ");
                fgets(loggedUser->pin, MAX_PIN, stdin);
                trimNewline(loggedUser->pin);
                break;

            case 5:
                printf("New favorite fruit: ");
                fgets(loggedUser->favFruit, MAX_FRUIT, stdin);
                trimNewline(loggedUser->favFruit);
                break;

            case 6:
                return;

            default:
                printf("Invalid option.\n");
                return;
        }
    }
    printf("User data modified during the current session.\n");
}