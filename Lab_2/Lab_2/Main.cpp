#include <iostream>
#include <windows.h>
#include <fstream>
#include <string>

using namespace std;

const int len_num = 4;
const int len_name = 20;
const int len_par = 50;

struct employee {
	int num;
	char name[10];
	double hours;
};

void start_process(char* process_name, char* par)
{
	STARTUPINFOA sinf;
	ZeroMemory(&sinf, sizeof(STARTUPINFO));
	sinf.cb = sizeof(STARTUPINFO);

	PROCESS_INFORMATION pi;
	CreateProcessA(NULL, par, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &sinf, &pi);
	DWORD exit_code;
	if (WaitForSingleObject(pi.hProcess, INFINITE) == WAIT_FAILED)
	{
		std::cerr << "GetExitCodeProcess()失败。 " << GetLastError() << "\n";
	}
	if (GetExitCodeProcess(pi.hProcess, &exit_code) == FALSE)
	{
		std::cerr << "GetExitCodeProcess()失败。 " << exit_code <<  "\n";
	}
	
	cout << process_name << " Exit " << endl;
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
}

void print(char* file_name, bool bin)
{
	cout << "\n\n" << file_name << ":\n";
	ifstream fin;

	if (bin)
	{
		fin.open(file_name, ios::binary | ios::out);
		employee temp;
		do
		{
			fin.read((char*)&temp, sizeof(employee));
			cout << temp.num << " " << temp.name << " " << temp.hours << endl;
		} while (fin.gcount() > 0);
	}
	else
	{
		fin.open(file_name);
		while (!fin.eof())
		{
			string tmp;
			getline(fin, tmp);
			cout << tmp << endl;
		}
	}


	fin.close();

	cout << "\n\n";
}

int main()
{
	char bin_name[len_name];
	char num[len_num];
	char par[len_par];
	char rep_name[len_name];
	char grade[len_num];

	cout << "输入bin文件名 Введите имя файла bin:\n";
	cin.getline(bin_name, len_name);

	cout << "输入记录的数量 Введите количество записей:\n";
	cin.getline(num, len_num);

	sprintf_s(par, "%s %s %s","Creator.exe", bin_name, num);
	cout << "启动进程Creator.exe" << endl;
	start_process((char*)"D:\IDM_download\OperatingSystems-main\OperatingSystems-main\lab1\Lab_2\Debug\Creator.exe", par);
	print(bin_name, true);

	cout << "\n输入报告文件的名称 Введите имя файла отчета\n";
	cin.getline(rep_name, len_name);

	cout << "输入工资 Введите зарплату \n";
	cin.getline(grade, len_num);

	sprintf_s(par, "%s %s %s %s","Reporter.exe", bin_name, rep_name, grade);
	cout << "启动进程Reporter.exe" << endl;
	start_process((char*)"D:\IDM_download\OperatingSystems-main\OperatingSystems-main\lab1\Lab_2\Debug\Reporter.exe", par);
	print(rep_name, false);
}

