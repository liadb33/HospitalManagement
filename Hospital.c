#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "Hospital.h"

int       initHospital(Hospital* pHospital){
    pHospital->name = getStrExactName("Enter hospital name: ");
    pHospital->DepartmentsArr = NULL;
    pHospital->departmentCount = 0;
    pHospital->PatientArr =NULL;
    pHospital->PatientCount = 0;
    pHospital->PatientSortType = 0;
    if(!initMedicalProvider(&pHospital->MedicalProvider))
        ERROR_MSG_AND_RETURN("Error init Medical Provider ", 0);
    return 1;
}

void addPatient(Hospital* pHospital) {
    if (pHospital->PatientCount >= MAX_PATIENTS)
        ERROR_MSG_AND_RETURN_VOID("Cannot add more patients. Maximum limit reached.\n");

    Patient* newPatient = (Patient*)malloc(sizeof(Patient));
    if (!newPatient)
        ERROR_MSG_AND_RETURN_VOID("Memory allocation failed.\n");

    if (initPatient(newPatient)) {
        pHospital->PatientArr = (Patient**)realloc(pHospital->PatientArr, sizeof(Patient*) * (pHospital->PatientCount + 1));
        pHospital->PatientArr[pHospital->PatientCount] = newPatient;
        pHospital->PatientCount++;
        pHospital->PatientSortType = 0;
        printf("Patient added successfully.\n");
    } else {
        printf("Failed to initialize patient.\n");
        free(newPatient);
    }
    printGoodLuck();
}

void addTreatment(Hospital* pHospital){
    if (pHospital->PatientCount == 0)ERROR_MSG_AND_RETURN_VOID("There is no patients in the reception hospital\n ");

    int DepartmentChoice;
    printf("Choose the Department you want to add Treatment\n");
    for (int i = 0; i < pHospital->departmentCount; ++i) {
        printf("Department %d: %s\n", (i + 1) , pHospital->DepartmentsArr[i]->name);
    }

    do{
        printf("Enter Valid Number (1-%d): ",pHospital->departmentCount);
        scanf("%d",&DepartmentChoice);
    }while(DepartmentChoice < 1 || DepartmentChoice > pHospital->departmentCount);

    if (pHospital->DepartmentsArr[DepartmentChoice-1]->doctorCount == 0)
        ERROR_MSG_AND_RETURN_VOID("There are no doctors in the department\n ");

    addTreatInDepar(pHospital->DepartmentsArr[DepartmentChoice-1],pHospital->PatientArr,pHospital->PatientCount);

    addUnitAtEquipmentList(pHospital->DepartmentsArr[DepartmentChoice-1]->specialtyType,&pHospital->MedicalProvider.EquipmentList);

    printGoodLuck();
}

void addDepartment(Hospital* pHospital) {
    if (pHospital->departmentCount >= MAX_DEPARTMENTS-1)
        ERROR_MSG_AND_RETURN_VOID("Cannot add more departments. Maximum limit reached.\n");

    Department* newDepartment = (Department*)calloc(1, sizeof(Department));
    if (!newDepartment)
        ERROR_MSG_AND_RETURN_VOID("Memory allocation failed.\n");

    initDepartment(newDepartment,pHospital->DepartmentsArr,pHospital->departmentCount);

    pHospital->DepartmentsArr = (Department**)realloc(pHospital->DepartmentsArr, sizeof(Department*) * (pHospital->departmentCount + 1));
    if (!pHospital->DepartmentsArr) {
        free(pHospital->DepartmentsArr);
        ERROR_MSG_AND_RETURN_VOID("Memory allocation failed. \n");
    }
    pHospital->DepartmentsArr[pHospital->departmentCount] = newDepartment;
    pHospital->departmentCount++;

    printf("Department added successfully.\n");
}

void sortPatient(Hospital* pHospital){

    if(pHospital->PatientCount == 0) ERROR_MSG_AND_RETURN_VOID("No patients to sort\n");

    pHospital->PatientSortType = getPatientSortType();

    qsort(pHospital->PatientArr, pHospital->PatientCount, sizeof(Patient**),
          (int (*)(const void *, const void *)) (const int (*)(const void *, const void *)) compare[pHospital->PatientSortType]);
}

