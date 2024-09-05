#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>

#include "Hospital.h"
#include "HospitalFile.h"


int readHospitalFromTextFile(Hospital* pHospital, const char* fileName) {
    FILE *fp = fopen(fileName, "r");
    if (!fp) ERROR_MSG_AND_RETURN("Error open Hospital file\n", 0);

    char temp[MAX_STR_LEN];
    if (!fGets(temp, MAX_STR_LEN, fp))
        return 0;
    pHospital->name = getDynStr(temp);

    if (!readPatientsArrFromTextFile(fp, pHospital)) CLOSE_AND_RETURN(fp, 0);
    if (!readDepartmentsArrFromTextFile(fp, pHospital)) CLOSE_AND_RETURN(fp, 0);
    if (!readMedicalProviderFromTextFile(fp, pHospital)) CLOSE_AND_RETURN(fp, 0);

    CLOSE_AND_RETURN(fp, 1);

}

int readDepartmentsArrFromTextFile(FILE *fp, Hospital *pHospital) {
    int countDepar;
    if (fscanf(fp, "%d\n", &countDepar) != 1) ERROR_MSG_CLOSE_AND_RETURN("Error reading department count\n",fp, 0);

    pHospital->departmentCount = countDepar;
    pHospital->DepartmentsArr = (Department **) malloc(countDepar * sizeof(Department *));
    if (!pHospital->DepartmentsArr) ERROR_MSG_AND_RETURN("Memory allocation failed for DepartmentsArr\n", 0);

    for (int i = 0; i < countDepar; i++) {
        Department *pDepar = calloc(1, sizeof(Department));
        if (!pDepar) {
            printf("Memory allocation failed for Department\n");
            // Free previously allocated memory before returning
            for (int j = 0; j < i; j++) {
                free(pHospital->DepartmentsArr[j]->name);
                free(pHospital->DepartmentsArr[j]);
            }
            free(pHospital->DepartmentsArr);
            return 0;
        }

        if (fscanf(fp, "%s %d\n",pDepar->departmentID, &pDepar->specialtyType) != 2)
            FREE_ERROR_MSG_AND_RETURN(pDepar,"Error reading Department details\n", 0);
        pDepar->name = getSpecialtyTypeStr(pDepar->specialtyType);

        if (!readDoctorArrInDepartmentFromTextFile(fp, pDepar))
            ERROR_MSG_CLOSE_AND_RETURN("Error reading doctors in department", fp, 0);

       
        if (!readTreatmentsArrInDepartmentFromTextFile(fp, pDepar, pHospital->PatientArr, pHospital->PatientCount))
            ERROR_MSG_CLOSE_AND_RETURN("Error reading treatments in department", fp, 0);
        
        pHospital->DepartmentsArr[i] = pDepar;
    }
    return 1;
}

int readTreatmentsArrInDepartmentFromTextFile(FILE* fp, Department* pDepar, Patient** PatientArr, int PatientCount) {
    if (fscanf(fp, "%d\n", &pDepar->treatmentsCount) != 1) ERROR_MSG_AND_RETURN("Error reading treatment count\n", 0);

    pDepar->treatmentsArr = (Treatment *) malloc(pDepar->treatmentsCount * sizeof(Treatment));
    if (!pDepar->treatmentsArr) ERROR_MSG_AND_RETURN("Memory allocation failed for treatmentsArr\n", 0);

    for (int i = 0; i < pDepar->treatmentsCount; i++) {
        Treatment *pTreatment = &pDepar->treatmentsArr[i];

        //read treatmentID,patientID,doctorID
        char patientID[MAX_PER_ID_LEN+1];
        if (fscanf(fp, "%d %s %s\n", &pTreatment->treatmentID, patientID , pTreatment->doctorID) != 3) ERROR_MSG_AND_RETURN("Error reading treatment details\n", 0);
        patientID[MAX_PER_ID_LEN]='\0';

        if (!readDateFromTextFile(fp, &pTreatment->startDate) || !readDateFromTextFile(fp, &pTreatment->endDate)) ERROR_MSG_AND_RETURN("Error reading treatment count\n",0);

        //find the patient with the matching patientID
        pTreatment->treatmentPatient = NULL;
        for (int j = 0; j < PatientCount; j++) {
            if (strcmp(PatientArr[j]->patientID,patientID)==0) {
                pTreatment->treatmentPatient = PatientArr[j];
                break;
            }
        }
        if (pTreatment->treatmentPatient == NULL) ERROR_MSG_AND_RETURN("Patient not found for treatment\n", 0);
    }
    return 1;
}

int readPatientsArrFromTextFile(FILE* fp, Hospital* pHospital) {
    ///PatientSortType
    if (fscanf(fp, "%d", &pHospital->PatientSortType) != 1)ERROR_MSG_CLOSE_AND_RETURN("Error reading Patient Sort Type\n",fp,0);

    int countPatient;
    if (fscanf(fp, "%d\n", &countPatient) != 1) ERROR_MSG_CLOSE_AND_RETURN("Error reading patient count\n",fp,0);

    pHospital->PatientCount = countPatient;
    pHospital->PatientArr = (Patient**)malloc(countPatient * sizeof(Patient*));
    if (!pHospital->PatientArr) ERROR_MSG_AND_RETURN("Memory allocation failed for PatientArr\n",0);


    for (int i = 0; i < countPatient; i++) {
        Patient* pPatient = calloc(1, sizeof(Patient));
        if (!pPatient) {
            printf("Memory allocation failed for Patient\n");
            //free previously allocated memory before returning
            for (int j = 0; j < i; j++) {
                free(pHospital->PatientArr[j]->name);
                free(pHospital->PatientArr[j]);
            }
            free(pHospital->PatientArr);
            return 0;
        }

        if (!readPatientFromTextFile(fp, pPatient)) {
            printf("Error reading Patient details\n");
            free(pPatient);
            //free previously allocated memory before returning
            for (int j = 0; j < i; j++) {
                free(pHospital->PatientArr[j]->name);
                free(pHospital->PatientArr[j]);
            }
            free(pHospital->PatientArr);
            return 0;
        }
        pHospital->PatientArr[i] = pPatient;
    }
    return 1;
}

