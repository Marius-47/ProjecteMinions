#ifndef UTILS_H
#define UTILS_H
#define _GNU_SOURCE

#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdio.h>

typedef struct {
    int minute;
    int hour;
    int day;
    int month;
    int year;
} DateTime;

/********************************* Functions to handle DateTime structs *************************************/
/********************************************************
* Purpose: To get the current date and time
* Parameters: None
* Returns: A DateTime struct with the current date and time
********************************************************/
DateTime getCurrentDateTime();

/********************************************************
* Purpose: To compare two DateTime structs
* Parameters: dt1 - the first DateTime struct
*             dt2 - the second DateTime struct
* Returns: A negative value if dt1 is earlier than dt2, a positive value if dt1 is later than dt2, and 0 if they are equal
********************************************************/
int compareDateTimes(DateTime dt1, DateTime dt2);

/********************************************************
* Purpose: To create a DateTime struct with the given values
* Parameters: minute - the minute value
*             hour - the hour value
*             day - the day value
*             month - the month value
*             year - the year value
* Returns: A pointer to a DateTime struct with the given values or NULL if memory allocation failed
********************************************************/
DateTime* createDateTime(int minute, int hour, int day, int month, int year);

/************************************************
* Purpose: To convert a DateTime struct to the total number of seconds since the epoch (January 1, 1970)
* Parameters: dt - the DateTime struct to convert
* Returns: The total number of seconds since the epoch represented by the DateTime struct
************************************************/
int DateTimeInSeconds(DateTime dt);

/********************************************************
* Purpose: To calculate the difference in seconds between two DateTime structs
* Parameters: dt1 - the first DateTime struct
*            dt2 - the second DateTime struct
* Returns: The difference in seconds between dt1 and dt2
********************************************************/
int calculateDifferenceInSeconds(DateTime dt1, DateTime dt2);

/********************************************************
* Purpose: To calculate the difference in minutes between two DateTime structs
* Parameters: dt1 - the first DateTime struct
*             dt2 - the second DateTime struct
* Returns: The difference in minutes between dt1 and dt2
********************************************************/
int calculateDifferenceInMinutes(DateTime dt1, DateTime dt2);

/********************************************************
* Purpose: To convert a DateTime struct to a string in the format "YYYY-MM-DD HH:MM"
* Parameters: dt - the DateTime struct to convert
* Returns: A string representation of the DateTime struct or NULL if memory allocation failed
********************************************************/
char* dateTimeToString(DateTime dt);

/********************************************************
* Purpose: To convert a string in the format "YYYY-MM-DD HH:MM" to a DateTime struct
* Parameters: dateTimeStr - the string to convert
* Returns: A pointer to a DateTime struct with the values from the string or NULL if memory allocation failed
********************************************************/
DateTime* stringToDateTime(char* dateTimeStr);

/************************************************
* Purpose: To validate a DateTime struct (check if the values are within valid ranges)
* Parameters: dt - the DateTime struct to validate
* Returns: 1 if the DateTime struct is valid, 0 otherwise
************************************************/
int validateDateTime(DateTime dt);


/****************************************** Other utility functions ******************************************/
/************************************************
 * Purpose: To check if an email address is in a valid format (contains an '@' and a '.' after the '@')
 * Parameters: email - the email address to check
 * Returns: 1 if the email address is in a valid format, 0 otherwise
*************************************************/
int checkEmailFormat(char* email);

/************************************************
 * Purpose: To trim the newline character from the end of a string
 * Parameters: str - the string to trim
 * Returns: None (the string is modified in place)
*************************************************/
void trimNewline(char* str);

/************************************************
 * Purpose: To check if a text file exists
 * Parameters: filename - the name of the file to check
 * Returns: 1 if the file exists, 0 otherwise
*************************************************/
int textfileExists(char* filename);

/************************************************
 * Purpose: To check if a binary file exists
 * Parameters: filename - the name of the file to check
 * Returns: 1 if the file exists, 0 otherwise
*************************************************/
int binFileExists(char* filename);

/************************************************
 * Purpose: To calculate the size of a binary file in bytes
 * Parameters: filename - the name of the file to check
 * Returns: the size of the file, -1 in case of an error
*************************************************/
int getFileSize(char* filename);

/************************************************
 * Purpose: To clear the input buffer to prevent unwanted characters from being read in later input readings
 * Parameters: None
 * Returns: None
*************************************************/
void clearInputBuffer();

#endif UTILS_H