sortType getPatientSortType() {
    int option;
    printf("\nBase on what field do you want to sort?");

    do {
        printf("\n");
        for (int i = 1; i < eNofSort; i++)
            printf("Enter %d for %s\n", i, getPatientSortTypeStr(i));
        scanf("%d", &option);
    } while (option < 1 || option >= eNofSort);
    return (sortType)option;
}

const char* getPatientSortTypeStr(int type){
    if (type < 0 || type >= eNofSort)
        return NULL;
    return sortName[type];
}


void findPatient(const Hospital* pHospital){
    if (!pHospital->PatientSortType) ERROR_MSG_AND_RETURN_VOID("The search cannot be performed, array not sorted\n")

    Patient temp = { 0 };
    Patient* pTemp = &temp;

    switch (pHospital->PatientSortType) {
        case eName:
            getPatientName(temp.name);
            break;
        case eID:
            getPatientID(temp.patientID);
            break;
        case eEndDate:
            getEndDate(&temp.healthPolicy.endValidDate);
            break;
        default:
            return;
    }

    Patient** pFound = (Patient**)bsearch(&pTemp, pHospital->PatientArr, pHospital->PatientCount, sizeof(Patient*),
                                        (int (*)(const void *, const void *)) compare[pHospital->PatientSortType]);

    if (!pFound)
        printf("Patient was not found");
    else {
        printf("Patient found, ");
        printPatient(*pFound);
    }
}

void replaceDocSpecialty(Hospital* pHospital){
    if (pHospital->departmentCount == 0 || getTotalDoctorCount(pHospital) == 0)
        ERROR_MSG_AND_RETURN_VOID("No departments or doctors available.\n");

    //choose the Doctor
    Doctor* selectedDoctor = selectDoctor(pHospital);
    if (selectedDoctor == NULL) ERROR_MSG_AND_RETURN_VOID("Not valid select Doctor");

    //choose the new Specialty (not original specialty)
    eSpecialty newSpecialty = selectNewSpecialty(pHospital,selectedDoctor);

    Department* targetDepartment = getDepartmentBySpecialty(pHospital, newSpecialty);
    if (targetDepartment == NULL)
        ERROR_MSG_AND_RETURN_VOID("No department found with the chosen specialty.\n");

    //find the Department, remove and add the doctor
    Department* previousDepartment = getDepartmentByDoctor(pHospital, selectedDoctor);
    removeDoctor(previousDepartment, selectedDoctor);
    selectedDoctor->specialtyType = newSpecialty;
    addSpecificDocToDepartment(targetDepartment, selectedDoctor);

    //print doctors all Treatments include from **previous department**
    printf("Doctor %s specialty changed successfully to %s.\n", selectedDoctor->name, getSpecialtyTypeStr(newSpecialty));
    printDoctorTreatments(selectedDoctor, previousDepartment);
}

//choose the Doctor to replace specialty
Doctor* selectDoctor(const Hospital* pHospital) {
    printAllDoctorsInHospital(pHospital);

    int doctorChoice;
    printf("\nEnter the number of the doctor you want to switch specialty");
    do{
        printf("\nnumber between 1-%d, and the doctor should be minimum 10 years experience: ",getTotalDoctorCount(pHospital));
        scanf("%d", &doctorChoice);
    }while(doctorChoice < 1 || doctorChoice > getTotalDoctorCount(pHospital));

    Doctor* selectedDoctor = getSelectedDoctor(pHospital, doctorChoice);
    if (selectedDoctor == NULL)
        ERROR_MSG_AND_RETURN("Invalid doctor selection.\n",NULL);

    if (selectedDoctor->yearExperience < 10)
        ERROR_MSG_AND_RETURN("The selected doctor does not have enough experience (minimum 10 years) to switch specialty.\n",NULL);

    return selectedDoctor;
}