int readPatientFromTextFile(FILE* fp, Patient* pPatient) {
    char temp[MAX_STR_LEN];
    if (!fGets(temp,MAX_STR_LEN,fp))
        ERROR_MSG_AND_RETURN("Error reading Patient name\n", 0);
    pPatient->name = getDynStr(temp);
    if (!pPatient->name) ERROR_MSG_AND_RETURN("Error reading Patient name\n", 0);

    if (fscanf(fp, "%s", pPatient->patientID) != 1)
        FREE_ERROR_MSG_AND_RETURN(pPatient->name,"Error reading Patient ID\n", 0);

    if (!readHealthPolicyFromTextFile(fp, &pPatient->healthPolicy))
        FREE_ERROR_MSG_AND_RETURN(pPatient->name,"Error reading Patient health policy\n", 0);

    if (!readDateFromTextFile(fp, &pPatient->healthPolicy.startValidDate) ||
        !readDateFromTextFile(fp, &pPatient->healthPolicy.endValidDate))
        FREE_ERROR_MSG_AND_RETURN(pPatient->name,"Error reading Patient valid dates\n", 0)

    return 1;
}

int readHealthPolicyFromTextFile(FILE* fp, HealthPolicy* pHealthPolicy) {
    int insuranceType;
    if (fscanf(fp, "%d", &insuranceType) != 1)
    ERROR_MSG_AND_RETURN("Error reading Patient insurance type\n", 0);
    pHealthPolicy->type = (eInsuranceType)insuranceType;

    switch (pHealthPolicy->type) {
        case eMaccabi:
            if (fscanf(fp, "%s %d", pHealthPolicy->InsuranceType.maccabi.HealthStatus, &pHealthPolicy->InsuranceType.maccabi.discount) != 2)
            ERROR_MSG_AND_RETURN("Error reading Maccabi health status and discount\n", 0);
            pHealthPolicy->print = (void (*)(const struct HealthPolicy_ *))printMaccabiPolicy;
            break;
        case eClalit:
            if (fscanf(fp, "%s %d", pHealthPolicy->InsuranceType.clalit.HealthStatus, &pHealthPolicy->InsuranceType.clalit.discount) != 2)
            ERROR_MSG_AND_RETURN("Error reading Clalit health status and discount\n", 0);
            pHealthPolicy->print = (void (*)(const struct HealthPolicy_ *)) printClalitPolicy;
            break;
        case eMeuhedet:
            if (fscanf(fp, "%s %d", pHealthPolicy->InsuranceType.meuhedet.HealthStatus, &pHealthPolicy->InsuranceType.meuhedet.discount) != 2)
            ERROR_MSG_AND_RETURN("Error reading Meuhedet health status and discount\n", 0);
            pHealthPolicy->print = (void (*)(const struct HealthPolicy_ *)) printMeuhedetPolicy;
            break;
        default:
        ERROR_MSG_AND_RETURN("Invalid insurance type\n", 0);
    }
    return 1;
}


int readMedicalProviderFromTextFile(FILE* fp, Hospital* pHospital) {
    int equipmentCount;
    if (fscanf(fp, "%d\n", &equipmentCount) != 1) ERROR_MSG_AND_RETURN("Error reading equipment count\n", 0);

    L_init(&pHospital->MedicalProvider.EquipmentList);

    for (int i = 0; i < equipmentCount; i++) {
        Equipment* pEquipment = calloc(1, sizeof(Equipment));
        if (!pEquipment) {
            L_free(&pHospital->MedicalProvider.EquipmentList,freeEquipment);
            ERROR_MSG_AND_RETURN("Memory allocation failed for Equipment\n", 0);
        }

        int equipmentType, unitsProvided, maximumCapacity;
        if (fscanf(fp, "%d %d %d\n", &equipmentType, &unitsProvided, &maximumCapacity) != 3) {
            free(pEquipment);
            L_free(&pHospital->MedicalProvider.EquipmentList,freeEquipment);
            ERROR_MSG_AND_RETURN("Error reading Equipment details\n", 0);
        }

        if (equipmentType < 0 || equipmentType >= eNofEquipmentTypes) {
            free(pEquipment);
            L_free(&pHospital->MedicalProvider.EquipmentList,freeEquipment);
            ERROR_MSG_AND_RETURN("Invalid Equipment type\n", 0);
        }

        pEquipment->type = (eEquipmentType)equipmentType;
        pEquipment->unitsProvided = unitsProvided;
        pEquipment->maximumCapacity = maximumCapacity;
        L_insert(&pHospital->MedicalProvider.EquipmentList.head, pEquipment);
    }
    return 1;
}

