#include <stdio.h>
#include <windows.h>
#define BUFSIZE 4096

#define NO_OF_PROCESS 7
#pragma warning(disable:4996)


STARTUPINFO si[NO_OF_PROCESS];
PROCESS_INFORMATION pi[NO_OF_PROCESS];
SECURITY_ATTRIBUTES sa[NO_OF_PROCESS];
HANDLE writePipeStdIn[NO_OF_PROCESS], readPipeStdIn[NO_OF_PROCESS], writePipeStdOut[NO_OF_PROCESS], readPipeStdOut[NO_OF_PROCESS], processHandle[NO_OF_PROCESS];
DWORD dwRead, dwWritten;

CHAR chBuf[BUFSIZE];
BOOL bSuccess = FALSE;
//CHAR data[NO_OF_PROCESS][4];
CHAR itemNames[4][100] = { { " MILK" },{ " BISCUIT" },{ " CHIPS" },{ " COKE" } };
CHAR days[NO_OF_PROCESS][14] = { { "#START DAY 1#" },{ "#START DAY 2#" },{ "#START DAY 3#" },{ "#START DAY 4#" },{ "#START DAY 5#" },{ "#START DAY 6#" },{ "#START DAY 7#" } };

char allData[7][4][1];

void createProcess(int index);
void createSecurityAttributes(int index);
void setStartUpInfo(int index);
void createPipesForStdInOut(int index);
void closeAllHandles(int index);
void parentPrint1();
void parentPrint2();
void parentPrint3();
void parentPrint4();
void createDataArray();

int totalMilk=0;
int totalBis=0;
int totalChips=0;
int totalCoke=0;

int main(int argc, char* argv[])
{
	int index = 0;
	HANDLE hParentStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

	
	int bytesToWrite = 0;
	int bytesWritten = 0;
	int i = 0;


	
	for (index = 0; index < NO_OF_PROCESS; index++) {
		//creating security attributes
		createSecurityAttributes(index);


		//creating  pipe for child  std input
		
		createPipesForStdInOut(index);

		//creating variables for child process
		SecureZeroMemory(&si[index], sizeof(STARTUPINFO));
		SecureZeroMemory(&pi[index], sizeof(PROCESS_INFORMATION));

		setStartUpInfo(index);
			

		//create child process
		
		createProcess(index);
		
		//send messages through pipe

		
		bytesToWrite = strlen(days[index]);
		bytesToWrite++;

		
		//writing day info to pipe
		if (!WriteFile(writePipeStdIn[index], days[index], bytesToWrite, &bytesWritten, NULL))
		{
			printf("unable to write to pipe\n");
			system("pause");
			exit(0);
		}
		
		
		//reading stdoutput of child process and holding the data in chBuf
		bSuccess = ReadFile(readPipeStdOut[index], chBuf, BUFSIZE, &dwRead, NULL);
		if(!bSuccess){
			printf("unable to read file\n");
			system("pause");
			exit(0);
		}
		

	
		//parsing the child process output to a 3 dimensional array allData[day][item][item count]
		createDataArray();
		
	

		
		
		
		
		
	}
	//wating all processes
	WaitForMultipleObjects(NO_OF_PROCESS , processHandle, TRUE, INFINITE);
		

		//closing all handles
		
		for (i = 0; i < NO_OF_PROCESS; i++)
		{
			closeAllHandles(i);
		}
		
		int sum = 0;
		
		parentPrint1();//The most sold item in seven days
		printf("-----------------------------------------------------------\n");
		parentPrint2();//The most sold item for each day
		printf("-----------------------------------------------------------\n");
		parentPrint3();// The total number of each item sold in seven days 
		printf("-----------------------------------------------------------\n");
		parentPrint4();// The total number of each item sold in each day 
		printf("-----------------------------------------------------------\n");
	
	system("pause");
	return 1;
}


//*****************************************Functions*********************************************


