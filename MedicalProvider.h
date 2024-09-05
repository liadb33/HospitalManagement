#ifndef __MED_PROVIDER__
#define __MED_PROVIDER__

#include "General.h"

typedef enum { eMedicine, eBeds, eBabyBeds, eWheelchairs, eNofEquipmentTypes } eEquipmentType;
static const char* EquipmentTypeStr[eNofEquipmentTypes] = { "Medicine", "Bed","BabyBed", "Wheelchair" };

typedef unsigned char BYTE;

typedef struct
{
    eEquipmentType type;
    unsigned unitsProvided;
    unsigned maximumCapacity;
}Equipment;


void	printEquipment(const void* a1);
void	freeEquipment(void* a1);



typedef struct
{
    LIST EquipmentList;
}MedicalProvider;


void    insertEquipmentType(LIST* EquipmentList, Equipment* pType);
int     addUnitAtEquipmentList(int departmentType, LIST* EquipmentList);
void    addBedUnit(LIST* EquipmentList);
void    addMedicineUnit(LIST* EquipmentList);
void    addBabyBedUnit(LIST* EquipmentList);
void    addWheelchairUnit(LIST* EquipmentList);
void    addUnit(Equipment* pType);
int     compareEquipmentType(const void* a1, const void* a2);

int    initMedicalProvider(MedicalProvider* pEquip);
void    printMedicalProvider(const MedicalProvider* pMedicalProvider);
void	freeMedicalProvider(MedicalProvider* pProvider);



#endif