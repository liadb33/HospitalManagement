#ifndef __DOCTOR__
#define __DOCTOR__

#include "General.h"

typedef struct
{
    char* name;
    char doctorID[MAX_PER_ID_LEN+1];
    eSpecialty specialtyType;//declared in General
    int yearExperience;
}Doctor;

int	    initDoctor(Doctor* pDoc, int specialtyType);
char*   getDoctorAtTreatment(Doctor** DoctorsArr, int doctorCount);
void    printDoctorArrInDepartment(const Doctor** pDoctorArr, int countDoctor);

void	printDoctor(const Doctor* pDoc);
void	freeDoctor(Doctor* pDoc);

void    getCorrectDocName(Doctor* pDoc);
char*   getLastWord(const char* name);

#endif