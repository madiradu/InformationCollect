#define _REGEX_MAX_STACK_COUNT 200000
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <winuser.h>
#include<iostream>
#include <io.h>   // For access().
#include <sys/types.h>  // For stat().
#include <sys/stat.h>   // For stat().
#include <cstdio>
#include <chrono>
#include <thread>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <stdlib.h> 
#include <vector>
#include <iostream>
#include <fstream>
#include <json/value.h>
#include <json/json.h>
#include <fstream>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <io.h>
#include <regex>
#include <map>
#include <iostream>
#include <chrono>
#include <ctime>
#include <codecvt>
#include <locale>




using namespace std;

//TO RUN USE ON WINDOWS vcpkg  jsoncpp



auto start = std::chrono::system_clock::now();

void task1(std::string msg)
{

	while (true) {
		auto end = std::chrono::system_clock::now();
		std::chrono::duration<double> elapsed = end - start;
		if ((std::chrono::duration_cast<std::chrono::milliseconds>(elapsed)) < std::chrono::milliseconds(5000)) {
			continue;
			start = end;

		}
	}


	try {
		std::wifstream resfile("test1.ps1");
		std::wstring line;
		std::map<long, string*> details;

		std::regex regex1("^(\\d)");
		std::regex regex("^\\d{4,5}\s([.]+)\n");

		if (resfile.is_open()) {
			//std::cout << "result file opened" << std::endl;
			while (getline(resfile, line)) {
				string b;
				unsigned int procId;
				if (((unsigned int)line[8] < 48) || ((unsigned int)line[8] > 57)) continue;

				std::wstring delimiter = L"     ";
				std::wstring token = line.substr((line.find(delimiter) == std::wstring::npos) ? (line.length() - 1) : line.find(delimiter) + delimiter.length());
				if (token != L"") {
					using convert_type = std::codecvt_utf8<wchar_t>;
					std::wstring_convert<convert_type, wchar_t> converter;
					string ab = converter.to_bytes(token.c_str());
					procId = atoi(ab.c_str());
					delimiter = std::to_wstring(procId);
					b = converter.to_bytes(token.substr(0, (token.find(delimiter) == std::wstring::npos) ? (token.length() - 1) : token.find(delimiter)));

				}


				if ((procId == 0)) { continue; }
				//if (details.find(procId) != details.end())

				details.insert(std::pair<long, string*>(procId, new string(b)));

			}
		}
		resfile.close();


		for (map<long, string* >::const_iterator it = details.begin();
			it != details.end(); ++it)
		{
			//std::cout << it->first << " " << it->second << " " << it->first << "\n";
		}

		Json::Value processes;
		int i = 0;
		for (map<long, string* >::const_iterator it = details.begin();
			it != details.end(); ++it)
		{
			Json::Value process;
			std::string processId = std::to_string(it->first);
			process["name"] = Json::Value(it->second);


			processes[i] = (process);
			i++;
		}

		std::ofstream results;
		results.open("results.txt", ios::app);
		results << processes << endl;
		results.close();
	}
	catch (...) { ; }

}






BOOL WINAPI consoleHandler(DWORD signal) {

	if (signal == CTRL_C_EVENT){
		
		exit(0);
}

	return TRUE;
}

void _tmain(int argc, TCHAR *argv[])
{
	std::thread t1(task1, "Data Parsing Thread");
	bool running = TRUE;
	if (!SetConsoleCtrlHandler(consoleHandler, TRUE)) {
		printf("\nERROR: Could not set control handler");

	}

	while (1) {

		STARTUPINFO si;
		PROCESS_INFORMATION pi;

		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		ZeroMemory(&pi, sizeof(pi));

		if (argc != 3)
		{
			printf("Usage: %s [cmdline]\n", argv[0]);
			return;
		}



		GetStartupInfo(&si);
 
		if (!CreateProcess(NULL,   // No module name (use command line)
			argv[1],        // Command line
			NULL,           // Process handle not inheritable
			NULL,           // Thread handle not inheritable
			FALSE,          // Set handle inheritance to FALSE
			0,              // No creation flags
			NULL,           // Use parent's environment block
			NULL,           // Use parent's starting directory 
			&si,            // Pointer to STARTUPINFO structure
			&pi)           // Pointer to PROCESS_INFORMATION structure
			)
		{
			printf("CreateProcess failed (%d).\n", GetLastError());
			return;
		}

		// Wait until child process exits.

		HWND hWnd = CreateWindowExA(WS_EX_CLIENTEDGE
			,
			"Spawned program",
			"Spawned program",
			WS_EX_CLIENTEDGE,
			CW_USEDEFAULT, // X coordinate
			CW_USEDEFAULT, // Y coordinate
			300, // Width
			200, // Height
			0, 0, 0, 0);

		ShowWindow(hWnd, SW_SHOWDEFAULT);


		std::string psfilename = "test1.ps1";



		system("start powershell.exe Set-ExecutionPolicy RemoteSigned \n");
		system("powershell.exe Remove-Variable * -ErrorAction SilentlyContinue; Remove-Module *; $error.Clear(); Clear-Host");

		while (true) {

			system((std::string("powershell.exe -command  \"Get-CimInstance -class win32_service | Where-Object{ $_.State -eq 'Running' } | Select-Object ProcessId, Name | Out-String \" >>" + psfilename).c_str()));

			//WaitForSingleObject(pi.hProcess, (int)argv[2]);
		}				



	// Close process and thread handles. 
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
}




