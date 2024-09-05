#ifndef __MYDATE__
#define __MYDATE__

#include <time.h>
#define MIN_YEAR 2023

typedef struct
{
    int			day;
    int			month;
    int			year;
}Date;

void	getCorrectDate(Date* pDate,char* msg);
int		checkDate(char* date, Date* pDate);
int checkIfDatePolicyValid(Date* startValidDate, Date* endValidDate);

//used for find patient
void getEndDate(Date* pDate);
void	printDate(const Date* pDate);
int readDateFromTextFile(FILE* fp, Date* pDate);
int		saveDateToBFile( FILE* fp, const Date* pDate);
int		readDateFromBFile(FILE* fp, Date* pDate);

int countBetweenDays(Date* pStartDate,Date* pEndDate);
int dateToDays(Date* pDate);
int isLeapYear(int year);



#endif
