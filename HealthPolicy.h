#ifndef __HEALTH_POLICY__
#define __HEALTH_POLICY__

#include "Date.h"
#include "General.h"

typedef enum { eMaccabi , eClalit , eMeuhedet, eNofInsuranceTypes} eInsuranceType;
static const char* InsuranceTypeStr[eNofInsuranceTypes] = { "Maccabi", "Clalit", "Meuhedet" };

typedef struct {
    char HealthStatus[POLICY_STATUS_MACCABI +1] ; //VIP,REG
    int discount; //Discount for basic treatment in Percent
}Maccabi;

typedef struct {
    char HealthStatus[POLICY_STATUS_CLALIT+1] ; //PREM ,BASE
    int discount; //Discount for basic treatment in Percent
}Clalit;


typedef struct {
    char HealthStatus[POLICY_STATUS_MEUHEDET+1] ; //STANDARD
    int discount; //Discount for basic treatment in Percent
}Meuhedet;

typedef struct
{
    void (*print)(const struct HealthPolicy_*);

    Date startValidDate;
    Date endValidDate;
    eInsuranceType type;

    union {
        Maccabi maccabi;
        Clalit clalit;
        Meuhedet meuhedet;
    }InsuranceType;

}HealthPolicy;

void    initHealthPolicy(HealthPolicy* pInsurance);

void    initMaccabiPolicy(HealthPolicy* pMaccabi);
void    initClalitPolicy(HealthPolicy* pClalit);
void    initMeuhedetPolicy(HealthPolicy* pMeuhedet);
void    getHealthPolicyStatusMaccabi(char* statusInput);
void    getHealthPolicyStatusClalit(char* statusInput);
int     getDiscountPercentage();


const char*	getInsuranceTypeStr(int type);
void        printPolicy(const void* p1);
void		printPolicyNoUnion(const void* p1);
void        printMaccabiPolicy(const HealthPolicy* pMaccabi);
void        printClalitPolicy(const HealthPolicy* pClalit);
void        printMeuhedetPolicy(const HealthPolicy* pMeuhedet);



#endif