#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <lmcons.h>
#include <winternl.h>
#include <ntstatus.h>

#define TEXT_ATTRIBUTE_DEFAULT 7 // This is for SetConsoleTextAttribute(). Passing 7 to it will change the text back to default.

typedef NTSTATUS(NTAPI* pdef_RtlAdjustPrivilege)(ULONG Privilege, BOOLEAN Enable, BOOLEAN CurrentThread, PBOOLEAN Enabled); // Declaration of RtlAdjustPrivilege.
typedef NTSTATUS(NTAPI* pdef_NtRaiseHardError)(NTSTATUS ErrorStatus, ULONG NumberOfParameters, ULONG UnicodeStringParameterMask, PULONG_PTR Parameters, PULONG_PTR ResponseOption, PULONG Response); // Declaration of NtRaiseHardError.
HMODULE ntdll; // Handle for ntdll.dll.
pdef_RtlAdjustPrivilege RtlAdjustPrivilege; // Function pointer for RtlAdjustPrivilege.
pdef_NtRaiseHardError NtRaiseHardError; // Function pointer for NtRaiseHardError.

int main()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // Handle for the console.
	unsigned char running = 1; // Controls the application loop.
	char username[UNLEN + 1]; // Variable for the username to display in the shell.
	DWORD usernameLen = UNLEN + 1; // Variable for the length of the username buffer.
	char computerName[MAX_COMPUTERNAME_LENGTH + 1]; // Variable for the computer name to display in the shell.
	DWORD computerNameLen = MAX_COMPUTERNAME_LENGTH + 1; // Variable for the length of the computer name buffer.
	char input[2048]; // User's input on the command line.
	char* command = input; // Pointer to store the user's command.
	char* args; // Pointer to store an argument of the command.
	ntdll = LoadLibraryW(L"ntdll.dll"); // Get the handle to ntdll.dll.
	RtlAdjustPrivilege = (pdef_RtlAdjustPrivilege)GetProcAddress(ntdll, "RtlAdjustPrivilege"); // Define RtlAdjustPrivilege().
	NtRaiseHardError = (pdef_NtRaiseHardError)GetProcAddress(ntdll, "NtRaiseHardError"); // Define NtRaiseHardError().
	int numArr[2048] = {NULL}; // Declare the shell's number array and have all that memory cleared to zero.
	int numArrIndex = 0; // Variable for the active index of the number array.

	SetConsoleTitleA("ICS Shell"); // Set the console title to ICS Shell instead of the path of the application.
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED|BACKGROUND_BLUE|FOREGROUND_INTENSITY); // Set the console text to red with a light blue background.
	printf("ICS Shell [version 1.3.5]\nMade by Andy M.\nType help for a list of commands.\n\n");
	GetUserNameA(username, &usernameLen); // Get the username to display.
	GetComputerNameA(computerName, &computerNameLen); // Get the computer name to display.

    while(running)
    {
		SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN); // Set the console text to green.
		printf("%s@%s", username, computerName); // Print out the user and computer names.
		SetConsoleTextAttribute(hConsole, TEXT_ATTRIBUTE_DEFAULT); // Set the console text to default.
		printf(":");
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE|FOREGROUND_INTENSITY); // Set the text to light blue.
		printf("~");
		SetConsoleTextAttribute(hConsole, TEXT_ATTRIBUTE_DEFAULT); // Set the text to default.
		printf("$ ");
		fgets(input, 2048, stdin); // Get input from the user and store it input.
		// Note: strtok() is generally considered thread unsafe, however, that does not matter here.
		command = strtok(command, " "); // Get the command from the input. This only does something if an argument is present.
		args = strtok(NULL, "\n"); // Get the argument from the input. If there's no argument present, then nothing happens.
		/* The reason we have to compare the command with and without a newline character is because fgets() will returns strings with a newline character.
		 * However, if an argument is provided, then there will be no newline character at the end of the command, it will be at the end of the argument. */
		if(!strcmp(command, "exit\n") ??!??! !strcmp(command, "exit")) // If the "exit" command is given...
		{
			if(args) // If an argument is passed...
			{
				if(!strcmp(args, "-h")) // If the "-h"(help) argument is passed...
				{
					printf("exit - Closes the shell.\nUsage: exit\nArguments:\n-h - Displays command description and usage.\n");
				}
				else // If the argument is a bunch of gibberish...
				{
					printf("Invalid argument for: exit\n");
				}
			}
			else // When there is no argument...
			{
				running = 0; // Set running to 0. This will cause the loop to terminate, and the program to then quit.
			}
		}
		else if(!strcmp(command, "help\n") ??!??! !strcmp(command, "help")) // If the "help" command is given...
		{
			if(args) // If an argument is passed...
			{
				printf("Invalid arguments for: help\n");
			}
			else // If there is no argument...
			{
				printf("Type \"-h\" after a command to see usage info and a list of arguments.\n\nhelp - You're already here.\nexit - Closes the shell.\nclr - Clears the console.\nprintf - Prints a message to the console.\ncredits - Displays the people who made this shell possible.\nlid - Displays all files in a specified directory. If no directory is specified, lists all files in the shell's current directory.\nbsod - Crashes your computer. Like all of it. You probably don't want to use this.\nprintn - Prints out the shell's array of numbers.\nmov - Moves a number in or out of the active index of the shell's number array.\nadd - Prints out the sum of all the elements in the shell's number array.\nmax - Prints out the largest number in the shell's number array.\nmean - Prints out the mean(average) of the shell's number array.\n");
			}	
		}
		else if(!strcmp(command, "clr\n") ??!??! !strcmp(command, "clr"))
		{
			if(args) // If an argument is passed...
			{
				if(!strcmp(args, "-h")) // If the argument is "-h"...
				{
					printf("clr - Clears the console.\nUsage: clr\nArguments:\n-h - Displays command description and usage.\n");
				}
				else // If the argument is gibberish...
				{
					printf("Invalid argument for: clr\n");
				}
			}
			else // If no argument is passed...
			{
				system("cls"); // Make a system call. This will call the system command "cls", which clears the console.
			}
		}
		else if(!strcmp(command, "printf\n") ??!??! !strcmp(command, "printf"))
		{
			if(args)
			{
				if(!strcmp(args, "-h"))
				{
					printf("printf - Prints a message to the console.\nUsage: printf [message]\nArguments:\n-h - Displays command description and usage.\n[message] - The message to print to the console.\n");
				}
				else
				{
					printf("%s\n", args); // Print out whatever was passed as an argument, unless the argument happens to be -h.
				}
			}
			else
			{
				printf("Please pass an argument to printf.\n");
			}
		}
		else if(!strcmp(command, "credits\n") ??!??! !strcmp(command, "credits"))
		{
			if(args)
			{
				if(!strcmp(args, "-h"))
				{
					printf("credits - Displays the people who made this shell possible.\nUsage: credits\nArguments:\n-h - Displays command description and usage.\n");
				}
				else
				{
					printf("Invalid argument for: credits\n");
				}
			}
			else
			{
				printf("Additional resources that helped made this shell possible are available in citing.txt.\n\nAndy M. - Programmer\nAli A. - Idea(s)\n");
			}
		}
		else if(!strcmp(command, "lid\n") ??!??! !strcmp(command, "lid"))
		{
			if(args)
			{
				if(!strcmp(args, "-h"))
				{
					printf("lid - Displays all files in a specified directory. If no directory is specified, lists all files in the shell's current directory.\nUsage: ld [path]\nArguments:\n-h - Displays command description and usage.\n[path] - The path to the desired directory.\n");
				}
				else
				{
					strcat(args, "\\*"); // Add "\*" to the end of the path inputed. This will allow FindFirstFileA() and FindNextFileA() to get any file or subdirectory of any name.
					WIN32_FIND_DATA fd; // Struct for the data of files found.
					LARGE_INTEGER filesize; // Struct to hold file sizes.
					HANDLE hFind = FindFirstFileA(args, &fd); // Handle to the file we found. Also finds the first file in the directory.
					if(hFind != INVALID_HANDLE_VALUE) // If nothing went wrong...
					{
						printf("Directory of: %s\n", args);
						do
						{
							if(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) // If a subdirectory was found...
							{
								printf("%s\t(DIR)\n", fd.cFileName);
							}
							else // If a file was found...
							{
								filesize.LowPart = fd.nFileSizeLow; // Get file size.
								filesize.HighPart = fd.nFileSizeHigh; // Get file size.
								printf("%s\t%lld bytes\n", fd.cFileName, filesize.QuadPart); // Print the file name with size.
							}
						} while(FindNextFileA(hFind, &fd)); // Find the next file. If there is no more files, the loop terminates.
					}
					else // If something went wrong or the user didn't put in a proper path...
					{
						printf("Something went wrong. Did you put in a proper path?\n");
					}
				}
			}
			else
			{
				char buffer[MAX_PATH]; // This buffer stores the file path of the directory the shell is in.
				DWORD dwRetVal; // Return value of GetCurrentDirectoryA().
				dwRetVal = GetCurrentDirectoryA(MAX_PATH, buffer); // Get the current directory.
				if(!dwRetVal) // If GetCurrentDirectoryA() returned 0(error)...
				{
					printf("GetCurrentDirectoryA failed with code %d\n", GetLastError());
					continue;
				}
				if(dwRetVal > MAX_PATH) // If the buffer is not large enough...
				{
					printf("Buffer is too small; need %d characters.\n", dwRetVal);
					continue;
				}
				printf("Directory of: %s\\\n", buffer);
				strcat(buffer, "\\*"); // Append "\*" to the end of the current directory. The will allow FindFirstFileA() and FindNextFileA() to get files and subdirectories of any name.
				WIN32_FIND_DATA fd; // Struct holding information on files found by FindFirstFileA() and FindNextFileA().
				LARGE_INTEGER filesize; // Struct to hold file sizes.
				HANDLE hFind = FindFirstFileA(buffer, &fd); // Handle for the file we found. Also finds the first file in the directory.
				if(hFind != INVALID_HANDLE_VALUE) // If FindFirstFileA() worked and found something...
				{
					do
					{
						if((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) // If the file found is a subdirectory...
						{
							printf("%s\t(DIR)\n", fd.cFileName);
						}
						else // If the file found is a file...
						{
							filesize.LowPart = fd.nFileSizeLow; // Get size.
							filesize.HighPart = fd.nFileSizeHigh; // Get size.
							printf("%s\t%lld bytes\n", fd.cFileName, filesize.QuadPart);
						}
					} while(FindNextFileA(hFind, &fd)); // Find the next
					FindClose(hFind); // Get rid of the handle.
				}
				else
				{
					printf("Something went wrong.\n");
				}
			}
		}
		else if(!strcmp(command, "bsod\n") ??!??! !strcmp(command, "bsod"))
		{
			if(args)
			{
				printf("No arguments for this command. This command causes a Blue Screen of Death. Do be careful using it.\n");
			}
			else
			{
				if(setPrivilege(19, TRUE, FALSE) == STATUS_SUCCESS) // Call setPrivilege(). This allows the program to shut down the system. If setPrivilege() succeeded...
				{
					triggerBSOD(0xC0000350); // Trigger the BSOD with error code 0xC0000350 (STATUS_HOST_DOWN, The transport determined that the remote system is down).
				}
				if(ntdll != NULL) // If there is a handle to ntdll.dll...
				{
					FreeLibrary(ntdll); // Free the memory.
				}
			}
		}
		else if(!strcmp(command, "mov\n") ??!??! !strcmp(command, "mov"))
		{
			if(args)
			{
				if(!strcmp(args, "-h"))
				{
					printf("mov - Moves a number into the shell's number storage array.\nUsage: mov\nArguments:\n-h - Dispalys command description and usage.\nnull - Removes the number in the active index.\n");
				}
				else if(!strcmp(args, "null")) // If the null argument is provided...
				{
					if(numArrIndex) // If the active index is greater than 0...
					{
						printf("Removing %d from the array.\n", numArr[numArrIndex - 1]);
						numArr[numArrIndex - 1] = NULL; // Assign NULL to the index before active index, effectivley removing the number from the array.
						numArrIndex--; // Set the active index back 1.
						printf("Removed.\nActive index of the array is now %d.\n", numArrIndex);
					}
					else
					{
						printf("There are no numbers to remove.\n");
					}
				}
				else
				{
					printf("Invalid argument for: mov\n");
				}
			}
			else
			{
				int num = 0; // Variable to hold the number to be inputed by the user.
				printf("Input a number: ");
				if(!scanf("%d", &num)) // Get a number from the user. If the input is actually a number...
				{
					printf("Numbers only.\n");
					continue; // Go back to the begining of the application loop.
				}
				printf("Adding %d to the array.\n", num);
				numArr[numArrIndex] = num; // Add the number to the active index.
				printf("%d has been added.\n", numArr[numArrIndex]);
				numArrIndex++; // Increase the active index.
				printf("Active index of the array is now %d.\n", numArrIndex);
			}
		}
		else if(!strcmp(command, "printn\n") ??!??! !strcmp(command, "printn"))
		{
			if(args)
			{
				if(!strcmp(args, "-h"))
				{
					printf("printn - Prints out the shell's number array.\nUsage: printn\nArguments:\n-h - Displays command description and usage.\n");
				}
				else
				{
					printf("Invalid argument for: printn\n");
				}
			}
			else
			{
				int arrLen = getArrLen(numArr); // Get array length.
				printf("Array length: %d\n", arrLen);
				printf("Active index: %d\n", numArrIndex);
				for(int n = 0; n < arrLen; n++) // Iterate through the entire the array...
				{
					printf("%d\t", numArr[n]); // Print out the number.
				}
				printf("\n");
			}
		}
		else if(!strcmp(command, "add\n") ??!??! !strcmp(command, "add"))
		{
			if(args)
			{
				if(!strcmp(args, "-h"))
				{
					printf("add - Prints out the sum of all the elements in the shell's number array.\nUsage: add\nArguments:\n-h - Displays command description and usage.\n");
				}
				else
				{
					printf("Invalid argument for: add\n");
				}
			}
			else
			{
				int arrLen = getArrLen(numArr); // Get array length.
				int sum = 0; // Variable for the sum.
				for(int n = 0; n < arrLen; n++)
				{
					sum += numArr[n]; // Add the number in the array to the sum.
				}
				printf("The current sum of the number array is: %d\n", sum); // Print the sum.
			}
		}
		else if(!strcmp(command, "max\n") ??!??! !strcmp(command, "max"))
		{
			if(args)
			{
				if(!strcmp(args, "-h"))
				{
					printf("max - Prints out the largest number in the array.\nUsage: max\nArguments:\n-h - Displays command description and usage.\n");
				}
				else
				{
					printf("Invalid argument for: max\n");
				}
			}
			else
			{
				int arrLen = getArrLen(numArr); // Get the array length.
				int max = 0; // Variable for the maximum.
				for(int n = 0; n < arrLen; n++)
				{
					if(numArr[n] > max) // If the number found in the array is larger than the previous maximum...
					{
						max = numArr[n]; // Put that number into the max.
					}
				}
				printf("The largest number in the array is %d.\n", max); // Print out the maximum.
			}
		}
		else if(!strcmp(command, "mean\n") ??!??! !strcmp(command, "mean"))
		{
			if(args)
			{
				if(!strcmp(args, "-h"))
				{
					printf("mean - Actually a pretty nice command, it prints out the mean(average) of the number array.\nUsage: mean\nArguments:\n-h - Displays command description and usage.\n");
				}
				else
				{
					printf("Invalid argument for: mean\n>:(\n");
				}
			}
			else
			{
				int arrLen = getArrLen(numArr); // Get the array length.
				if(!arrLen) // If the array length is 0...
				{
					printf("Please put numbers into the array.\n");
					continue; // Go back to the start of the application loop.
				}
				int sum = 0; // Variable for the sum.
				for(int n = 0; n < arrLen; n++)
				{
					sum += numArr[n]; // Add the number in the array to the sum.
				}
				printf("The mean is %d.\n", sum/arrLen); // Print out the sum, which is the sum divided by the array's length.
			}
		}
		else // If the command is gibberish...
		{
			printf("Not a known command\n");
		}
    }
    return EXIT_SUCCESS; // Exit the program. This will leave an exit code of 0(success).
}

