#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "Patient.h"


int initPatient(Patient* pPatient) {
    pPatient->name = getStrExactName("Enter patient name: ");
    char ID[MAX_PER_ID_LEN + 1];
    getCorrectID(ID);
    strcpy(pPatient->patientID,ID);

    int type;
    do {
        printf("Enter Insurance Type (0-Maccabi, 1-Clalit, 2-Meuhedet): \n");
        scanf("%d", &type);
    } while (type < 0 || type >= eNofInsuranceTypes);
    pPatient->healthPolicy.type = type;

    switch (pPatient->healthPolicy.type) {
        case 0:
            initMaccabiPolicy(&pPatient->healthPolicy);
            break;
        case 1:
            initClalitPolicy(&pPatient->healthPolicy);
            break;
        case 2:
            initMeuhedetPolicy(&pPatient->healthPolicy);
            break;
        default:
            break;
    }
    return 1;
}

//the user select the patient for the treatment
Patient* setPatientAtTreatment(Patient** PatientArr, int countPatient){
    int PatientChoice;
    printPatientArr(PatientArr, countPatient);
    printf("The treatment is for the patient\n");
    do{
        printf("Enter Valid Number (1-%d): ",countPatient);
        scanf("%d",&PatientChoice);
    }while(PatientChoice < 1 || PatientChoice > countPatient);
    return PatientArr[PatientChoice-1];
}

void printPatientArr(const Patient** pPatientArr, int countPatient) {
    for (int i = 0; i < countPatient; i++) {
        printf("%d.", i + 1);
        printPatient(pPatientArr[i]);
    }
}

void printPatient(const Patient* pPatient) {
    printf("\n+-----------------------------------------------------+\n");
    printf("|    Patient Name: %-27s        |\n",pPatient->name);
    printf("|    ID: %-37s        |\n", pPatient->patientID);
    printf("|*****************************************************|\n");
    printf("|                                                     |\n");
    printPolicy(&pPatient->healthPolicy);
    printf("+-----------------------------------------------------+\n");
}

void freePatient(Patient* pPatient) {
    free(pPatient->name);
}

int comparePatientByName(const void* t1, const void* t2) {
    const Patient* Patient1 = *(const Patient**)t1;
    const Patient* Patient2 = *(const Patient**)t2;

    return strcmp(Patient1->name,Patient2->name);
}

int comparePatientByID(const void* t1, const void* t2) {
    const Patient* Patient1 = *(const Patient**)t1;
    const Patient* Patient2 = *(const Patient**)t2;

    int tempID1, tempID2;
    sscanf(Patient1->patientID, "%d", &tempID1);
    sscanf(Patient2->patientID, "%d", &tempID2);

    return tempID1-tempID2;
}

int comparePatientByEndValidDate(const void* t1, const void* t2) {
    //Sorted by insurance policy END DATE
    //For example: the patient whose health policy expired first, he will be first in the array
    const Patient* Patient1 = *(const Patient**)t1;
    const Patient* Patient2 = *(const Patient**)t2;

    // Compare the end dates of the health policies
    if (Patient1->healthPolicy.endValidDate.year < Patient2->healthPolicy.endValidDate.year) {
        return -1;
    } else if (Patient1->healthPolicy.endValidDate.year > Patient2->healthPolicy.endValidDate.year) {
        return 1;
    } else {
        // If years are the same, compare months
        if (Patient1->healthPolicy.endValidDate.month < Patient2->healthPolicy.endValidDate.month) {
            return -1;
        } else if (Patient1->healthPolicy.endValidDate.month > Patient2->healthPolicy.endValidDate.month) {
            return 1;
        } else {
            // If months are also the same, compare days
            if (Patient1->healthPolicy.endValidDate.day < Patient2->healthPolicy.endValidDate.day) {
                return -1;
            } else if (Patient1->healthPolicy.endValidDate.day > Patient2->healthPolicy.endValidDate.day) {
                return 1;
            } else {
                return 0;
            }
        }
    }
}


//used for Find Patient
void getPatientName(char* name)
{
    char temp[MAX_STR_LEN];
    printf("Enter patient name: ");
    fGets(temp, MAX_STR_LEN, stdin);
    strcpy(name, temp);
}

//used for Find Patient
void getPatientID(char* id){
    char ID[MAX_STR_LEN];
    int isValid;
    printf("Enter the 9-digit ID (including the Check Digit): \n");

    do {
        isValid = 1;
        scanf("%s", ID);
        if (strlen(ID) != MAX_PER_ID_LEN) {
            printf("Invalid input. Please enter exactly 9 digits.\n");
            isValid = 0;
        }
        else {
            // Check if the input contains only digits
            for (int i = 0; i < MAX_PER_ID_LEN; i++) {
                if (!isdigit(ID[i])) {
                    printf("Invalid input. Please enter only digits.\n");
                    isValid = 0;
                    break;
                }
            }
        }
    } while (!isValid);

    strdup(ID);
    strcpy(id, ID);
}
