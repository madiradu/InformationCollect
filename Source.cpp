
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
#include <map>
#include <iostream>
#include <chrono>
#include <ctime>
#include <codecvt>
#include <locale>
#include <ratio>

#include <numeric>


using namespace std;

//TO RUN USE ON WINDOWS vcpkg  jsoncpp



auto start = std::chrono::system_clock::now();

thread_local int J = 0;

string map_to_string(const map<string, string*>  &m) {
	string output = "[";
	string convrt = "";
	string result = "";

	for (auto it = m.cbegin(); it != m.cend(); it++) {

		convrt = *(it->second);
		output += "{" + (it->first) + ":" + (convrt)+"}, ";
	}

	result = output.substr(0, output.size() - 2);
	output += "]";
	return result;
}


void task1(int *p)
{

	while (true) {
		auto end = std::chrono::system_clock::now();
		std::chrono::duration<double> elapsed = end - start;

		typedef std::chrono::duration<int, std::milli> milliseconds_types;

		milliseconds_types ms_oneday(*p);
		if ((std::chrono::duration_cast<std::chrono::milliseconds>(elapsed)) < std::chrono::milliseconds(ms_oneday)) {

			continue;

		}
		else {
			start = end;

			break;
		}
	}


		//try {
		std::wifstream resfile("test1.ps1");
		std::wstring line;
		std::map<string, string*> details;


		if (resfile.is_open()) {
			//std::cout << "result file opened" << std::endl;
			while (getline(resfile, line)) {
				string b;
				double procId;
				if (line.length() > 16) {
					if (((unsigned int)line[17] < 48) || ((unsigned int)line[17] > 57)) { continue; }
				}
				else
				{
					continue;
				}

				std::wstring delimiter = L" ";
				std::wstring token = line.substr(0, line.find(delimiter));
				if (token != L"") {
					using convert_type = std::codecvt_utf8<wchar_t>;
					std::wstring_convert<convert_type, wchar_t> converter;
					string ab = converter.to_bytes(token.c_str());
					delimiter = token;
					wstring abc =
						line.substr(line.find(delimiter) + delimiter.length(), line.length() - 1);
					b = converter.to_bytes(abc.c_str());
					b.erase(0, b.find_first_not_of(" \n\r\t"));
					b.erase(b.find_last_not_of(" \n\r\t") + 1);
					procId = atoi(b.c_str());
					//if ((procId == 0)) { continue; }

					details.insert(std::pair<string, string*>(ab, new string(b)));

				}



			}
		}






		string result = map_to_string(details);


		Json::StreamWriterBuilder builder;
		builder["indentation"] = "";  // assume default for comments is None
		std::string str = Json::writeString(builder, result);





		std::ofstream results;
		results.open("results.txt", ios::app);
		results << result;// << endl;
		results.close();
		//}
		//catch (...) { ; }
	}



BOOL WINAPI consoleHandler(DWORD signal) {

	if (signal == CTRL_C_EVENT){
		
		exit(0);
}

	return TRUE;
}



void _tmain(int argc, TCHAR *argv[])
{


	J = _wtoi(argv[2]);
	std::thread t1(task1, &J);
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

			//system((std::string("powershell.exe -command  \"Get-CimInstance -class win32_service | Where-Object{ $_.State -eq 'Running' } | Select-Object ProcessId, Name | Out-String \" >>" + psfilename).c_str()));

			system((std::string(R"(powershell.exe -command  "(Get-Counter '\Process(*)\% Processor Time').CounterSamples | Select InstanceName, @{Name='CPU %';Expression={[Decimal]::Round(($_.CookedValue / (Get-WMIObject Win32_ComputerSystem).NumberOfLogicalProcessors), 2)}} | Out-String " >>)" + psfilename).c_str()));

			//Get-process  -name notepad | Sort PrivateMemorySize -Descending | Select PrivateMemorySize -First 10



			//get-process -name notepad |Measure-Object Handles -Sum |% Sum
			//

			//WaitForSingleObject(pi.hProcess, (int)argv[2]);
		}				



	// Close process and thread handles. 
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
}




