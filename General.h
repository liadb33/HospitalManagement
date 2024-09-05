#ifndef __GENERAL__
#define __GENERAL__

#define MAX_STR_LEN 255
#define MAX_PATIENTS 10
#define MAX_DEPARTMENTS eNofSpecialtyTypes


#define POLICY_STATUS_MACCABI 3 //VIP,REG
#define POLICY_STATUS_CLALIT 4 //PREM ,BASE
#define POLICY_STATUS_MEUHEDET 8 //STANDARD

#define MAX_ID_LEN 4
#define MAX_PER_ID_LEN 9
#define MAX_CAPACITY 200
#define MAX_CAPACITY_MAX 250

#define ERROR_MSG_AND_RETURN(msg,num) {printf("%s",msg); return num; }
#define ERROR_MSG_AND_RETURN_VOID(msg) {printf("%s",msg); return; }
#define FREE_ERROR_MSG_AND_RETURN(pointer,msg,num) {free(pointer); printf("%s",msg); return num; }


#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_BOLD_ON    "\x1b[1m"
#define ANSI_BOLD_OFF   "\x1b[0m"

#include "list.h"

//for eSpecialty in Department and doctors
typedef enum { eNone, eMaternity, eRehabilitation, eTrauma, eCardiology ,eRadiology, eOncology,eNeurology,ePsychiatry,eUrology ,eEmergency,eNofSpecialtyTypes } eSpecialty;
static const char* SpecialtyTypeStr[eNofSpecialtyTypes] = { "None","Maternity", "Rehabilitation", "Trauma","Cardiology","Radiology","Oncology","Neurology","Psychiatry","Urology","Emergency"};
 
const char*	getSpecialtyTypeStr(int type);
int     fromNameToSpecialtyType(char* name);

int     generalSaveArrayToTextFile(FILE* fp, void* arr, int size, size_t elementSize, int (*saveElement)(FILE*, void*));


void	getCorrectID(char* IDBuffer);
int		calculateCheckDigit(int id);
void    capitalizeFirstLetter(char* str);
void    capitalizeAllLetters(char* str);
char*	getStrExactName(const char* msg);
char*   fGets(char* buffer, int size, FILE* stream);
char*	getDynStr(char* str);
void    printWelcome();
void    printGoodLuck();


#endif

