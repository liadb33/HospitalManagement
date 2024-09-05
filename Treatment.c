#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>


#include "Treatment.h"


void initTreatment(Treatment* pTreat, Patient** pPatientArr,int countPatient, char* DoctorID) {
    //already done the check if patient exists

    pTreat->treatmentPatient = setPatientAtTreatment(pPatientArr, countPatient);
  
    strcpy(pTreat->doctorID,DoctorID);

    pTreat->treatmentID = rand() % 9000 + 1000; //num between 1000-9999

    getCorrectDate(&pTreat->startDate,"Enter start date Treatment (dd/mm/yyyy): ");

    getCorrectDate(&pTreat->endDate,"Enter end date Treatment (dd/mm/yyyy): ");
}


void printTreatmentArrInDepartment(const Treatment* pTreatmentArr, int countTreatment) {
    for (int i = 0; i < countTreatment; i++) {
        printf("|                                              |\n");
        printTreatment(&pTreatmentArr[i]);
    }
}

void printTreatment(const void* p1) {
    const Treatment* pTreat = (const Treatment*)p1;
    printf("|    Treatment ID: %-27d |\n", pTreat->treatmentID);

    if (pTreat->treatmentPatient != NULL) {
        printf("|    Patient's name: %-16s          |\n|    Patient's ID:  %-11s                |\n", pTreat->treatmentPatient->name, pTreat->treatmentPatient->patientID);
    } else {
        printf("|    Patient Details: Not available           |\n");
    }

    printf("|    Doctor ID: %-30s |\n", pTreat->doctorID);
    printf("|    Start Date: ");
    printDate(&pTreat->startDate);
    printf(" End Date: ");
    printDate(&pTreat->endDate);
    printf(" |\n");
}




