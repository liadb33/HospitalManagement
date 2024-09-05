#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "Department.h"

void	initDepartment(Department* pDepartment, Department** pDepartmentArr, int departmentCount){
    
    int tempID = (rand() % 9000) + 1000;
    
    pDepartment->name = (char*)getCorrectNameDep(pDepartmentArr,departmentCount);
    sprintf(pDepartment->departmentID, "%d", tempID);
    pDepartment->specialtyType = (eSpecialty)fromNameToSpecialtyType(pDepartment->name);
    pDepartment->DoctorsArr = NULL;
    pDepartment->doctorCount = 0;
    pDepartment->treatmentsArr = NULL;
    pDepartment->treatmentsCount = 0;
}

int addTreatInDepar(Department* pDepartment,Patient** PatientArr,int countPatient) {
    Treatment newTreatment;

    //already done the check if Doctor exists
    char* tempDoctorID = getDoctorAtTreatment(pDepartment->DoctorsArr,pDepartment->doctorCount);

    initTreatment(&newTreatment,PatientArr, countPatient,tempDoctorID);

    //memory for the new Treatment in the treatmentsArr
    pDepartment->treatmentsArr = (Treatment*)realloc(pDepartment->treatmentsArr, (pDepartment->treatmentsCount + 1) * sizeof(Treatment));
    if (pDepartment->treatmentsArr == NULL) ERROR_MSG_AND_RETURN("Memory allocation failed.\n",0);

    //copy the new Treatment to the treatmentsArr
    pDepartment->treatmentsArr[pDepartment->treatmentsCount] = newTreatment;
    pDepartment->treatmentsCount++;

    return 1;
}

int addDoctorToDepar(Department* pDepartment) {
    Doctor* pDoc = (Doctor*)malloc(sizeof(Doctor));
    if (!pDoc) ERROR_MSG_AND_RETURN("Memory allocation failed.\n",0);

    if (!initDoctor(pDoc, pDepartment->specialtyType))
        FREE_ERROR_MSG_AND_RETURN(pDoc,"Error initializing doctor\n", 0);

    pDepartment->DoctorsArr = (Doctor**)realloc(pDepartment->DoctorsArr, (pDepartment->doctorCount + 1) * sizeof(Doctor*));
    if (!pDepartment->DoctorsArr)
        FREE_ERROR_MSG_AND_RETURN(pDoc,"Memory allocation failed.\n", 0);

    //add the new Doctor to the DoctorsArr
    pDepartment->DoctorsArr[pDepartment->doctorCount] = pDoc;
    pDepartment->doctorCount++;

    printGoodLuck();

    ERROR_MSG_AND_RETURN("Doctor added successfully\n",1);
}

//delete Doctor Department
void removeDoctor(Department* pDepartment, const Doctor* pDoctor) {
    for (int i = 0; i < pDepartment->doctorCount; i++) {
        if (pDepartment->DoctorsArr[i] == pDoctor) {
            for (int j = i; j < pDepartment->doctorCount - 1; j++) {
                pDepartment->DoctorsArr[j] = pDepartment->DoctorsArr[j + 1];
            }
            pDepartment->doctorCount--;
            break;
        }
    }
}

//add doctor the already known and not add new one
void addSpecificDocToDepartment(Department* pDepartment, Doctor* pDoctor){
    pDepartment->DoctorsArr = (Doctor**)realloc(pDepartment->DoctorsArr, (pDepartment->doctorCount + 1) * sizeof(Doctor*));
    if (pDepartment->DoctorsArr != NULL) {
        pDepartment->DoctorsArr[pDepartment->doctorCount] = pDoctor;
        pDepartment->doctorCount++;
    }
}


char* getCorrectNameDep(Department** pDepartmentArr, int departCount) {
    int tempName,isValid;
    for (int i = 1; i < eNofSpecialtyTypes; ++i)
        printf("%d. %s\n",i,SpecialtyTypeStr[i]);
    printf("Choose the department you want to add \n");

    do{
        isValid = 0;
        printf("Enter number: (1-%d)\n",eNofSpecialtyTypes-1);
        scanf("%d",&tempName);
        isValid = isDepartmentExists(tempName,pDepartmentArr, departCount);
    }while(tempName<1 || tempName >= eNofSpecialtyTypes || !isValid);

    return (char*) getSpecialtyTypeStr(tempName);
}

int isDepartmentExists(int userChoice, Department** pDepartmentArr, int departCount) {
    for (int i = 0; i < departCount; ++i) {
        if (pDepartmentArr[i]->specialtyType == (eSpecialty)userChoice) {
            ERROR_MSG_AND_RETURN("This department already exists\n",0);
        }
    }
    return 1;
}

void printDepartmentArr(const Department** pDepartArr, int countDepar) {
    for (int i = 0; i < countDepar; i++) {
        printf("\nDepartment %d: ", (i+1));
        printDepartment(pDepartArr[i]);
    }
}

void printDepartment(const Department* pDepart) {
    printf("\n+----------------------------------------------+\n");
    printf("|    Department name: %-24s |\n", pDepart->name);
    printf("|    Department ID: %-27s|\n", pDepart->departmentID);
    printf("|    Specialty: %-30s |\n", getSpecialtyTypeStr(pDepart->specialtyType));
    printf("|**********************************************|\n");
    if(pDepart->doctorCount == 0){
        printf("| There are no Doctors in the department       |\n");
    } else {
        printf("| The Doctors in the department:               |\n");
        printDoctorArrInDepartment(pDepart->DoctorsArr, pDepart->doctorCount);
    }
    printf("|**********************************************|\n");
    if(pDepart->treatmentsCount == 0){
        printf("| There are no Treatments in the department    |\n");
    } else {
        printf("| The Treatments in the department:            |\n");
        printTreatmentArrInDepartment(pDepart->treatmentsArr, pDepart->treatmentsCount);
    }
    printf("+----------------------------------------------+\n");
}

void freeDepartment(Department* pDepart) {
    
    //Free doctors array

    if (!pDepart->doctorCount)
        return;
    for (int i = 0; i < pDepart->doctorCount; i++) {
        freeDoctor(pDepart->DoctorsArr[i]);
        free(pDepart->DoctorsArr[i]);
    }
    free(pDepart->DoctorsArr);
    free(pDepart->treatmentsArr);
}

