#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "MedicalProvider.h"

//when added treatment so needed to provide from company more units
int addUnitAtEquipmentList(int departmentType, LIST* EquipmentList) {
    if (!EquipmentList) return 0;

    addBedUnit(EquipmentList);
    addMedicineUnit(EquipmentList);

    switch (departmentType) {
        case eMaternity:
            addBabyBedUnit(EquipmentList);
            break;
        case eRehabilitation:
            addWheelchairUnit(EquipmentList);
            break;
        case eTrauma:
            addMedicineUnit(EquipmentList);
            break;
        default:
            break;
    }

    return 1;
}

//adds one Bed unit to list
//added to all types of treatment
void addBedUnit(LIST* EquipmentList) {
    Equipment bedEquipment = { eBeds, 0, 0 };
    const NODE* p = L_find(EquipmentList->head.next, (DATA)&bedEquipment, compareEquipmentType);
    if (p != NULL) {
        Equipment* foundBedEquipment = (Equipment*)p->key;
        if (foundBedEquipment->unitsProvided < foundBedEquipment->maximumCapacity) {
            addUnit(foundBedEquipment);
        }
    }
}

//adds one Medicine unit to list
//added to all types of treatment, one extra in Trauma treatment
void addMedicineUnit(LIST* EquipmentList) {
    Equipment medicineEquipment = { eMedicine, 0, 0 };
    const NODE* p = L_find(EquipmentList->head.next, (DATA)&medicineEquipment, compareEquipmentType);
    if (p != NULL) {
        Equipment* foundMedicineEquipment = (Equipment*)p->key;
        if (foundMedicineEquipment->unitsProvided < foundMedicineEquipment->maximumCapacity) {
            addUnit(foundMedicineEquipment);
        }
    }
}

//adds one Baby Bed unit to list
//added in Maternity treatment
void addBabyBedUnit(LIST* EquipmentList) {
    Equipment babyBedEquipment = { eBabyBeds, 0, 0 };
    const NODE* p = L_find(EquipmentList->head.next, (DATA)&babyBedEquipment, compareEquipmentType);
    if (p != NULL) {
        Equipment* foundBabyBedEquipment = (Equipment*)p->key;
        if (foundBabyBedEquipment->unitsProvided < foundBabyBedEquipment->maximumCapacity) {
            addUnit(foundBabyBedEquipment);
        }
    }
}

//adds one Wheelchair unit to list
//added in Rehabilitation treatment
void addWheelchairUnit(LIST* EquipmentList) {
    Equipment wheelchairEquipment = { eWheelchairs, 0, 0 };
    const NODE* p = L_find(EquipmentList->head.next, (DATA)&wheelchairEquipment, compareEquipmentType);
    if (p != NULL) {
        Equipment* foundWheelchairEquipment = (Equipment*)p->key;
        if (foundWheelchairEquipment->unitsProvided < foundWheelchairEquipment->maximumCapacity) {
            addUnit(foundWheelchairEquipment);
        }
    }
}

/* Adds a unit to the specified Equipment in list */
void addUnit(Equipment* pEquipment) {
    if (pEquipment->unitsProvided < pEquipment->maximumCapacity) {
        pEquipment->unitsProvided++;
        printf("additional unit of %s added successfully\n", EquipmentTypeStr[pEquipment->type]);
    } else {
        printf("Cannot add more units for %s. Maximum capacity reached.\n", EquipmentTypeStr[pEquipment->type]);
    }
}
//for using L_find while adding specipic unit
int compareEquipmentType(const void* a1, const void* a2) {
    const Equipment* pEquipment1 = (const Equipment*)a1;
    const Equipment* pEquipment2 = (const Equipment*)a2;

    return pEquipment1->type - pEquipment2->type;
}

void freeEquipment(void* a1) {
    Equipment* pEquipment = (Equipment*)a1;
    free(pEquipment);
}

void printEquipment(const void* a1) {
    const Equipment* pEquipment = (const Equipment*)a1;
    const char* colorCode;

    switch (pEquipment->type) {
        case eMedicine:
            colorCode = ANSI_COLOR_RED;
            break;
        case eBeds:
            colorCode = ANSI_COLOR_GREEN;
            break;
        case eBabyBeds:
            colorCode = ANSI_COLOR_YELLOW;
            break;
        case eWheelchairs:
            colorCode = ANSI_COLOR_BLUE;
            break;
        default:
            colorCode = ANSI_COLOR_RESET;
            break;
    }
    printf("|                                              |\n");
    printf("| Equipment Type: %s%-28s%s |\n", colorCode, EquipmentTypeStr[pEquipment->type], ANSI_COLOR_RESET);
    printf("| Units Provided: %-28d |\n", pEquipment->unitsProvided);
    printf("| Maximum Capacity: %-26d |\n", pEquipment->maximumCapacity);

}

//add new type Equipment to list
void insertEquipmentType(LIST* EquipmentList, Equipment* pType) {
    if (EquipmentList == NULL || pType == NULL)  return;

    Equipment* newEquipment = (Equipment*)malloc(sizeof(Equipment));
    if (newEquipment == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }

    *newEquipment = *pType;
    L_insert(&EquipmentList->head, (DATA)newEquipment);
}


int initMedicalProvider(MedicalProvider* pEquip) {
    if (!L_init(&pEquip->EquipmentList)) return 0;

    Equipment medicineEquipment = { eMedicine, 0, MAX_CAPACITY_MAX };
    Equipment bedsEquipment = { eBeds, 0, MAX_CAPACITY_MAX };
    Equipment babyBedsEquipment = { eBabyBeds, 0, MAX_CAPACITY };
    Equipment wheelchairsEquipment = { eWheelchairs, 0, MAX_CAPACITY };

    insertEquipmentType(&pEquip->EquipmentList, &medicineEquipment);
    insertEquipmentType(&pEquip->EquipmentList, &bedsEquipment);
    insertEquipmentType(&pEquip->EquipmentList, &babyBedsEquipment);
    insertEquipmentType(&pEquip->EquipmentList, &wheelchairsEquipment);
    return 1;
}

void printMedicalProvider(const MedicalProvider* pMedicalProvider) {
    printf("\n%sThe Medical Provider Company: %s", ANSI_BOLD_ON, ANSI_BOLD_OFF);
    printf("\n+----------------------------------------------+");
    L_print(&pMedicalProvider->EquipmentList, printEquipment);
    printf("+----------------------------------------------+\n");
}

void freeMedicalProvider(MedicalProvider* pProvider) {
    L_free(&pProvider->EquipmentList, freeEquipment);
}