//choose the Doctor NEW specialty
eSpecialty selectNewSpecialty(const Hospital* pHospital,const Doctor* selectedDoctor) {

    printf("\nCurrent specialty: %s\n", getSpecialtyTypeStr(selectedDoctor->specialtyType));

    printf("Available specialties:\n");
    for (int i = 1; i <= pHospital->departmentCount; i++) {
        printf("%d. %s\n", i, getSpecialtyTypeStr(pHospital->DepartmentsArr[i-1]->specialtyType));
    }

    int specialtyChoice;
    int notCurrentSpeciality;

    printf("Enter the number of the new specialty (not current specialty\n");
    do{
        notCurrentSpeciality = 0;
        printf("number between 1-%d: \n",pHospital->departmentCount);
        scanf("%d", &specialtyChoice);
        if (specialtyChoice == selectedDoctor->specialtyType) {
            notCurrentSpeciality = 1;
            printf("Not allowed enter current specialty\n");
        }
    }while(specialtyChoice < 1 || specialtyChoice > pHospital->departmentCount || notCurrentSpeciality);


    return (eSpecialty)(specialtyChoice);
}

//mount of doctors in ALL HOSPITAL
int getTotalDoctorCount(const Hospital* pHospital) {
    int totalCount = 0;
    for (int i = 0; i < pHospital->departmentCount; i++) {
        totalCount += pHospital->DepartmentsArr[i]->doctorCount;
    }
    return totalCount;
}

//The function goes through all its departments and doctors
//return pointer to the doctor according to the user's choice
Doctor* getSelectedDoctor(const Hospital* pHospital, int doctorChoice) {
    int currentIndex = 0;
    for (int i = 0; i < pHospital->departmentCount; i++) {
        Department* department = pHospital->DepartmentsArr[i];
        if (doctorChoice <= currentIndex + department->doctorCount) {
            return (Doctor*)department->DoctorsArr[doctorChoice - currentIndex - 1];
        }
        currentIndex += department->doctorCount;
    }
    return NULL;
}

//return the Department which the doctor belong
Department* getDepartmentByDoctor(const Hospital* pHospital, const Doctor* pDoctor) {
    for (int i = 0; i < pHospital->departmentCount; i++) {
        Department* department = pHospital->DepartmentsArr[i];
        for (int j = 0; j < department->doctorCount; j++) {
            if (department->DoctorsArr[j] == pDoctor) {
                return department;
            }
        }
    }
    return NULL;
}

//return the department with Specialty
Department* getDepartmentBySpecialty(const Hospital* pHospital, eSpecialty specialty) {
    for (int i = 0; i < pHospital->departmentCount; i++) {
        if (pHospital->DepartmentsArr[i]->specialtyType == specialty) {
            return pHospital->DepartmentsArr[i];
        }
    }
    return NULL;
}


void printDoctorTreatments(const Doctor* selectedDoctor, const Department* previousDepartment) {
    printf("Reminder: Doctor %s still has obligations to the following treatments in the previous department:\n", selectedDoctor->name);
    int treatmentCount = 0;
    for (int i = 0; i < previousDepartment->treatmentsCount; i++) {
        Treatment* treatment = &previousDepartment->treatmentsArr[i];
        if (strcmp(treatment->doctorID,selectedDoctor->doctorID)==0) {
            treatmentCount++;
            printf("Treatment number %d between the dates in department %s\n", treatmentCount, previousDepartment->name);
            printDate(&treatment->startDate);
            printf(" - ");
            printDate(&treatment->endDate);
            printf("\n");
        }
    }
    if (treatmentCount == 0) {
        printf("No ongoing treatments found.\n");
    }
}

//more detailed in the report
//Checks compatibility between length of treatment and doctor's year experience per department
void isHospitalMatchedCorrectly(Hospital* pHospital) {
    if (pHospital->departmentCount == 0) ERROR_MSG_AND_RETURN_VOID("No departments available.\n");

    for (int i = 0; i < pHospital->departmentCount; i++) {
        Department* department = pHospital->DepartmentsArr[i];
        printf("In department number %d there are %d treatments:\n", (i + 1), department->treatmentsCount);

        int matchFlag = isDepartmentMatchedCorrectly(department, pHospital);

        if (matchFlag == 1)
            printf("Department number %d is matched correctly!\n\n", (i + 1));
        else
            printf("Department number %d is not matched correctly.\n\n", (i + 1));
    }
}