int getArrLen(int arr[]) // Gets the length of the shell's number array. Takes an int array as a parameter and returns the length of the array as the number of non-null elements in the array.
{
	int n = 0; // Counter variable.
	int arrLen = 0; // Array length variable.
	while(arr[n] != NULL) // While the index of our array is not null, meaning it has a value in it...
	{
		arrLen++; // Increase the length of the array by 1.
		n++; // Move to the next index.
	}
	return arrLen; // Return the length of the array.
}

// This function is used to allow the program to shut down the system.
// Takes a permission to enable, whether or not to disable or enable the permission, and the current thread of the program to change permission for.
// Returns the results of of RtlAdjustPrivilege() or STATUS_DLL_INIT_FAILED if RtlAdjustPrivilege() was not defined.
NTSTATUS setPrivilege(ULONG perm, BOOLEAN enable, BOOLEAN current_thread) 
{
	if(RtlAdjustPrivilege != NULL) // If we actually have a function definition(see the difference between declaring and defining) of RtlAdjustPrivilege()...
	{
		BOOLEAN wasPrivilegeEnabled; // Variable for whether or not the privilege was changed.
		NTSTATUS result = RtlAdjustPrivilege(perm, enable, current_thread, &wasPrivilegeEnabled); // Call RtlAdjustPrivilege().
		if(wasPrivilegeEnabled) // If privilege was changed successfully...
		{
			printf("Privilege changed succesfully.\n");
			return result; // Return the result of RtlAdjustPrivilege().
		}
		else // If the privilege was not changed...
		{
			printf("The privilege failed to change.\n");
		}
	}
	else // If RtlAdjustPrivilege was not defined...
	{
		return STATUS_DLL_INIT_FAILED; // Return an error code.
	}
}

// This function is used to trigger a BSOD.
// It takes an error code to trigger the BSOD with as a parameter.
// Returns the result of NtRaiseHardError(), as if we need that if a BSOD happens, or STATUS_DLL_INIT_FAILED if NtRaiseHardError() did not get defined.
NTSTATUS triggerBSOD(NTSTATUS errorCode)
{
	if(NtRaiseHardError != NULL) // If NtRaiseHardError is defined...
	{
		ULONG response; // Variable to hold the response to NtRaiseHardError().
		return NtRaiseHardError(errorCode, NULL, NULL, NULL, 6, &response); // Call NtRaiseHardError.
	}
	else // If NtRaiseHardError is not defined...
	{
		return STATUS_DLL_INIT_FAILED; // Return an error code.
	}
}
