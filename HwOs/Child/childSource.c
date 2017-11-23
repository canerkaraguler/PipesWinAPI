#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#define BUFSIZE 4096
#define threadCount  4
#pragma warning(disable:4996)
typedef struct
{
	int itemId;
	int itemType;
	int threadNo;
	
	char *day;

	
}THREAD_PARAMETERS;
void CreateAllThreads();
void ManageStdHandles();
void CreateThreadsWithDayInfo();

DWORD WINAPI threadWork(LPVOID parameters);
CHAR itemNames[4][100] = { { " MILK" },{ " BISCUIT" },{ " CHIPS" },{ " COKE" } };
CHAR chBuf[BUFSIZE];
HANDLE* handles;
THREAD_PARAMETERS* lpParameter;
HANDLE hStdin, hStdout;
int* threadID;

int milkCounter = 0;
int biscuitCounter = 0;
int chipsCounter = 0;
int cokeCounter = 0;


int dayIndex = 0;
char days[7][14] = { { "#START DAY 1#" },{ "#START DAY 2#" },{ "#START DAY 3#" },{ "#START DAY 4#" },{ "#START DAY 5#" },{ "#START DAY 6#" },{ "#START DAY 7#" } };
char endDays[8][30] = { { "#END DAY 1#" },{ "#END DAY 2#" },{ "#END DAY 3#" },{ "#END DAY 4#" },{ "#END DAY 5#" },{ "#END DAY 6#" },{ "#END DAY 7#" },{ "#END DAY 7" } };

int main()
{

	
	
	
	ManageStdHandles();
	

	
	//getting the day info as a std input;
	int i = 0;
	for (i = 0; (chBuf[i] = getchar()) != 0; i++)
	{
	}
	
	//defining the day index .
	for (i = 0; i < 7; i++) {
		if (strcmp(chBuf, days[i]) == 0) {
			dayIndex = i;
		}
	}
	
	
	

	
	
	//creating threads related to day info
	
	CreateThreadsWithDayInfo();
	
	
	return 0;
}

void CreateAllThreads() {
	//allocating the memory
	handles = malloc(sizeof(HANDLE)* threadCount);
	lpParameter = malloc(sizeof(THREAD_PARAMETERS)* threadCount);
	threadID = malloc(sizeof(int)* threadCount);
	int i = 0;
	for (i = 0; i < threadCount; i++)
	{
		//initialize parameters
		lpParameter[i].itemId = i + 1;
		lpParameter[i].itemType = i;
		lpParameter[i].threadNo = i + 1;
		lpParameter[i].day = chBuf;


		handles[i] = CreateThread(NULL, 0, threadWork, &lpParameter[i], 0, &threadID[i]);

		


		//check errors in creation
		if (handles[i] == INVALID_HANDLE_VALUE)
		{
			printf("error when creating thread\n");
			system("pause");
			exit(0);
		}

		
		

			
	}
	//waiting for threads
	WaitForMultipleObjects(threadCount, handles, TRUE, INFINITE);
	//closing all handles
	for (i = 0; i < threadCount; i++)
	{
		CloseHandle(handles[i]);
	}
	
	
}
DWORD WINAPI threadWork(LPVOID parameters)
{
	THREAD_PARAMETERS* param = (THREAD_PARAMETERS*)parameters;
	FILE *fp;  /* file pointer*/
	
	char buf[100][BUFSIZE];
	int j = 0;

	errno_t x;

	//x = fopen_s(&fp, "C:\\Users\\cnrkrglr\\Desktop\\market.txt", "r");
	x = fopen_s(&fp, "../Debug/market.txt", "r");


	/*  if file is error , then excute error and exit failure */
	if (x != 0)
	{
		printf("\n Error while openeing the file	%d",x);
		system("pause");
		exit(0);
	}
	else {
		boolean tmpbool = FALSE;


		
		//reading from file and holding in an array with removing the last element of line. 
		while ((fgets(buf[j], BUFSIZE, fp)) != 0) {

			buf[j][strlen(buf[j]) - 1] = '\0';
			j++;
		}


		fclose(fp);
	}
	int i = 0;
	int dayStartIndex = 0;
	int dayEndIndex = 0;
	//defining the start and end days of the current day from the buffer.
	for (i = 0; i < j; i++) {
		
		
		if (strcmp(chBuf,buf[i])==0) {
			dayStartIndex = i;
			
			
		}
		if (dayIndex==6) {
			if (strcmp(buf[i], endDays[dayIndex]) == 0 || strcmp(buf[i], endDays[dayIndex+1]) == 0) {

				
				dayEndIndex = i;
			}

		}
		else {
			if (strcmp(buf[i], endDays[dayIndex]) == 0) {

				
				dayEndIndex = i;
			}
		}
		
		
		
	}
	
	char *token;
	char bufferEdited[BUFSIZE];
	int h = 0;
	int itemCounter = 0;
	//becuase the strtok editing the originall buffer, i created a clone one to solve that problem.  
	for (i = dayStartIndex + 1; i < dayEndIndex; i++) {
		
		for (h = 0; h < BUFSIZE; h++) {
			bufferEdited[h] = buf[i][h];
		}
		//spliting the buffer elements with defined delimeters
		token = strtok(bufferEdited,"(),");
		while (token!=NULL) {
			
			if (strcmp(token, itemNames[param->itemType])==0) {
				
				itemCounter++;
				
			}
			token = strtok(NULL, "(),");
			
		}
		
		
	}
	
	//gettint an Stdoutput with a special form to parse in parent.
	if (param->itemType==0) {
		milkCounter = itemCounter;
		printf("%d%d%d,",dayIndex, param->itemType,  milkCounter);
	}
	else if (param->itemType == 1) {
		biscuitCounter = itemCounter;
		printf("%d%d%d,", dayIndex, param->itemType, biscuitCounter);
	}
	else if (param->itemType == 2) {
		chipsCounter = itemCounter;
		printf("%d%d%d,", dayIndex, param->itemType, chipsCounter);
	}
	else if (param->itemType == 3) {
		cokeCounter = itemCounter;
		printf("%d%d%d,", dayIndex, param->itemType , cokeCounter);
	}
	else {
		printf("invalid item type entered to thread!!!\n");
		system("pause");
		exit(0);
	}
	
	
	return 0;
}
void ManageStdHandles() {
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	hStdin = GetStdHandle(STD_INPUT_HANDLE);
	if (
		(hStdout == INVALID_HANDLE_VALUE) ||
		(hStdin == INVALID_HANDLE_VALUE)
		)
		ExitProcess(1);

}

void CreateThreadsWithDayInfo()
{
	if (strcmp(chBuf, "#START DAY 1#") == 0) {

		CreateAllThreads();

	}

	else if (strcmp(chBuf, "#START DAY 2#") == 0) {

		CreateAllThreads();

	}
	else if (strcmp(chBuf, "#START DAY 3#") == 0) {

		CreateAllThreads();
	}
	else if (strcmp(chBuf, "#START DAY 4#") == 0) {

		CreateAllThreads();
	}
	else if (strcmp(chBuf, "#START DAY 5#") == 0) {

		CreateAllThreads();
	}
	else if (strcmp(chBuf, "#START DAY 6#") == 0) {

		CreateAllThreads();
	}
	else if (strcmp(chBuf, "#START DAY 7#") == 0) {

		CreateAllThreads();
	}
	else {

		printf("\n invalid day entered \n");
		system("pause");
		exit(0);

	}
}