void createProcess(int index) {
	if (!CreateProcess(NULL, "Child.exe", NULL, NULL, TRUE, 0, NULL, NULL, &si[index], &pi[index]))
	{
		printf("cannot create child process\n");
		system("pause");
		exit(0);
	}
	else {
		processHandle[index] = pi[index].hProcess;
	}
}
void setStartUpInfo(int index) {
	si[index].cb = sizeof(STARTUPINFO);
	si[index].hStdInput = readPipeStdIn[index];
	si[index].hStdOutput = writePipeStdOut[index];
	si[index].hStdError = writePipeStdOut[index];
	si[index].dwFlags = STARTF_USESTDHANDLES;
}
void createPipesForStdInOut(int index) {
	//creating  pipe for child std input
	if (!CreatePipe(&readPipeStdIn[index], &writePipeStdIn[index], &sa[index], 0)) //use the default buffer size.
	{
		printf("unable to create pipe\n");
		system("pause");
		exit(0);
	}
	//creating  pipe for child std output
	if (!CreatePipe(&readPipeStdOut[index], &writePipeStdOut[index], &sa[index], 0)) //use the default buffer size.
	{
		printf("unable to create pipe\n");
		system("pause");
		exit(0);
	}

}
void createSecurityAttributes(int index) {

	SecureZeroMemory(&sa[index], sizeof(SECURITY_ATTRIBUTES));
	sa[index].bInheritHandle = TRUE;
	sa[index].lpSecurityDescriptor = NULL;
	sa[index].nLength = sizeof(SECURITY_ATTRIBUTES);

}
void closeAllHandles(int index) {
	CloseHandle(readPipeStdOut[index]);
	CloseHandle(writePipeStdOut[index]);
	CloseHandle(readPipeStdIn[index]);
	CloseHandle(writePipeStdIn[index]);
	CloseHandle(pi[index].hThread);
	CloseHandle(pi[index].hProcess);
	
}
void parentPrint1() {
	int i = 0;
	int j = 0;
	int tmpData = 0;
	for (i = 0; i < 7; i++) {
		for (j = 0; j < 4; j++) {
			
			tmpData = (allData[i][j][0] - '0');
			if (j == 0) {
				totalMilk = totalMilk + tmpData;
			}
			else if (j == 1) {
				totalBis = totalBis + tmpData;
			}
			else if (j == 2) {
				totalChips = totalChips + tmpData;
			}
			else if (j == 3) {
				totalCoke = totalCoke + tmpData;
			}
		}

	}
	int totalArray[4] = { totalMilk, totalBis, totalChips, totalCoke };

	int max = 0;
	for (i = 0; i < 4; i++) {
		if (totalArray[i]>max) {
			max = totalArray[i];
		}
	}
	for (i = 0; i < 4; i++) {
		if (totalArray[i] == max) {
			printf("The most sold item in seven days :%s -> %d times\n", itemNames[i], max);
		}
	}
	
}
void parentPrint2() {
	int i = 0;
	int j = 0;
	int tmpData = 0;
	for (i = 0; i < 7; i++) {
		for (j = 0; j < 4; j++) {
			
			tmpData = (allData[i][j][0] - '0');
			if (j == 0) {
				totalMilk = tmpData;
			}
			else if (j == 1) {
				totalBis = tmpData;
			}
			else if (j == 2) {
				totalChips = tmpData;
			}
			else if (j == 3) {
				totalCoke = tmpData;
			}
		}
		int totalArray[4] = { totalMilk, totalBis, totalChips, totalCoke };
		int n = 0;
		int mostSoldItem = 0;
		for (n = 0; n < 4; n++) {
			if (totalArray[n]>mostSoldItem) {
				mostSoldItem = totalArray[n];
			}
		}
		int moreThanOneMax = 0;
		for (n = 0; n < 4; n++) {
			if (totalArray[n] == mostSoldItem) {

				if (moreThanOneMax == 0) {
					printf("The most sold item for  day %s :%s -> %d times ", days[i], itemNames[n], mostSoldItem);

				}
				if (moreThanOneMax >= 1) {

					printf("and %s -> %d times ", itemNames[n], mostSoldItem);
				}
				moreThanOneMax++;
			}
		}
		printf("\n");



	}

}

void parentPrint3() {
	int sum = 0;
	int i = 0;
	
	int j = 0;
	int tmpData = 0;
	totalMilk = 0; totalBis = 0; totalChips = 0; totalCoke = 0;
	for (i = 0; i < 7; i++) {
		for (j = 0; j < 4; j++) {
			
			tmpData = (allData[i][j][0] - '0');
			if (j == 0) {
				totalMilk = totalMilk + tmpData;
			}
			else if (j == 1) {
				totalBis = totalBis + tmpData;
			}
			else if (j == 2) {
				totalChips = totalChips + tmpData;
			}
			else if (j == 3) {
				totalCoke = totalCoke + tmpData;
			}
		}

	}
	int totalArray[4] = { totalMilk, totalBis, totalChips, totalCoke };
	for (i = 0; i < 4; i++) {

		sum = sum + totalArray[i];

	}
	printf("The total number of each item sold in seven days ->%d which are  ", sum);
	printf("MILK:%d BISQUITE:%d CHIPS:%d COKE:%d\n", totalMilk, totalBis, totalChips, totalCoke);

}
void parentPrint4() {
	int i = 0;
	int tmpData = 0;
	int j = 0;
	for (i = 0; i < 7; i++) {
		for (j = 0; j < 4; j++) {
			
			tmpData = (allData[i][j][0] - '0');
			if (j == 0) {
				totalMilk = tmpData;
			}
			else if (j == 1) {
				totalBis = tmpData;
			}
			else if (j == 2) {
				totalChips = tmpData;
			}
			else if (j == 3) {
				totalCoke = tmpData;
			}
		}
		int totalArray[4] = { totalMilk, totalBis, totalChips, totalCoke };
		
		

		int sum2 = 0;
		int m = 0;
		for (m = 0; m < 4; m++) {

			sum2 = sum2 + totalArray[m];

		}
		printf("The total number of each item sold in day %s -> %d\n", days[i], sum2);


	}
}

void createDataArray(){
	int i = 0;
	for (i = 0; i < strlen(chBuf); i++) {
		
		if (chBuf[i] == ',') {
			int dayNo = chBuf[i - 3] - '0';
			int itemNo1 = chBuf[i - 2] - '0';


			allData[dayNo][itemNo1][0] = chBuf[i - 1];
			
		}
	}
}