int readDoctorArrInDepartmentFromTextFile(FILE* fp, Department* pDepar) {
    if (fscanf(fp, "%d\n", &pDepar->doctorCount) != 1) ERROR_MSG_AND_RETURN("Error reading doctor count\n", 0);

    pDepar->DoctorsArr = (Doctor**)malloc(pDepar->doctorCount * sizeof(Doctor*));
    if (!pDepar->DoctorsArr) ERROR_MSG_AND_RETURN("Memory allocation failed for DoctorsArr\n", 0);

    for (int i = 0; i < pDepar->doctorCount; i++) {
        Doctor* pDoctor = calloc(1, sizeof(Doctor));
        if (!pDoctor) {   // free previously allocated memory before returnin
            for (int j = 0; j < i; j++) {
                free(pDepar->DoctorsArr[j]->name);
                free(pDepar->DoctorsArr[j]);
            }
            FREE_ERROR_MSG_AND_RETURN(pDepar->DoctorsArr,"Memory allocation failed for Doctor\n", 0);
        }


        char temp[MAX_STR_LEN];
        if (!fGets(temp,MAX_STR_LEN,fp))
        ERROR_MSG_AND_RETURN("Error reading Doctor name\n", 0);
        pDoctor->name = getDynStr(temp);
        if (!pDoctor->name) ERROR_MSG_AND_RETURN("Error adding Doctor name\n", 0);


        if (fscanf(fp, "%s %d %d\n", pDoctor->doctorID, &pDoctor->specialtyType, &pDoctor->yearExperience) != 3) {
            free(pDoctor->name);
            FREE_ERROR_MSG_AND_RETURN(pDoctor,"Error reading Doctor details\n", 0);
        }
        pDepar->DoctorsArr[i] = pDoctor;
    }
    return 1;
}



int readHospitalFromBFile(Hospital* pHospital, const char* fileName) {
    FILE* fp = fopen(fileName, "rb");
    if (!fp) ERROR_MSG_AND_RETURN("Error opening Hospital binary file\n", 0);

    if (!readHospitalNameFromBFile(fp, pHospital)) CLOSE_AND_RETURN(fp, 0);

    if (fread(&pHospital->PatientSortType, sizeof(sortType), 1, fp) != 1)
        FREE_ERROR_MSG_AND_RETURN(pHospital->name,"Error reading patient sort type\n", 0);

    if (!readPatientsArrFromBFile(fp, pHospital))ERROR_MSG_CLOSE_AND_RETURN("Error reading patients array\n",fp, 0);

    if (!readDepartmentsArrFromBFile(fp, pHospital))ERROR_MSG_CLOSE_AND_RETURN("Error reading departments array\n",fp, 0);

    if (!readMedicalProviderFromBFile(fp, pHospital))ERROR_MSG_CLOSE_AND_RETURN("Error reading Medical Provider array\n",fp, 0);

    CLOSE_AND_RETURN(fp, 1);
}

int readHospitalNameFromBFile(FILE* fp, Hospital* pHospital) {
    int lenName;
    if (fread(&lenName, sizeof(int), 1, fp) != 1)
    ERROR_MSG_AND_RETURN("Error reading hospital name length\n", 0);

    pHospital->name = (char*)malloc(lenName * sizeof(char));
    if (!pHospital->name)
    ERROR_MSG_AND_RETURN("Memory allocation failed for hospital name\n", 0);

    if (fread(pHospital->name, sizeof(char), lenName, fp) != lenName)
        FREE_ERROR_MSG_AND_RETURN(pHospital->name,"Error reading hospital name\n", 0);

    return 1;
}

int readPatientsArrFromBFile(FILE* fp, Hospital* pHospital) {
    if (fread(&pHospital->PatientCount, sizeof(int), 1, fp) != 1)
    ERROR_MSG_AND_RETURN("Error reading patient count\n", 0);

    pHospital->PatientArr = (Patient**)malloc(pHospital->PatientCount * sizeof(Patient*));
    if (!pHospital->PatientArr)
    ERROR_MSG_AND_RETURN("Memory allocation failed for PatientArr\n", 0);

    for (int i = 0; i < pHospital->PatientCount; i++) {
        Patient* pPatient = (Patient*)calloc(1, sizeof(Patient));
        if (!pPatient) {
            for (int j = 0; j < i; j++)
                freePatient(pHospital->PatientArr[j]);
            FREE_ERROR_MSG_AND_RETURN(pHospital->PatientArr,"Memory allocation failed for Patient\n", 0);
        }

        if (!readPatientFromBFile(fp, pPatient)) {
            for (int j = 0; j < i; j++)
                freePatient(pHospital->PatientArr[j]);
            free(pHospital->PatientArr);
            FREE_ERROR_MSG_AND_RETURN(pPatient,"Error reading Patient details\n", 0);
        }
        pHospital->PatientArr[i] = pPatient;
    }

    return 1;
}

int readPatientFromBFile(FILE* fp, Patient* pPatient) {
    int lenName;
    if (fread(&lenName, sizeof(int), 1, fp) != 1)
    ERROR_MSG_AND_RETURN("Error reading patient name length\n", 0);

    pPatient->name = (char*)malloc(lenName * sizeof(char));
    if (!pPatient->name)
    ERROR_MSG_AND_RETURN("Memory allocation failed for patient name\n", 0);

    if (fread(pPatient->name, sizeof(char), lenName, fp) != lenName)
    FREE_ERROR_MSG_AND_RETURN(pPatient->name, "Error reading patient name\n", 0);

    if (fread(pPatient->patientID, sizeof(char), MAX_PER_ID_LEN, fp) != MAX_PER_ID_LEN)
    FREE_ERROR_MSG_AND_RETURN(pPatient->name, "Error reading patient ID\n", 0);

    if (!readHealthPolicyFromBFile(fp, &pPatient->healthPolicy))
    FREE_ERROR_MSG_AND_RETURN(pPatient->name, "Error reading patient health policy\n", 0);

    return 1;
}

