#ifndef __HOSPITAL__
#define __HOSPITAL__

#include "Department.h"
#include "MedicalProvider.h"

typedef enum{eUnsorted,eName,eID,eEndDate,eNofSort} sortType;
static const char* sortName[eNofSort] = {"Unsorted","Name","ID","End Date"};

typedef enum{eComparePatientByName,eComparePatientByID,eComparePatientByEndValidDate} compareType;
static const int (*compare[])(const void*, const void*) = {
        NULL,
        (const int (*)(const void *, const void *))comparePatientByName,
        (const int (*)(const void *, const void *)) comparePatientByID,
        (const int (*)(const void *, const void *))comparePatientByEndValidDate};


typedef struct
{
    char* name;
    int departmentCount;
    Department** DepartmentsArr;
    int PatientCount;
    Patient** PatientArr;
    sortType PatientSortType;
    MedicalProvider MedicalProvider;
}Hospital;

int        initHospital(Hospital* pHospital);

void        addDepartment(Hospital* pHospital);
void        addPatient(Hospital* pHospital);
void        addTreatment(Hospital* pHospital);
void        addDoctor(Hospital* pHospital);

void        sortPatient(Hospital* pHospital);
sortType    getPatientSortType();
const char* getPatientSortTypeStr(int type);
void        findPatient(const Hospital* pHospital);

void        replaceDocSpecialty(Hospital* pHospital);
int         getTotalDoctorCount(const Hospital* pHospital);
Doctor*     getSelectedDoctor(const Hospital* pHospital, int doctorChoice);
Department* getDepartmentByDoctor(const Hospital* pHospital, const Doctor* pDoctor);
Department* getDepartmentBySpecialty(const Hospital* pHospital, eSpecialty specialty);
Doctor*     selectDoctor(const Hospital* pHospital);
eSpecialty  selectNewSpecialty(const Hospital* pHospital, const Doctor* selectedDoctor);

void        isHospitalMatchedCorrectly(Hospital* pHospital);
int         isDepartmentMatchedCorrectly(Department* department, Hospital* pHospital);
int         getYearExperienceFromDocID(const Hospital* pHospital, char* doctorID);

void        printAllDoctorsInHospital(const Hospital* pHospital);
void	    printHospital(const Hospital* pHospital);
void	    freeHospital(Hospital* pHospital);


#endif