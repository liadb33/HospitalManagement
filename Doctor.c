#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h> //for rand

#include "Doctor.h"

//The specialtyType corresponds to the specialtyType of the departments. (from the same enum)
int	initDoctor(Doctor* pDoc, int specialtyType){
    printf("-----------  Init Doctor\n");
    getCorrectDocName(pDoc);

    char ID[MAX_PER_ID_LEN + 1];
    getCorrectID(ID);
    strcpy(pDoc->doctorID,ID);
    pDoc->specialtyType = (eSpecialty)specialtyType;//declared in General
    pDoc->yearExperience = (rand() % 49) + 1; //1-50
    return 1;
}

//Detailed in the report
//lINA Linu fLAt  --->  Dr. Flat
void getCorrectDocName(Doctor* pDoc) {
    char* name = getStrExactName("Enter doctor name:");
    char* lastName = getLastWord(name);
    capitalizeFirstLetter(lastName);

    char formattedName[MAX_STR_LEN]; 

    snprintf(formattedName, sizeof(formattedName), "Dr. %s", lastName);
    printf("Formatted Doctor Name: %s\n", formattedName);

    pDoc->name = getDynStr(formattedName);

    free(name);
}



char* getLastWord(const char* name) {//return the last word in str
    char* lastWord = strrchr(name, ' ');
    if (lastWord != NULL)
        return lastWord + 1; // Skip the space
    return (char*)name; // If no space (one word), return the original name
}

//the user choose the doctor he wants in the treatment
//the function return Doctor's ID
char* getDoctorAtTreatment(Doctor** DoctorsArr, int doctorCount) {
    int DoctorChoice;
    for (int i = 0; i < doctorCount; ++i) {
        printf("%d. ", (i+1));
        printf("\n+--------------------------------------------+\n");
        printDoctor(DoctorsArr[i]);
        printf("+--------------------------------------------+\n");
    }
    printf("Choose the doctor to the treatment, ");
    do {
        printf("Enter Valid Number (1-%d): ", doctorCount);
        scanf("%d", &DoctorChoice);
    } while (DoctorChoice < 1 || DoctorChoice > doctorCount);
    return DoctorsArr[doctorCount - 1]->doctorID;
}

void printDoctorArrInDepartment(const Doctor** pDoctorArr, int countDoctor) {
    for (int i = 0; i < countDoctor; i++) {
        printf("|                                              |\n");
        printDoctor(pDoctorArr[i]);
    }
}

void printDoctor(const Doctor* pDoc) {
    printf("|    Doctor's name: %-26s |\n", pDoc->name);
    printf("|    ID: %-37s |\n", pDoc->doctorID);
    printf("|    Specialty: %-30s |\n", getSpecialtyTypeStr(pDoc->specialtyType));
    printf("|    Year Experience: %-24d |\n", pDoc->yearExperience);
}

void freeDoctor( Doctor* pDoc) {
        free(pDoc->name);
}

