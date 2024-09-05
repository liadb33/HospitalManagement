#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "HealthPolicy.h"


void initHealthPolicy(HealthPolicy* pInsurance){
    do{
        getCorrectDate(&pInsurance->startValidDate, "Enter Insurance Coverage Start Date (dd/mm/yyyy): \n");

        getCorrectDate(&pInsurance->endValidDate,"Enter Insurance Coverage End Date (dd/mm/yyyy): \n");
    } while (!checkIfDatePolicyValid(&pInsurance->startValidDate,&pInsurance->endValidDate));
    //checks if for TODAY there is valid Policy
}



void initMaccabiPolicy(HealthPolicy* pMaccabi) {
    char statusInput[MAX_STR_LEN];
    initHealthPolicy(pMaccabi);
    pMaccabi->print = (void (*)(const struct HealthPolicy_ *)) printMaccabiPolicy;

    getHealthPolicyStatusMaccabi(statusInput);
    strcpy(pMaccabi->InsuranceType.maccabi.HealthStatus, statusInput);
    pMaccabi->InsuranceType.maccabi.discount = getDiscountPercentage();

}
void initClalitPolicy(HealthPolicy* pClalit) {
    char statusInput[MAX_STR_LEN];
    initHealthPolicy(pClalit);
    pClalit->print = (void (*)(const struct HealthPolicy_ *)) printClalitPolicy;

    getHealthPolicyStatusClalit(statusInput);
    strcpy(pClalit->InsuranceType.clalit.HealthStatus, statusInput);
    pClalit->InsuranceType.clalit.discount = getDiscountPercentage();

}
void initMeuhedetPolicy(HealthPolicy* pMeuhedet) {

    initHealthPolicy(pMeuhedet);
    pMeuhedet->print = (void (*)(const struct HealthPolicy_ *)) printMeuhedetPolicy;

    strcpy(pMeuhedet->InsuranceType.meuhedet.HealthStatus, "STANDARD");
    printf("Health Policy Status set to STANDARD automatically.\n");
    pMeuhedet->InsuranceType.meuhedet.discount = getDiscountPercentage();

}

void printPolicy(const void* p1) {
    const HealthPolicy* pInsurance = (const HealthPolicy*)p1;
    pInsurance->print((const struct HealthPolicy_ *)pInsurance);
}


void printPolicyNoUnion(const void* p1) {
    const HealthPolicy* pInsurance = (const HealthPolicy*)p1;
    printf("|    Insurance Type: %-26s       |\n", getInsuranceTypeStr(pInsurance->type));

    printf("|    Insurance Coverage: "); 
    printDate(&pInsurance->startValidDate);
    printf(" - ");
    printDate(&pInsurance->endValidDate);
    printf("        |");
    printf("\n");
}


void printMaccabiPolicy(const HealthPolicy* pMaccabi) {
    printPolicyNoUnion(pMaccabi);
    printf("|    Health Policy Status: %-20s       |\n", pMaccabi->InsuranceType.maccabi.HealthStatus);
    printf("|    Discount for basic treatment: %d%%                |\n", pMaccabi->InsuranceType.maccabi.discount);
}
void printClalitPolicy(const HealthPolicy* pClalit) {
    printPolicyNoUnion(pClalit);
    printf("|    Health Policy Status: %-20s       |\n", pClalit->InsuranceType.clalit.HealthStatus);
    printf("|    Discount for basic treatment: %d%%                |\n", pClalit->InsuranceType.clalit.discount);
}
void printMeuhedetPolicy(const HealthPolicy* pMeuhedet) {
    printPolicyNoUnion(pMeuhedet);
    printf("|    Health Policy Status: %-20s       |\n", pMeuhedet->InsuranceType.meuhedet.HealthStatus);
    printf("|    Discount for basic treatment: %d%%                |\n", pMeuhedet->InsuranceType.meuhedet.discount);
}


const char* getInsuranceTypeStr(int type) {
    if (type < 0 || type >= eNofInsuranceTypes)
        return NULL;
    return InsuranceTypeStr[type];
}


void getHealthPolicyStatusMaccabi(char* statusInput) {
    
    int isValidStatus = 0;
    do {
        printf("Enter Health Policy Status (VIP/REG): ");
        fGets(statusInput, POLICY_STATUS_MACCABI + 1, stdin);
        capitalizeAllLetters(statusInput);
        if (strcmp(statusInput, "VIP") == 0 || strcmp(statusInput, "REG") == 0) {
            isValidStatus = 1;
        } else {
            printf("Invalid input. Please enter VIP or REG.\n");
        }
    } while (!isValidStatus);
}

void getHealthPolicyStatusClalit(char* statusInput) {
   
    int isValidStatus = 0;
    do {
        printf("Enter Health Policy Status (PREM/BASE): ");
        fGets(statusInput, POLICY_STATUS_CLALIT + 1, stdin);
        capitalizeAllLetters(statusInput);
        if (strcmp(statusInput, "PREM") == 0 || strcmp(statusInput, "BASE") == 0) {
            isValidStatus = 1;
        } else {
            printf("Invalid input. Please enter PREM or BASE.\n");
        }
    } while (!isValidStatus);
}

int getDiscountPercentage() {
    int discount;
    do {
        printf("Enter Discount for basic treatment in Percent (1-100): ");
        scanf("%d", &discount);
    } while (discount < 1 || discount > 100);
    return discount;
}

