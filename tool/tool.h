#ifndef TOOL_H
#define TOOL_H

#define MAX_TOOL_NAME 100
#define MAX_TOOL_TYPE 50
#define MAX_TOOL_DESC 200
#define MAX_TOOLS 50
#define MAX_PIECES_PER_TOOL 10
#define TOOLS_FILE "data/tools.txt"

typedef struct {
    int id;
    char name[MAX_TOOL_NAME];
    char type[MAX_TOOL_TYPE];
    char description[MAX_TOOL_DESC];
    int currentQuantity;
    int inProductionQuantity;
    char pieces[MAX_PIECES_PER_TOOL][MAX_TOOL_NAME];
    int numPieces;
} Tool;

int loadTools(Tool tools[], int *total);
void saveTools(Tool tools[], int total);
void loadHardcodedTools(void);
void listTools(void);

#endif