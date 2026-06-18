#ifndef USER_H
#define USER_H

#define MAX_USERNAME 50
#define MAX_PASSWORD 50
#define MAX_NAME 100
#define MAX_FRUIT 50
#define MAX_PIN 20
#define USERS_FILE "data/users.txt"
#define MAX_USERS 100


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
int loadUsers(User users[], int *total);
void saveUsers(User users[], int total);
void loadHardcodedUsers(void);
int usernameExists(char* username);
User loginUser(void);
User loginWithPin(char* username);
int registerMinion(void);
void modifyUserData(User *loggedUser);
int usernameExistsExcept(char* username, int currentUserId);

#endif