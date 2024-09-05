#ifndef __DEPATMENT__
#define __DEPATMENT__

#include "Treatment.h"
#include "Doctor.h"
#include "General.h"


typedef struct
{
    char* name;
    char departmentID[MAX_ID_LEN+1];
    eSpecialty specialtyType;//declared in General
    Doctor **DoctorsArr;
    int doctorCount;
    Treatment* treatmentsArr;
    int treatmentsCount;
}Department;

void	initDepartment(Department* pDepartment, Department** DepartArr, int departmentCount);
int addTreatInDepar(Department* pDepartment,Patient** PatientArr,int countPatient);

void removeDoctor(Department* pDepartment, const Doctor* pDoctor);
void addSpecificDocToDepartment(Department* pDepartment, Doctor* pDoctor);
void printDoctorTreatments(const Doctor* selectedDoctor, const Department* previousDepartment);

char* getCorrectNameDep(Department** pDepartmentArr, int departCount);
int isDepartmentExists(int userChoice, Department** pDepartmentArr, int departCount);

void    printDepartmentArr(const Department** pDepartArr, int countDepar);
void    printDepartment(const Department* pDepart);
void	freeDepartment(Department* pDepart);

int		addDoctorToDepar(Department* pDepartment);




#endif