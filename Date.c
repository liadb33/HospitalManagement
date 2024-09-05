#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "General.h"
#include "Date.h"

const int DAY_MONTHS[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
#define SPECIAL_TAV '/'

void getCorrectDate(Date* pDate,char* msg)
{
	char date[MAX_STR_LEN];
	int ok = 1;

	do {
		printf("%sminimum year %d\t",msg, MIN_YEAR);
        fGets(date, MAX_STR_LEN, stdin);
		ok = checkDate(date, pDate);
		if (!ok)
			printf("Error try again\n");
	} while (!ok);
}


int	 checkDate(char* date, Date* pDate)
{
	int day, month, year;
	if (strlen(date) != 10)
		return 0;
	if ( (date[2] != SPECIAL_TAV) || (date[5] != SPECIAL_TAV))
		return 0;
	sscanf(date, "%d%*c%d%*c%d", &day, &month, &year);
	if (day < 1 || month < 1 || month > 12 || year < MIN_YEAR)
		return 0;

	if (day > DAY_MONTHS[month - 1])
		return 0;

	pDate->day = day;
	pDate->month = month;
	pDate->year = year;

	return 1;
}

//used for init health policy
int checkIfDatePolicyValid(Date* startValidDate, Date* endValidDate) {
    // Get today's date
    Date today;
    time_t
    now = time (NULL);
    struct tm *currentTime = localtime(&now);
    today.day = currentTime->tm_mday;
    today.month = currentTime->tm_mon + 1; // Months are 0-based, so add 1
    today.year = currentTime->tm_year + 1900; // Years since 1900, so add 1900

    //compare start date with today's date
    if (startValidDate->year > today.year ||
        (startValidDate->year == today.year && startValidDate->month > today.month) ||
        (startValidDate->year == today.year && startValidDate->month == today.month &&
         startValidDate->day > today.day)) {
        ERROR_MSG_AND_RETURN("Invalid start date. Start date cannot be in the future.\n",0);
         //exit the function if start date is invalid
    }

    // Compare end date with today's date
    if (endValidDate->year < today.year ||
        (endValidDate->year == today.year && endValidDate->month < today.month) ||
        (endValidDate->year == today.year && endValidDate->month == today.month && endValidDate->day < today.day)) {
        ERROR_MSG_AND_RETURN("Your health policy is invalid.\n",0);
    }
    return 1;
}

//used for find patient
void getEndDate(Date* pDate)
{
    char date[MAX_STR_LEN];
    int ok = 1;

    do {
        printf("Enter end date of health policy dd%cmm%cyyyy: ",
               SPECIAL_TAV, SPECIAL_TAV);
        fGets(date, MAX_STR_LEN, stdin);
        ok = checkDate(date, pDate);
        if (!ok)
            printf("Error try again\n");
    } while (!ok);
}

void printDate(const Date* pDate)
{
	printf("%d/%d/%d", pDate->day, pDate->month, pDate->year);
}

int readDateFromTextFile(FILE* fp, Date* pDate) {
    if (fscanf(fp, "%d/%d/%d\n", &pDate->day, &pDate->month, &pDate->year) != 3) {
        printf("Error reading date\n");
        return 0;
    }
    return 1;
}

int		saveDateToBFile( FILE* fp, const Date* pDate){
    if (fwrite(pDate, sizeof(Date), 1, fp) != 1)
    {
        printf("Error write date\n");
        return 0;
    }
    return 1;
}

int		readDateFromBFile(FILE* fp, Date* pDate){
    if (fread(pDate, sizeof(Date), 1, fp) != 1)
    {
        printf("Error write date\n");
        return 0;
    }
    return 1;
}

int countBetweenDays(Date* pStartDate, Date* pEndDate) {
    if (pStartDate == NULL || pEndDate == NULL)
        return -1;

    // Calculate the number of days from the reference date (01/01/0001) to each date
    int startDays = dateToDays(pStartDate);
    int endDays = dateToDays(pEndDate);

    // Calculate the difference in days
    int daysBetween = endDays - startDays;

    return daysBetween;
}

int dateToDays(Date* pDate) {
    int days = 0;
    int year = pDate->year;
    int month = pDate->month;
    int day = pDate->day;

    // Count the number of days from the reference date to the given date
    for (int y = 1; y < year; y++) {
        if (isLeapYear(y))
            days += 366;
        else
            days += 365;
    }

    for (int m = 1; m < month; m++) {
        days += DAY_MONTHS[m - 1];
        if (m == 2 && isLeapYear(year))
            days++;
    }

    days += day - 1;

    return days;
}

int isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}
