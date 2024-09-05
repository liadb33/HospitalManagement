#include <stdio.h>
#include <stdlib.h>
#include "Hospital.h"
#include "HospitalFile.h"
#include "General.h"
#include "main.h"

int main() {

    Hospital hospital;
    int option;
    int stop = 0;
    printWelcome();

    option = loadDateMenu();
        switch (option) {
            case 1:
                readHospitalFromTextFile(&hospital, HOSPITAL_TEXT_FILE_SRC);
                break;
            case 2:
                readHospitalFromBFile(&hospital, HOSPITAL_BIN_FILE_SRC);
                break;
            case 3:
                initHospital(&hospital);
                break;
            default:
                printf("Invalid option!!\n");
                break;
        }

    do {
        option = menu();
        switch (option) {
            case ePrintHospital:
                printHospital(&hospital);
                break;
            case ePrintAllDoctorsInHospital:
                printAllDoctorsInHospital(&hospital);
                break;
            case eAddDepartment:
                addDepartment(&hospital);
                break;
            case eAddDoctor:
                addDoctor(&hospital);
                break;
            case eAddTreatment:
                addTreatment(&hospital);
                break;
            case eAddPatient:
                addPatient(&hospital);
                break;
            case eSortPatient:
                sortPatient(&hospital);
                break;
            case eFindPatient:
                findPatient(&hospital);
                break;
            case eReplaceDocSpecialty:
                replaceDocSpecialty(&hospital);
                break;
            case eIsHospitalMatchedCorrectly:
                isHospitalMatchedCorrectly(&hospital);
                break;
            case EXIT:
                stop = 1;
                break;
            default:
                printf("Wrong option\n");
                break;
        }
    } while (!stop);

    saveHospitalToTextFile(&hospital, HOSPITAL_TEXT_FILE_DST);
    saveHospitalToBinaryFile(&hospital,HOSPITAL_BIN_FILE_DST);

    freeHospital(&hospital);
    return 0;
}

int menu()
{
    int option;
    printf("\n\n");
    printf("Please choose one of the following options\n");
    for(int i = 0 ; i < eNofOptions ; i++)
        printf("%d - %s\n",i,str[i]);
    printf("%d - Quit\n", EXIT);
    scanf("%d", &option);
    //clean buffer
    char tav;
    scanf("%c", &tav);
    return option;
}

int loadDateMenu()
{
    int option;
    printf("Choose option for hospital Initial Setup:\n");
    printf("1. load date from text file\n");
    printf("2. load date from binary file\n");
    printf("3. Insert data manually\n");

    do{
        printf("Please choose number between 1 - 3:\n");
        scanf("%d", &option);
    }while(option < 1 || option > 3);

    //clean buffer
    char tav;
    scanf("%c", &tav);
    return option;
}