int readHealthPolicyFromBFile(FILE* fp, HealthPolicy* pHealthPolicy) {
    if (fread(&pHealthPolicy->type, sizeof(eInsuranceType), 1, fp) != 1)
    ERROR_MSG_AND_RETURN("Error reading insurance type\n", 0);

    switch (pHealthPolicy->type) {
        case eMaccabi:
            if (fread(pHealthPolicy->InsuranceType.maccabi.HealthStatus, sizeof(char), POLICY_STATUS_MACCABI + 1, fp) != POLICY_STATUS_MACCABI + 1)
                ERROR_MSG_AND_RETURN("Error reading Maccabi health status\n", 0);
            if (fread(&pHealthPolicy->InsuranceType.maccabi.discount, sizeof(int), 1, fp) != 1)
                ERROR_MSG_AND_RETURN("Error reading Maccabi discount\n", 0);
            pHealthPolicy->print = (void (*)(const struct HealthPolicy_ *)) printMaccabiPolicy;
            break;
        case eClalit:
            if (fread(pHealthPolicy->InsuranceType.clalit.HealthStatus, sizeof(char), POLICY_STATUS_CLALIT + 1, fp) != POLICY_STATUS_CLALIT + 1)
                ERROR_MSG_AND_RETURN("Error reading Clalit health status\n", 0);
            if (fread(&pHealthPolicy->InsuranceType.clalit.discount, sizeof(int), 1, fp) != 1)
                ERROR_MSG_AND_RETURN("Error reading Clalit discount\n", 0);
            pHealthPolicy->print = (void (*)(const struct HealthPolicy_ *))printClalitPolicy;
            break;
        case eMeuhedet:
            if (fread(pHealthPolicy->InsuranceType.meuhedet.HealthStatus, sizeof(char), POLICY_STATUS_MEUHEDET + 1, fp) != POLICY_STATUS_MEUHEDET + 1)
                ERROR_MSG_AND_RETURN("Error reading Meuhedet health status\n", 0);
            if (fread(&pHealthPolicy->InsuranceType.meuhedet.discount, sizeof(int), 1, fp) != 1)
                ERROR_MSG_AND_RETURN("Error reading Meuhedet discount\n", 0);
            pHealthPolicy->print =(void (*)(const struct HealthPolicy_ *)) printMeuhedetPolicy;
            break;
        default:
        ERROR_MSG_AND_RETURN("Invalid insurance type\n", 0);
    }

    if (!readDateFromBFile(fp, &pHealthPolicy->startValidDate) || !readDateFromBFile(fp, &pHealthPolicy->endValidDate))
    ERROR_MSG_AND_RETURN("Error reading valid dates\n", 0);

    return 1;
}

int readDepartmentsArrFromBFile(FILE* fp, Hospital* pHospital) {
    if (fread(&pHospital->departmentCount, sizeof(int), 1, fp) != 1)
    ERROR_MSG_AND_RETURN("Error reading department count\n", 0);

    pHospital->DepartmentsArr = (Department**)malloc(pHospital->departmentCount * sizeof(Department*));
    if (!pHospital->DepartmentsArr)
    ERROR_MSG_AND_RETURN("Memory allocation failed for DepartmentsArr\n", 0);

    for (int i = 0; i < pHospital->departmentCount; i++) {
        Department* pDepar = (Department*)calloc(1, sizeof(Department));
        if (!pDepar) {
            for (int j = 0; j < i; j++)
                freeDepartment(pHospital->DepartmentsArr[j]);
            free(pHospital->DepartmentsArr);
            ERROR_MSG_AND_RETURN("Memory allocation failed for Department\n", 0);
        }

        if (!readDepartmentFromBFile(fp, pDepar,pHospital)) {
            for (int j = 0; j < i; j++)
                freeDepartment(pHospital->DepartmentsArr[j]);
            free(pHospital->DepartmentsArr);
            freeDepartment(pDepar);
            ERROR_MSG_AND_RETURN("Error reading Department details\n", 0);
        }

        pHospital->DepartmentsArr[i] = pDepar;
    }
    return 1;
}

int readDepartmentFromBFile(FILE* fp, Department* pDepar, Hospital* pHospital) {
    
    if (fread(pDepar->departmentID, sizeof(char), MAX_ID_LEN, fp) != MAX_ID_LEN)
    FREE_ERROR_MSG_AND_RETURN(pDepar->name,"Error reading department ID\n", 0);
    pDepar->departmentID[MAX_ID_LEN] = '\0';  // Null-terminate the string

    if (fread(&pDepar->specialtyType, sizeof(eSpecialty), 1, fp) != 1)
        FREE_ERROR_MSG_AND_RETURN(pDepar->name,"Error reading specialty type\n", 0);

    pDepar->name = getSpecialtyTypeStr(pDepar->specialtyType);

    if (fread(&pDepar->doctorCount, sizeof(int), 1, fp) != 1)
        FREE_ERROR_MSG_AND_RETURN(pDepar->name,"Error reading doctor count\n", 0);

    pDepar->DoctorsArr = (Doctor**)malloc(pDepar->doctorCount * sizeof(Doctor*));
    if (!pDepar->DoctorsArr) FREE_ERROR_MSG_AND_RETURN(pDepar->name,"Memory allocation failed for DoctorsArr\n", 0);

    for (int i = 0; i < pDepar->doctorCount; i++) {
        pDepar->DoctorsArr[i] = (Doctor*)calloc(1, sizeof(Doctor));
        if (!pDepar->DoctorsArr[i]) {
            for (int j = 0; j < i; j++)
                freeDoctor(pDepar->DoctorsArr[j]);
            free(pDepar->DoctorsArr);
            FREE_ERROR_MSG_AND_RETURN(pDepar->name,"Memory allocation failed for Doctor\n", 0);
        }

        if (!readDoctorFromBFile(fp, pDepar->DoctorsArr[i])) {
            for (int j = 0; j < i; j++)
                freeDoctor(pDepar->DoctorsArr[j]);
            free(pDepar->DoctorsArr);
            FREE_ERROR_MSG_AND_RETURN(pDepar->name,"Error reading Doctor details\n", 0);
        }
    }

    if (fread(&pDepar->treatmentsCount, sizeof(int), 1, fp) != 1)
        FREE_ERROR_MSG_AND_RETURN(pDepar,"Error reading treatment count\n", 0);

    pDepar->treatmentsArr = (Treatment*)malloc(pDepar->treatmentsCount * sizeof(Treatment));
    if (!pDepar->treatmentsArr) FREE_ERROR_MSG_AND_RETURN(pDepar,"Memory allocation failed for treatmentsArr\n", 0);


    for (int i = 0; i < pDepar->treatmentsCount; i++) {
        if (!readTreatmentFromBFile(fp, &pDepar->treatmentsArr[i], pHospital->PatientArr, pHospital->PatientCount))
            FREE_ERROR_MSG_AND_RETURN(pDepar,"Error reading Treatment details\n", 0);
    }
    return 1;
}

