#ifndef HOSPITAL_FILE
#define HOSPITAL_FILE

#include <stdio.h>
#include "Hospital.h"
#include "HealthPolicy.h"

#define CLOSE_AND_RETURN(fp,num) { fclose(fp); return num; }
#define ERROR_MSG_CLOSE_AND_RETURN(msg,fp,num) {printf(msg); CLOSE_AND_RETURN(fp,num) }


int readHospitalFromTextFile(Hospital* pHospital, const char* fileName);
int readDepartmentsArrFromTextFile(FILE* fp, Hospital* pHospital);
int readTreatmentsArrInDepartmentFromTextFile(FILE* fp, Department* pDepar, Patient** PatientArr, int PatientCount);
int readPatientsArrFromTextFile(FILE* fp, Hospital* pHospital);
int readPatientFromTextFile(FILE* fp, Patient* pPatient);
int readHealthPolicyFromTextFile(FILE* fp, HealthPolicy* pHealthPolicy);
int readMedicalProviderFromTextFile(FILE* fp, Hospital* pHospital);
int readDoctorArrInDepartmentFromTextFile(FILE* fp, Department* pDepar);

int readHospitalFromBFile(Hospital* pHospital, const char* fileName);
int readHospitalNameFromBFile( FILE* fp, Hospital* pHospital);
int readDepartmentsArrFromBFile(FILE* fp, Hospital* pHospital);
int readPatientsArrFromBFile(FILE* fp, Hospital* pHospital);
int readPatientFromBFile(FILE* fp, Patient* pPatient);
int readHealthPolicyFromBFile(FILE* fp, HealthPolicy* pHealthPolicy);
int readDepartmentFromBFile(FILE* fp, Department* pDepar, Hospital* pHospital);
int readTreatmentFromBFile(FILE* fp, Treatment* pTreatment, Patient** PatientArr, int PatientCount);
int readDoctorFromBFile(FILE* fp, Doctor* pDoctor);
int readMedicalProviderFromBFile(FILE* fp, Hospital* pHospital);
int readEquipmentFromBFile(FILE* fp, Equipment* pEquipment);

int saveHospitalToBinaryFile(Hospital* pHospital, const char* FileName);
int savePatientToBFile(FILE* fp, Patient* pPatient);
int saveHealthPolicyToBFile(FILE* fp, HealthPolicy* pHealthPolicy);
int saveDepartmentToBFile(FILE* fp,Department* pDepartment);
int saveMedicalProviderToBFile(FILE* fp, MedicalProvider* pMedicalProvider);
int saveEquipmentToBFile(FILE* fp, Equipment* pEquipment);
int saveDoctorToBFile(FILE* fp, Doctor* pDoctor);
int saveTreatmentToBFile(FILE* fp, Treatment* pTreatment);

int saveHospitalToTextFile(Hospital* pHospital, const char* FileName);
int saveHealthPolicyToTextFile(FILE* fp, const HealthPolicy* pHealthPolicy);
int saveMedicalProviderToTextFile(FILE* fp, MedicalProvider* pMedicalProvider);
int savePatientArrToTextFile(FILE* fp, Patient** PatientArr, int PatientCount);
int savePatientToTextFile(FILE* fp, void* pPatient);
int saveDepartmentArrToTextFile(FILE* fp, Department** DepartmentArr, int DepartmentCount);
int saveDepartmentToTextFile(FILE* fp, void* pDepartment);
int saveDoctorArrToTextFile(FILE* fp, Doctor** DoctorArr, int DoctorCount);
int saveDoctorToTextFile(FILE* fp, void* pDoctor);
int saveTreatmentArrToTextFile(FILE* fp, Treatment* DoctorArr, int TreatmentCount);
int saveTreatmentToTextFile(FILE* fp, void* pTreatment);


#endif