//sub function of isHospitalMatchedCorrectly
int isDepartmentMatchedCorrectly(Department* department, Hospital* pHospital) {
    int maxYearExperience = 0, maxDaysLongTreat = 0, maxLengthTreatmentID = 0;
    char maxExpDoctorID[MAX_PER_ID_LEN+1];

    for (int j = 0; j < department->treatmentsCount; j++) {
        Treatment* treatment = &department->treatmentsArr[j];
        int daysLongTreat = countBetweenDays(&treatment->startDate, &treatment->endDate);
        int yearExperience = getYearExperienceFromDocID(pHospital, treatment->doctorID);
        printf("  Treatment number %d is %d days long and the doctor has %d years of experience.\n", (j + 1), daysLongTreat, yearExperience);

        if (yearExperience > maxYearExperience) {
            maxYearExperience = yearExperience;
            strcpy(maxExpDoctorID, treatment->doctorID);
        }
        if (daysLongTreat > maxDaysLongTreat) {
            maxDaysLongTreat = daysLongTreat;
            maxLengthTreatmentID = treatment->treatmentID;
        }
    }

    for (int j = 0; j < department->treatmentsCount; j++) {
        Treatment* treatment = &department->treatmentsArr[j];
        if (treatment->treatmentID == maxLengthTreatmentID && strcmp(treatment->doctorID,maxExpDoctorID)!= 0) {
            return 0;
        }
    }
    return 1;
}

void addDoctor(Hospital* pHospital){
    int deparChoice;
    printf("Choose to which department you want to add Doctor:\n");
    for (int i = 0; i < pHospital->departmentCount; ++i)
        printf("%d. %s Department\n",(i+1), pHospital->DepartmentsArr[i]->name);

    do{
        printf("Enter number between (1-%d): ", pHospital->departmentCount);
        scanf("%d",&deparChoice);
    }while(deparChoice < 1 || deparChoice > pHospital->departmentCount);
    if(!addDoctorToDepar(pHospital->DepartmentsArr[deparChoice-1]))
        return;
}


int getYearExperienceFromDocID(const Hospital* pHospital, char* doctorID) {
    for (int i = 0; i < pHospital->departmentCount; i++) {
        const Department* department = pHospital->DepartmentsArr[i];
        for (int j = 0; j < department->doctorCount; j++) {
            const Doctor* doctor = department->DoctorsArr[j];
            if (strcmp(doctor->doctorID,doctorID)==0)
                return doctor->yearExperience;
        }
    }

    return -1; // Doctor not found
}

void printAllDoctorsInHospital(const Hospital* pHospital) {
    if (pHospital->departmentCount == 0) ERROR_MSG_AND_RETURN_VOID("No doctors in the hospital\n");
    printf("List of Doctors:\n");
    int doctorCount = 1;
    for (int i = 0; i < pHospital->departmentCount; i++) {
        Department* pDepartment = pHospital->DepartmentsArr[i];
        for (int j = 0; j < pDepartment->doctorCount; j++) {
            Doctor* pDoctor = (Doctor*)pDepartment->DoctorsArr[j];
            printf("%d. %s (Specialty: %s, Experience: %d years)\n", doctorCount, pDoctor->name, getSpecialtyTypeStr(pDoctor->specialtyType), pDoctor->yearExperience);
            doctorCount++;
        }
    }
}



void printHospital(const Hospital* pHospital) {
    printf("\n+----------------------------------------------+\n");
    printf("|\t        %s%s Hospital%s\t               |\n", ANSI_BOLD_ON, pHospital->name, ANSI_BOLD_OFF);
    printf("+----------------------------------------------+\n");

    printf("\nThere are %d departments\n", pHospital->departmentCount);
    printDepartmentArr(pHospital->DepartmentsArr, pHospital->departmentCount);

    printf("\nThere are %d Patients\n", pHospital->PatientCount);
    printPatientArr(pHospital->PatientArr, pHospital->PatientCount);

    printf("Type Patient Sort: %s\n", getPatientSortTypeStr(pHospital->PatientSortType));

    printMedicalProvider(&pHospital->MedicalProvider);
}

void freeHospital(Hospital* pHospital) {

    free(pHospital->name);

    for (int i = 0; i < pHospital->departmentCount; i++) {
        freeDepartment(pHospital->DepartmentsArr[i]);
        free(pHospital->DepartmentsArr[i]);
    }
    free(pHospital->DepartmentsArr);

    for (int j = 0; j < pHospital->PatientCount; j++) {
        freePatient(pHospital->PatientArr[j]);
        free(pHospital->PatientArr[j]);
    }
    free(pHospital->PatientArr);

    L_free(&pHospital->MedicalProvider.EquipmentList, freeEquipment);
}