int readTreatmentFromBFile(FILE* fp, Treatment* pTreatment, Patient** PatientArr, int PatientCount) {
    if (fread(&pTreatment->treatmentID, sizeof(int), 1, fp) != 1)
        ERROR_MSG_AND_RETURN("Error reading treatment ID\n", 0);

    char patientID[MAX_PER_ID_LEN+1];
    if (fread(patientID, sizeof(char), MAX_PER_ID_LEN, fp) != MAX_PER_ID_LEN)
        ERROR_MSG_AND_RETURN("Error reading patient ID\n", 0);
    patientID[MAX_PER_ID_LEN]='\0';

    //Find the patient with the matching patientID
    pTreatment->treatmentPatient = NULL;
    for (int i = 0; i < PatientCount; i++) {
        if (strcmp(PatientArr[i]->patientID,patientID)==0) {
            pTreatment->treatmentPatient = PatientArr[i];
            break;
        }
    }

    if (pTreatment->treatmentPatient == NULL) ERROR_MSG_AND_RETURN("Patient not found for treatment\n", 0);

    if (fread(pTreatment->doctorID, sizeof(char), MAX_PER_ID_LEN, fp) != MAX_PER_ID_LEN)
        ERROR_MSG_AND_RETURN("Error reading doctor ID\n", 0);

    pTreatment->doctorID[MAX_PER_ID_LEN] = '\0';

    if (!readDateFromBFile(fp, &pTreatment->startDate) || !readDateFromBFile(fp, &pTreatment->endDate))
        ERROR_MSG_AND_RETURN("Error reading treatment dates\n", 0);

    return 1;
}

int readDoctorFromBFile(FILE* fp, Doctor* pDoctor) {
    int lenName;
    if (fread(&lenName, sizeof(int), 1, fp) != 1)
        ERROR_MSG_AND_RETURN("Error reading doctor name length\n", 0);

    pDoctor->name = (char*)malloc(lenName * sizeof(char));
    if (!pDoctor->name) ERROR_MSG_AND_RETURN("Memory allocation failed for doctor name\n", 0);

    if (fread(pDoctor->name, sizeof(char), lenName, fp) != lenName) FREE_ERROR_MSG_AND_RETURN(pDoctor->name, "Error reading doctor name\n", 0);

    if (fread(pDoctor->doctorID, sizeof(char), MAX_PER_ID_LEN, fp) != MAX_PER_ID_LEN) FREE_ERROR_MSG_AND_RETURN(pDoctor->name, "Error reading doctor ID\n", 0);

    if (fread(&pDoctor->specialtyType, sizeof(eSpecialty), 1, fp) != 1) FREE_ERROR_MSG_AND_RETURN(pDoctor->name, "Error reading doctor specialty type\n", 0);

    if (fread(&pDoctor->yearExperience, sizeof(int), 1, fp) != 1) FREE_ERROR_MSG_AND_RETURN(pDoctor->name, "Error reading doctor year experience\n", 0);

    return 1;
}

int readMedicalProviderFromBFile(FILE* fp, Hospital* pHospital) {
    int equipmentCount;
    if (fread(&equipmentCount, sizeof(int), 1, fp) != 1) ERROR_MSG_AND_RETURN("Error reading equipment count\n", 0);

    L_init(&pHospital->MedicalProvider.EquipmentList);

    for (int i = 0; i < equipmentCount; i++) {
        Equipment* pEquipment = (Equipment*)calloc(1, sizeof(Equipment));
        if (!pEquipment) {
            L_free(&pHospital->MedicalProvider.EquipmentList, freeEquipment);
            ERROR_MSG_AND_RETURN("Memory allocation failed for Equipment\n", 0);
        }

        if (!readEquipmentFromBFile(fp, pEquipment)) {
            L_free(&pHospital->MedicalProvider.EquipmentList, freeEquipment);
            freeEquipment(pEquipment);
            ERROR_MSG_AND_RETURN("Error reading Equipment details\n", 0);
        }
        L_insert(&pHospital->MedicalProvider.EquipmentList.head, pEquipment);
    }

    return 1;
}

int readEquipmentFromBFile(FILE* fp, Equipment* pEquipment) {
    BYTE data[3];
    if (fread(data, sizeof(BYTE), 3, fp) != 3) {
        ERROR_MSG_AND_RETURN("Error reading Equipment data\n", 0);
    }

    // Extract equipment type from the first 3 bits of the first byte
    pEquipment->type = (eEquipmentType)(data[0] & 0x07);

    // Extract maximum capacity from the last 5 bits of the first byte and the first 3 bits of the second byte
    pEquipment->maximumCapacity = (data[0] >> 3) & 0x1F;
    pEquipment->maximumCapacity |= (data[1] & 0x07) << 5;

    // Extract units provided from the last 5 bits of the second byte and the third byte
    pEquipment->unitsProvided = (data[1] >> 3) & 0x1F;
    pEquipment->unitsProvided |= (data[2] << 5);

    return 1;
}

