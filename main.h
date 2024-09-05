#include <stdio.h>
#include <stdlib.h>
#include "Hospital.h"
#include "HospitalFile.h"
#include "General.h"

#define HOSPITAL_TEXT_FILE_SRC "hospital_src_text.txt"
#define HOSPITAL_BIN_FILE_SRC "hospital_src_binary.bin"

#define HOSPITAL_TEXT_FILE_DST "Hospital_out_text.txt"
#define HOSPITAL_BIN_FILE_DST "Hospital_out_binary.bin"


typedef enum {ePrintHospital, ePrintAllDoctorsInHospital,eAddDepartment, eAddDoctor,eAddTreatment,eAddPatient,eSortPatient,eFindPatient,eReplaceDocSpecialty,eIsHospitalMatchedCorrectly,eNofOptions} eMenuOptions;
const char* str[eNofOptions] = { "Print Hospital","Print All Doctors In Hospital", "Add Department","Add Doctor","Add Treatment","Add Patient","Sort Patient","Find Patient","Replace Doctor's Specialty","Is Hospital Matched Correctly"};

#define EXIT			-1
int menu();
int loadDateMenu();


