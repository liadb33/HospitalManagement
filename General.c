#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "General.h"


char* getStrExactName(const char* msg)
{
	char* str;
	char temp[MAX_STR_LEN];
	printf("%s\t", msg);
    fGets(temp, MAX_STR_LEN, stdin);

	str = getDynStr(temp);
	return str;
}

char* getDynStr(char* str)
{
	char* theStr;
	theStr = (char*)malloc((strlen(str) + 1) * sizeof(char));
	if (!theStr)
		return NULL;

	strcpy(theStr, str);
	return theStr;
}

char* fGets(char* buffer, int size, FILE* stream)
{
	char* ok;
	if (buffer != NULL && size > 0)
	{
		do { //skip only '\n' strings
			ok = fgets(buffer, size, stream);
		} while (ok && ((strlen(buffer) <= 1) && (isspace(buffer[0]))));
		if (ok)
		{
            char* back = buffer + strlen(buffer);
            //trim end spaces
			while ((buffer < back) && (isspace(*--back)));
			*(back + 1) = '\0';
			return buffer;
		}
		buffer[0] = '\0';
	}
	return NULL;
}

char** splitCharsToWords(char* str, int* pCount, int* pTotalLength)
{
	char temp[MAX_STR_LEN];
	char* delimiters = " ";
	char* word;
	int count = 0;

	strcpy(temp, str);
	char** wordsArray = NULL;
	*pTotalLength = 0;

	word = strtok(temp, delimiters);
	while (word != NULL)
	{
		wordsArray = (char**)realloc(wordsArray, (count + 1) * sizeof(char*));
		if (!wordsArray)
			return 0;
		wordsArray[count] = getDynStr(word);
		count++;
		*pTotalLength += (int)strlen(word);
		word = strtok(NULL, delimiters);
	}
	*pCount = count;
	return wordsArray;
}

int generalSaveArrayToTextFile(FILE* fp, void* arr, int size, size_t elementSize, int (*saveElement)(FILE*, void*)) {
    fprintf(fp, "%d\n", size);
    for (int i = 0; i < size; i++) {
        void* element = (char*)arr + i * elementSize;
        if (!saveElement(fp, element)) {
            printf("Error saving element %d to text file\n", i + 1);
            return 0;
        }
    }
    return 1;
}


const char* getSpecialtyTypeStr(const int type)
{
    if (type < 0 || type >= eNofSpecialtyTypes)
        return NULL;
    return SpecialtyTypeStr[type];
}

int fromNameToSpecialtyType(char* name){//from specially name you get the eSpecialty
    for(int i = 1; i < eNofSpecialtyTypes; i++){
        if(strcmp(name, getSpecialtyTypeStr(i)) == 0)
            return i;
    }
    return -1;
}

//check 9 digit, and check correct Check Digit ID
void getCorrectID(char* IDBuffer) {
    char ID[MAX_STR_LEN];
    int tempID = 0;
    int isValid;
    printf("Enter the 9-digit ID (including the Check Digit): \n");

    do {
        isValid = 1;
        scanf("%s", ID);
        if (strlen(ID) != MAX_PER_ID_LEN) {
            printf("Invalid input. Please enter exactly 9 digits.\n");
            isValid = 0;
        }
        else {
            // Check if the input contains only digits
            for (int i = 0; i < MAX_PER_ID_LEN; i++) {
                if (!isdigit(ID[i])) {
                    printf("Invalid input. Please enter only digits.\n");
                    isValid = 0;
                    break;
                }
            }

            if (isValid) {
                // Check if the 9th digit is the correct check digit
                sscanf(ID, "%d", &tempID);
                int checkDigit = calculateCheckDigit(tempID / 10);
                if (checkDigit != (tempID % 10)) {
                    printf("Incorrect ID. The check digit is invalid.\n");
                    isValid = 0;
                }
            }
        }
    } while (!isValid);
    
    memcpy(IDBuffer, ID, MAX_PER_ID_LEN);
    IDBuffer[MAX_PER_ID_LEN] = '\0';
}


int calculateCheckDigit(int id){
    int sum = 0;
    int weight = 2;
    while (id > 0)
    {
        int digit = id % 10;
        id /= 10;
        int product = digit * weight;
        sum += (product > 9) ? product - 9 : product;
        weight = (weight == 2) ? 1 : 2;
    }
    int checkDigit = (10 - (sum % 10)) % 10;
    return checkDigit;
}

void capitalizeFirstLetter(char* str) {
    //return with capital first letter and others in lowercase
    if (str && strlen(str) > 0) {
        str[0] = toupper(str[0]);

        //convert the remaining characters to lowercase
        for (int i = 1; i < strlen(str); i++)
            str[i] = tolower(str[i]);
    }
}

void capitalizeAllLetters(char* str) {
    if (str == NULL) {
        return;
    }

    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = toupper(str[i]);
    }
}

void printWelcome() {
    printf(" ____    __    ____  _______  __        ______   ______   .___  ___.  _______ \n");
    printf(" \\   \\  /  \\  /   / |   ____||  |      /      | /  __  \\  |   \\/   | |   ____|\n");
    printf("  \\   \\/    \\/   /  |  |__   |  |     |  ,----'|  |  |  | |  \\  /  | |  |__   \n");
    printf("   \\            /   |   __|  |  |     |  |     |  |  |  | |  |\\/|  | |   __|  \n");
    printf("    \\    /\\    /    |  |____ |  `----.|  `----.|  `--'  | |  |  |  | |  |____ \n");
    printf("     \\__/  \\__/     |_______||_______| \\______| \\______/  |__|  |__| |_______|\n");
    printf("                                                                               \n");
}

void printGoodLuck() {
    printf("  _____                 _   _                _    _   _ \n");
    printf(" / ____|               | | | |              | |  | | | |\n");
    printf("| |  __  ___   ___   __| | | |    _   _  ___| | _| |_| |\n");
    printf("| | |_ |/ _ \\ / _ \\ / _` | | |   | | | |/ __| |/ / __| |\n");
    printf("| |__| | (_) | (_) | (_| | | |___| |_| | (__|   <| |_|_|\n");
    printf(" \\_____|\\___/ \\___/ \\__,_| |______\\__,_|\\___|_|\\_\\\\__(_)\n");
}