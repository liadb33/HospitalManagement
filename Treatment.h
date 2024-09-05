#ifndef __TREATMENT__
#define __TREATMENT__

#include "Date.h"
#include "Patient.h"
#include "General.h"


typedef struct
{
    int treatmentID;
    Patient* treatmentPatient;
    char doctorID[MAX_PER_ID_LEN+1];
    Date startDate;
    Date endDate;

}Treatment;

void initTreatment(Treatment* pTreat, Patient** pPatientArr,int countPatient,char* DoctorID);
void printTreatmentArrInDepartment(const Treatment* pTreatmentArr, int countTreatment);
void printTreatment(const void* p1);






#endif