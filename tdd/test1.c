#include <stdio.h>
#include <string.h>
#include "../user/user.h"



void TEST_usernameExists() {
    int resultExisting = usernameExists("bob");
    int resultNotExisting = usernameExists("inexistent");

    printf("usernameExists(\"bob\") = %d, esperat: 1\n", resultExisting);
    printf("usernameExists(\"inexistent\") = %d, esperat: 0\n", resultNotExisting);
}

int main() {
    loadHardcodedUsers();
    TEST_usernameExists();
    return 0;
}