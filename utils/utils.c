/****************************************************************************
 *
 * @Purpose:    Utility functions for the minions project
 * @Author:     Marina Miranda & Elisa Arasa
 *
 ****************************************************************************/
#include "utils.h"

/********************************* Functions to handle DateTime structs *************************************/
/********************************************************
* Purpose: To get the current date and time
* Parameters: None
* Returns: A DateTime struct with the current date and time
********************************************************/
DateTime getCurrentDateTime() {
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);

    DateTime dt;
    dt.minute = tm_info->tm_min;
    dt.hour = tm_info->tm_hour;
    dt.day = tm_info->tm_mday;
    dt.month = tm_info->tm_mon + 1;
    dt.year = tm_info->tm_year + 1900;

    return dt;
}

/********************************************************
* Purpose: To compare two DateTime structs
* Parameters: dt1 - the first DateTime struct
*             dt2 - the second DateTime struct
* Returns: A negative value if dt1 is earlier than dt2, a positive value if dt1 is later than dt2, and 0 if they are equal
********************************************************/
int compareDateTimes(DateTime dt1, DateTime dt2) {

    if (dt1.year != dt2.year) {
        return dt1.year - dt2.year;
    }
    if (dt1.month != dt2.month) {
        return dt1.month - dt2.month;
    }
    if (dt1.day != dt2.day) {
        return dt1.day - dt2.day;
    }
    if (dt1.hour != dt2.hour) {
        return dt1.hour - dt2.hour; 
    }

    return dt1.minute - dt2.minute;
    
}

/********************************************************
* Purpose: To create a DateTime struct with the given values
* Parameters: minute - the minute value
*             hour - the hour value
*             day - the day value
*             month - the month value
*             year - the year value
* Returns: A pointer to a DateTime struct with the given values or NULL if memory allocation failed
********************************************************/
DateTime* createDateTime(int minute, int hour, int day, int month, int year) {
    DateTime* dt = (DateTime*)malloc(sizeof(int) * 5);

    if (dt == NULL) {
        return NULL;
    }

    dt->minute = minute;
    dt->hour = hour;
    dt->day = day;
    dt->month = month;
    dt->year = year;
    return dt;
}

/************************************************
* Purpose: To convert a DateTime struct to the total number of seconds since the epoch (January 1, 1970)
* Parameters: dt - the DateTime struct to convert
* Returns: The total number of seconds since the epoch represented by the DateTime struct
************************************************/
int DateTimeInSeconds(DateTime dt) {
    return dt.year * 31536000 + dt.month * 2592000 + dt.day * 86400 + dt.hour * 3600 + dt.minute * 60;
}

/********************************************************
* Purpose: To calculate the difference in seconds between two DateTime structs
* Parameters: dt1 - the first DateTime struct
*            dt2 - the second DateTime struct
* Returns: The difference in seconds between dt1 and dt2
********************************************************/
int calculateDifferenceInSeconds(DateTime dt1, DateTime dt2) {
    int totalSeconds1 = DateTimeInSeconds(dt1);
    int totalSeconds2 = DateTimeInSeconds(dt2);

    return totalSeconds2 - totalSeconds1;
}

/********************************************************
* Purpose: To calculate the difference in minutes between two DateTime structs
* Parameters: dt1 - the first DateTime struct
*             dt2 - the second DateTime struct
* Returns: The difference in minutes between dt1 and dt2
********************************************************/
int calculateDifferenceInMinutes(DateTime dt1, DateTime dt2) {
    int secondsDifference = calculateDifferenceInSeconds(dt1, dt2);

    return secondsDifference / 60;
}

/********************************************************
* Purpose: To convert a DateTime struct to a string in the format "YYYY-MM-DD HH:MM"
* Parameters: dt - the DateTime struct to convert
* Returns: A string representation of the DateTime struct or NULL if memory allocation failed
********************************************************/
char* dateTimeToString(DateTime dt) {
    char* buffer = (char*)malloc(20 * sizeof(char));

    if (buffer == NULL) {
        return NULL;
    }

    snprintf(buffer, 20, "%04d-%02d-%02d %02d:%02d", dt.year, dt.month, dt.day, dt.hour, dt.minute);
    return buffer;
}