int saveHospitalToBinaryFile(Hospital* pHospital, const char* FileName) {
    FILE* fp;
    fp = fopen(FileName, "wb");
    if (!fp) ERROR_MSG_CLOSE_AND_RETURN("Error opening hospital file to write\n", fp, 0);

    //Write Hospital name length
    int lenName = (int)strlen(pHospital->name) + 1;
    if (fwrite(&lenName, sizeof(int), 1, fp) != 1)
    ERROR_MSG_CLOSE_AND_RETURN("Error writing hospital name length to binary file\n", fp, 0);

    //Write Hospital name
    if (fwrite(pHospital->name, sizeof(char), lenName, fp) != lenName)
    ERROR_MSG_CLOSE_AND_RETURN("Error writing hospital name to binary file\n", fp, 0);

    //Write patient sort type
    if (fwrite(&pHospital->PatientSortType, sizeof(int), 1, fp) != 1)
    ERROR_MSG_CLOSE_AND_RETURN("Error writing patient sort type to binary file\n", fp, 0);

    //Write number of patients
    if (fwrite(&pHospital->PatientCount, sizeof(int), 1, fp) != 1)
    ERROR_MSG_CLOSE_AND_RETURN("Error writing patient count to binary file\n", fp, 0);

    //Write each patient
    for (int i = 0; i < pHospital->PatientCount; ++i) {
        if (!savePatientToBFile(fp, pHospital->PatientArr[i])) {
            printf("Error writing patient %d to binary file\n", i+1);
            CLOSE_AND_RETURN(fp,0);
        }
    }

    //Write number of departments
    if (fwrite(&pHospital->departmentCount, sizeof(int), 1, fp) != 1)
    ERROR_MSG_CLOSE_AND_RETURN("Error writing department count to binary file\n", fp, 0);

    //Write each department
    for (int i = 0; i < pHospital->departmentCount; ++i) {
        if (!saveDepartmentToBFile(fp, pHospital->DepartmentsArr[i])) {
            printf("Error writing department %d to binary file\n", i+1);
            CLOSE_AND_RETURN(fp,0);
        }
    }

    //Write medical provider
    if (!saveMedicalProviderToBFile(fp, &pHospital->MedicalProvider))
    ERROR_MSG_CLOSE_AND_RETURN("Error writing medical provider to binary file\n", fp, 0);

    CLOSE_AND_RETURN(fp,1);
}

int savePatientToBFile(FILE* fp, Patient* pPatient) {
    // Write patient name length
    int lenName = (int)strlen(pPatient->name) + 1;
    if (fwrite(&lenName, sizeof(int), 1, fp) != 1)
    ERROR_MSG_AND_RETURN("Error writing patient name length to binary file\n", 0);

    // Write patient name
    if (fwrite(pPatient->name, sizeof(char), lenName, fp) != lenName)
    ERROR_MSG_AND_RETURN("Error writing patient name to binary file\n", 0);

    // Write patient ID
    if (fwrite(pPatient->patientID, sizeof(char), MAX_PER_ID_LEN, fp) != MAX_PER_ID_LEN)
    ERROR_MSG_AND_RETURN("Error writing patient ID to binary file\n", 0);

    // Write health policy
    if (!saveHealthPolicyToBFile(fp, &pPatient->healthPolicy))
    ERROR_MSG_AND_RETURN("Error writing health policy to binary file\n", 0);

    return 1;
}

int saveHealthPolicyToBFile(FILE* fp, HealthPolicy* pHealthPolicy) {
    // Check if the insurance type is valid
    if (pHealthPolicy->type < 0 || pHealthPolicy->type >= eNofInsuranceTypes)
    ERROR_MSG_AND_RETURN("Invalid insurance type\n", 0);

    // Write insurance type
    if (fwrite(&pHealthPolicy->type, sizeof(eInsuranceType), 1, fp) != 1)
    ERROR_MSG_AND_RETURN("Error writing insurance type to binary file\n", 0);

    switch (pHealthPolicy->type) {
        case eMaccabi:
            if (fwrite(pHealthPolicy->InsuranceType.maccabi.HealthStatus, sizeof(char), POLICY_STATUS_MACCABI + 1, fp) != POLICY_STATUS_MACCABI + 1)
                ERROR_MSG_AND_RETURN("Error writing Maccabi health status to binary file\n", 0);
            if (fwrite(&pHealthPolicy->InsuranceType.maccabi.discount, sizeof(int), 1, fp) != 1)
                ERROR_MSG_AND_RETURN("Error writing Maccabi discount to binary file\n", 0);
            break;
        case eClalit:
            if (fwrite(pHealthPolicy->InsuranceType.clalit.HealthStatus, sizeof(char), POLICY_STATUS_CLALIT + 1, fp) != POLICY_STATUS_CLALIT + 1)
                ERROR_MSG_AND_RETURN("Error writing Clalit health status to binary file\n", 0);
            if (fwrite(&pHealthPolicy->InsuranceType.clalit.discount, sizeof(int), 1, fp) != 1)
                ERROR_MSG_AND_RETURN("Error writing Clalit discount to binary file\n", 0);
            break;
        case eMeuhedet:
            if (fwrite(pHealthPolicy->InsuranceType.meuhedet.HealthStatus, sizeof(char), POLICY_STATUS_MEUHEDET + 1, fp) != POLICY_STATUS_MEUHEDET + 1)
                ERROR_MSG_AND_RETURN("Error writing Meuhedet health status to binary file\n", 0);
            if (fwrite(&pHealthPolicy->InsuranceType.meuhedet.discount, sizeof(int), 1, fp) != 1)
                ERROR_MSG_AND_RETURN("Error writing Meuhedet discount to binary file\n", 0);
            break;
        default:
        ERROR_MSG_AND_RETURN("Invalid insurance type\n", 0);
    }

    // Write start date and end date
    if (!saveDateToBFile(fp, &pHealthPolicy->startValidDate)) ERROR_MSG_AND_RETURN("Error writing start date to binary file\n", 0);
    if (!saveDateToBFile(fp, &pHealthPolicy->endValidDate)) ERROR_MSG_AND_RETURN("Error writing end date to binary file\n", 0);

    return 1;
}


