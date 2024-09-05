#ifndef __PATIENT__
#define __PATIENT__

#include "General.h"
#include "HealthPolicy.h"

typedef struct
{
    char* name;
    char patientID[MAX_PER_ID_LEN + 1];
    HealthPolicy healthPolicy;
}Patient;

int	    initPatient(Patient* pPatient);
Patient*    setPatientAtTreatment(Patient** PatientArr, int countPatient);
void    printPatientArr(const Patient** PatientArr, int countPatient);
void	printPatient(const Patient* pPatient);
void	freePatient(Patient* pPatient);

//used for Sort Patient
int     comparePatientByName(const void* f1, const void* f2);
int     comparePatientByID(const void* f1, const void* f2);
int     comparePatientByEndValidDate(const void* t1, const void* t2);

//used for Find Patient
void    getPatientName(char* name);
void    getPatientID(char* id);








#endif