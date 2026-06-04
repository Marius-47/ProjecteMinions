#include <stdio.h>
#include <string.h>
#include "tool.h"
#include "../utils/utils.h"

//Carrega totes les eines del fitxer
int loadTools(Tool tools[], int *total) {
    FILE* f = fopen(TOOLS_FILE, "r");
    if (f == NULL) {
        *total = 0;
        return 0;
    }

    fscanf(f, "%d\n", total);

    for (int i = 0; i < *total; i++) {
        fscanf(f, "ID: %d\n", &tools[i].id);
        fscanf(f, "NAME: %s\n", tools[i].name);
        fscanf(f, "TYPE: %s\n", tools[i].type);
        fscanf(f, "DESC: %s\n", tools[i].description);
        fscanf(f, "QUANTITY: %d\n", &tools[i].currentQuantity);
        fscanf(f, "IN_PRODUCTION: %d\n", &tools[i].inProductionQuantity);
        fscanf(f, "NUM_PIECES: %d\n", &tools[i].numPieces);
        for (int j = 0; j < tools[i].numPieces; j++) {
            fscanf(f, "PIECE: %s\n", tools[i].pieces[j]);
        }
    }

    fclose(f);
    return 1;
}

//Guarda totes les eines de l'array al fitxer
void saveTools(Tool tools[], int total) {
    FILE* f = fopen(TOOLS_FILE, "w");
    if (f == NULL) {
        printf("Error opening tools file.\n");
        return;
    }

    fprintf(f, "%d\n", total);

    for (int i = 0; i < total; i++) {
        fprintf(f, "ID: %d\n", tools[i].id);
        fprintf(f, "NAME: %s\n", tools[i].name);
        fprintf(f, "TYPE: %s\n", tools[i].type);
        fprintf(f, "DESC: %s\n", tools[i].description);
        fprintf(f, "QUANTITY: %d\n", tools[i].currentQuantity);
        fprintf(f, "IN_PRODUCTION: %d\n", tools[i].inProductionQuantity);
        fprintf(f, "NUM_PIECES: %d\n", tools[i].numPieces);
        for (int j = 0; j < tools[i].numPieces; j++) {
            fprintf(f, "PIECE: %s\n", tools[i].pieces[j]);
        }
    }

    fclose(f);
}

//Si no existeixen les hardcoded les carrega
void loadHardcodedTools(void) {
    if (textfileExists(TOOLS_FILE)) return;

    Tool tools[MAX_TOOLS];
    int total = 0;

    tools[0].id = 1;
    strcpy(tools[0].name, "Fart-Gun");
    strcpy(tools[0].type, "Weapon");
    strcpy(tools[0].description, "Shoots-fart-gas-at-enemies");
    tools[0].currentQuantity = 2;
    tools[0].inProductionQuantity = 1;
    tools[0].numPieces = 2;
    strcpy(tools[0].pieces[0], "Gas-Tank");
    strcpy(tools[0].pieces[1], "Nozzle");
    total++;

    tools[1].id = 2;
    strcpy(tools[1].name, "Shrink-Ray");
    strcpy(tools[1].type, "Weapon");
    strcpy(tools[1].description, "Shrinks-any-object-or-person");
    tools[1].currentQuantity = 1;
    tools[1].inProductionQuantity = 0;
    tools[1].numPieces = 2;
    strcpy(tools[1].pieces[0], "Lens");
    strcpy(tools[1].pieces[1], "Power-Cell");
    total++;

    saveTools(tools, total);
}

//Llista totes les eines
void listTools(void) {
    Tool tools[MAX_TOOLS];
    int total = 0;

    loadTools(tools, &total);

    if (total == 0) {
        printf("No tools available.\n");
        return;
    }

    printf("\n--TOOL LIST--\n");
    for (int i = 0; i < total; i++) {
        printf("\nName: %s\n", tools[i].name);
        printf("Type: %s\n", tools[i].type);
        printf("Description: %s\n", tools[i].description);
        printf("Current quantity: %d\n", tools[i].currentQuantity);
        printf("In production: %d\n", tools[i].inProductionQuantity);
        printf("Pieces needed: ");
        for (int j = 0; j < tools[i].numPieces; j++) {
            printf("%s", tools[i].pieces[j]);
            if (j < tools[i].numPieces - 1) printf(", ");
        }
        printf("\n");
    }
    printf("\n");
}