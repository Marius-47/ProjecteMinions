#ifndef USER_H
#define USER_H

#define MAX_USERNAME 50
#define MAX_PASSWORD 50
#define MAX_NAME 100
#define MAX_FRUIT 50
#define MAX_PIN 10
#define USERS_FILE "data/users.bin"

typedef enum {
    GRU,
    MINION,
    SUPERMINION,
    MINION_ENGINEER
} UserRole;

typedef struct {
    int id;
    char name[MAX_NAME];
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];
    char pin[MAX_PIN];
    char favFruit[MAX_FRUIT];
    UserRole role;
} User;

// Funcions de registre
int registerMinion(void);
int saveUser(User u);
int usernameExists(char* username);
void loadHardcodedUsers(void);

#endif