/********************************************************
* Purpose: To convert a string in the format "YYYY-MM-DD HH:MM" to a DateTime struct
* Parameters: dateTimeStr - the string to convert
* Returns: A pointer to a DateTime struct with the values from the string or NULL if memory allocation failed
********************************************************/
DateTime* stringToDateTime(char* dateTimeStr) {
    DateTime* dt = (DateTime*)malloc(sizeof(int) * 5);

    if (dt == NULL) {
        return NULL;
    }

    sscanf(dateTimeStr, "%d-%d-%d %d:%d", &dt->year, &dt->month, &dt->day, &dt->hour, &dt->minute);
    return dt;
}

/************************************************
* Purpose: To validate a DateTime struct (check if the values are within valid ranges)
* Parameters: dt - the DateTime struct to validate
* Returns: 1 if the DateTime struct is valid, 0 otherwise
************************************************/
int validateDateTime(DateTime dt) {
    if (dt.year < 1970 ||
        dt.month < 1 || dt.month > 12 ||
        dt.day < 1 || dt.day > 31 ||
        dt.hour < 0 || dt.hour > 23 ||
        dt.minute < 0 || dt.minute > 59) {
        return 0;
    }

    return 1;
}


/****************************************** Other utility functions ******************************************/
/************************************************
 * Purpose: To check if an email address is in a valid format (contains an '@' and a '.' after the '@')
 * Parameters: email - the email address to check
 * Returns: 1 if the email address is in a valid format, 0 otherwise
*************************************************/
 int checkEmailFormat(char* email) {
    int len = strlen(email);
    int atIndex = -1;

    for (int i = 0; i < len; i++) {
        if (email[i] == '@') {
            atIndex = i;
            break;
        }
    }
    if (atIndex == -1) {
        return 0;
    }
    else {
        for (int i = atIndex + 1; i < len; i++) {
            if (email[i] == '.') {
                return 1;
            }
        }
        return 0;
    }
}

/************************************************
 * Purpose: To trim the newline character from the end of a string
 * Parameters: str - the string to trim
 * Returns: None (the string is modified in place)
*************************************************/
void trimNewline(char* str) {
    size_t len = strlen(str);

    if (len > 0 && str[len -1] == '\n') {
        str[len -1] = '\0';
    }
}

/************************************************
 * Purpose: To check if a text file exists
 * Parameters: filename - the name of the file to check
 * Returns: 1 if the file exists, 0 otherwise
*************************************************/
int textfileExists(char* filename) {
    FILE* file = fopen(filename, "r");

    if (file) {
        fclose(file);
        return 1;
    }

    return 0;
}

/************************************************
 * Purpose: To check if a binary file exists
 * Parameters: filename - the name of the file to check
 * Returns: 1 if the file exists, 0 otherwise
*************************************************/
int binFileExists(char* filename) {
    FILE* file = fopen(filename, "rb");

    if (file) {
        fclose(file);
        return 1;
    }

    return 0;
}

/************************************************
 * Purpose: To calculate the size of a binary file in bytes
 * Parameters: filename - the name of the file to check
 * Returns: the size of the file, -1 in case of an error
*************************************************/
int getFileSize(char* filename) {
    FILE* file = fopen(filename, "r");
    int size;

    if (file == NULL) {
        return -1;
    }

    fseek(file, 0, SEEK_END);
    size = ftell(file);

    fclose(file);
    
    return size;
}

/************************************************
 * Purpose: To clear the input buffer to prevent unwanted characters from being read in later input readings
 * Parameters: None
 * Returns: None
*************************************************/
void clearInputBuffer() {
    int c;

    while ((c = getchar()) != '\n' && c != EOF) {
    }
}