int saveDepartmentToBFile(FILE* fp,Department* pDepartment) {
   
    // Write department ID and specialty type
    if (fwrite(pDepartment->departmentID, sizeof(char), MAX_ID_LEN, fp) != MAX_ID_LEN ||
        fwrite(&pDepartment->specialtyType, sizeof(eSpecialty), 1, fp) != 1)
        return 0;

    // Write doctor count and each doctor
    if (fwrite(&pDepartment->doctorCount, sizeof(int), 1, fp) != 1)
        return 0;

    for (int i = 0; i < pDepartment->doctorCount; i++)
        if (!saveDoctorToBFile(fp, pDepartment->DoctorsArr[i]))
            return 0;

    // Write treatment count and each treatment
    if (fwrite(&pDepartment->treatmentsCount, sizeof(int), 1, fp) != 1)
        return 0;

    for (int i = 0; i < pDepartment->treatmentsCount; i++)
        if (!saveTreatmentToBFile(fp,&pDepartment->treatmentsArr[i]))
            return 0;

    return 1;
}

int saveMedicalProviderToBFile(FILE* fp, MedicalProvider* pMedicalProvider) {
    // Write equipment count
    int equipmentCount = L_length(&pMedicalProvider->EquipmentList);
    if (fwrite(&equipmentCount, sizeof(int), 1, fp) != 1)
        return 0;

    // Write each equipment
    NODE* pNode = pMedicalProvider->EquipmentList.head.next;
    while (pNode != NULL) {
        Equipment* pEquipment = (Equipment*)pNode->key;
        if (!saveEquipmentToBFile(fp, pEquipment))
            return 0;
        pNode = pNode->next;
    }
    return 1;
}

int saveEquipmentToBFile(FILE* fp, Equipment* pEquipment) {
    BYTE data[3];

    // Store equipment type in the first 3 bits of the first byte
    data[0] = (BYTE)(pEquipment->type & 0x07);

    // Store maximum capacity in the last 5 bits of the first byte and the first 3 bits of the second byte
    data[0] |= (BYTE)((pEquipment->maximumCapacity & 0x1F) << 3);
    data[1] = (BYTE)((pEquipment->maximumCapacity >> 5) & 0x07);

    // Store units provided in the last 5 bits of the second byte and the third byte
    data[1] |= (BYTE)((pEquipment->unitsProvided & 0x1F) << 3);
    data[2] = (BYTE)(pEquipment->unitsProvided >> 5);

    if (fwrite(data, sizeof(BYTE), 3, fp) != 3)
        return 0;

    return 1;
}

int saveDoctorToBFile(FILE* fp, Doctor* pDoctor) {
    // Write doctor name length
    int lenName = (int)strlen(pDoctor->name) + 1;
    if (fwrite(&lenName, sizeof(int), 1, fp) != 1)
        return 0;

    // Write doctor name
    if (fwrite(pDoctor->name, sizeof(char), lenName, fp) != lenName)
        return 0;

    // Write doctor ID, specialty type, and year experience
    if (fwrite(pDoctor->doctorID, sizeof(char), MAX_PER_ID_LEN, fp) != MAX_PER_ID_LEN ||
        fwrite(&pDoctor->specialtyType, sizeof(eSpecialty), 1, fp) != 1 ||
        fwrite(&pDoctor->yearExperience, sizeof(int), 1, fp) != 1) {
        return 0;
    }
    return 1;
}

int saveTreatmentToBFile(FILE* fp, Treatment* pTreatment) {
    // Write treatment ID
    if (fwrite(&pTreatment->treatmentID, sizeof(int), 1, fp) != 1)
        return 0;

    // Write patient ID
    if (fwrite(pTreatment->treatmentPatient->patientID, sizeof(char), MAX_PER_ID_LEN, fp) != MAX_PER_ID_LEN)
        return 0;

    // Write doctor ID
    if (fwrite(pTreatment->doctorID, sizeof(char), MAX_PER_ID_LEN, fp) != MAX_PER_ID_LEN)
        return 0;

    // Write start date and end date
    if (!saveDateToBFile(fp, &pTreatment->startDate) ||!saveDateToBFile(fp, &pTreatment->endDate))
    ERROR_MSG_AND_RETURN("Error writing Dates to binary file\n",0);

    return 1;
}


int saveHospitalToTextFile(Hospital* pHospital, const char* FileName) {
    FILE* fp;
    fp = fopen(FileName, "w");
    if (!fp) ERROR_MSG_AND_RETURN("Error opening hospital file to write\n", 0);

    if(!fprintf(fp, "%s\n%d\n", pHospital->name, pHospital->PatientSortType))
        ERROR_MSG_CLOSE_AND_RETURN("Error saving hospital name and sort type of patients",fp, 0);

    if (!savePatientArrToTextFile(fp, pHospital->PatientArr, pHospital->PatientCount))
        ERROR_MSG_CLOSE_AND_RETURN("Error saving patients arr",fp, 0);

    if (!saveDepartmentArrToTextFile(fp, pHospital->DepartmentsArr, pHospital->departmentCount))
        ERROR_MSG_CLOSE_AND_RETURN("Error saving departments arr",fp, 0);

    if (!saveMedicalProviderToTextFile(fp, &pHospital->MedicalProvider))
        ERROR_MSG_CLOSE_AND_RETURN("Error saving Medical Provider",fp, 0);

    CLOSE_AND_RETURN(fp, 1);
}

int savePatientArrToTextFile(FILE* fp, Patient** PatientArr, int PatientCount){
    return generalSaveArrayToTextFile(fp, PatientArr, PatientCount, sizeof(Patient *),
                                      (int (*)(FILE *, void *)) savePatientToTextFile);
}

int saveDepartmentArrToTextFile(FILE* fp, Department** DepartmentArr, int DepartmentCount){
    return generalSaveArrayToTextFile(fp, DepartmentArr, DepartmentCount, sizeof(Department *),
                                      (int (*)(FILE *, void *)) saveDepartmentToTextFile);
}

int savePatientToTextFile(FILE* fp, void* pPatient) {
    const Patient* temp = *(Patient**)pPatient;

    if (!fprintf(fp, "%s\n%s ", temp->name,temp->patientID))
        ERROR_MSG_AND_RETURN("Error saving patient name and ID to text file\n", 0);

    if (!saveHealthPolicyToTextFile(fp, &temp->healthPolicy))
        ERROR_MSG_AND_RETURN("Error saving health policy to text file\n", 0);

    fprintf(fp, "\n");
    return 1;
}

int saveHealthPolicyToTextFile(FILE* fp, const HealthPolicy* pHealthPolicy) {
    if (!fprintf(fp, "%d ", pHealthPolicy->type))
    ERROR_MSG_AND_RETURN("Error saving Insurance Type to text file\n", 0);

    switch (pHealthPolicy->type) {
        case eMaccabi:
            if (!fprintf(fp, "%s %d\n", pHealthPolicy->InsuranceType.maccabi.HealthStatus, pHealthPolicy->InsuranceType.maccabi.discount))
            ERROR_MSG_AND_RETURN("Error saving Maccabi policy to text file\n", 0);
            break;
        case eClalit:
            if (!fprintf(fp, "%s %d\n", pHealthPolicy->InsuranceType.clalit.HealthStatus, pHealthPolicy->InsuranceType.clalit.discount))
            ERROR_MSG_AND_RETURN("Error saving Clalit policy to text file\n", 0);
            break;
        case eMeuhedet:
            if (!fprintf(fp, "%s %d\n", pHealthPolicy->InsuranceType.meuhedet.HealthStatus, pHealthPolicy->InsuranceType.meuhedet.discount))
            ERROR_MSG_AND_RETURN("Error saving Meuhedet policy to text file\n", 0);
            break;
        default:
        ERROR_MSG_AND_RETURN("Invalid insurance type\n", 0);
    }

    if (!fprintf(fp, "%d/%d/%d\n%d/%d/%d",
                 pHealthPolicy->startValidDate.day, pHealthPolicy->startValidDate.month, pHealthPolicy->startValidDate.year,
                 pHealthPolicy->endValidDate.day, pHealthPolicy->endValidDate.month, pHealthPolicy->endValidDate.year))
        return 0;

    return 1;
}

int saveDepartmentToTextFile(FILE* fp, void* pDepartment) {
    const Department* temp = *(Department**)pDepartment;

    fprintf(fp, "%s\n%d\n",temp->departmentID, temp->specialtyType);

    if (!saveDoctorArrToTextFile(fp, temp->DoctorsArr, temp->doctorCount))
        ERROR_MSG_AND_RETURN("Error saving doctor to text file\n", 0);

    if (!saveTreatmentArrToTextFile(fp, temp->treatmentsArr, temp->treatmentsCount))
        ERROR_MSG_AND_RETURN("Error saving treatment to text file\n", 0);

    return 1;
}

int saveDoctorArrToTextFile(FILE* fp, Doctor** DoctorArr, int DoctorCount){
    return generalSaveArrayToTextFile(fp, DoctorArr, DoctorCount, sizeof(Doctor *),
                                      (int (*)(FILE *, void *)) saveDoctorToTextFile);
}

int saveDoctorToTextFile(FILE* fp, void* pDoctor) {
    const Doctor* temp = *(Doctor**)pDoctor;
    return (fprintf(fp, "%s\n%s %d %d\n", temp->name, temp->doctorID, temp->specialtyType, temp->yearExperience));
}

int saveTreatmentArrToTextFile(FILE* fp, Treatment* TreatmentArr, int TreatmentCount){
    return generalSaveArrayToTextFile(fp, TreatmentArr, TreatmentCount, sizeof(Treatment),
                                      (int (*)(FILE *, void *)) saveTreatmentToTextFile);
}

int saveTreatmentToTextFile(FILE* fp, void* pTreatment) {
    const Treatment* temp = (Treatment*)pTreatment;
    return (fprintf(fp, "%d %s %s\n%d/%d/%d\n%d/%d/%d\n",temp->treatmentID, temp->treatmentPatient->patientID, temp->doctorID,
                    temp->startDate.day, temp->startDate.month, temp->startDate.year,
                    temp->endDate.day, temp->endDate.month, temp->endDate.year));
}

int saveMedicalProviderToTextFile(FILE* fp, MedicalProvider* pMedicalProvider) {
    if(!fprintf(fp, "%d\n", L_length(&pMedicalProvider->EquipmentList)))
        return 0;
    NODE* pNode = pMedicalProvider->EquipmentList.head.next;
    while (pNode != NULL) {
        Equipment* pEquipment = (Equipment*)pNode->key;
        if(!fprintf(fp, "%d %d %d\n",pEquipment->type, pEquipment->unitsProvided, pEquipment->maximumCapacity))
            return 0;
        pNode = pNode->next;
    }
    return 1;
}

