#include <stdio.h>
#include <windows.h>

#define PIPE_NAME "\\\\.\\pipe\\employee_pipe"
#define FILE_NAME "employees.dat"

typedef struct {
    int num;
    char name[20];
    float hours;
} Employee;

void read_employee(FILE* fp, int n, Employee* emp) {
    fseek(fp, (n - 1) * sizeof(Employee), SEEK_SET);
    fread(emp, sizeof(Employee), 1, fp);
}

void write_employee(FILE* fp, int n, Employee* emp) {
    fseek(fp, (n - 1) * sizeof(Employee), SEEK_SET);
    fwrite(emp, sizeof(Employee), 1, fp);
    fflush(fp);
}

int main() {
    HANDLE hMutex = CreateMutex(NULL, FALSE, NULL);
    if (hMutex == NULL) {
        printf("Failed to create mutex, error code: %d\n", GetLastError());
        return 1;
    }

    FILE* fp = fopen(FILE_NAME, "rb+");
    if (fp == NULL) {
        printf("Failed to open file %s\n", FILE_NAME);
        return 1;
    }

    HANDLE hPipe = CreateNamedPipe(
        PIPE_NAME,                   // 名称管道的名称
        PIPE_ACCESS_DUPLEX,          // 可读可写
        PIPE_TYPE_MESSAGE | PIPE_WAIT,  // 消息类型，等待模式
        1,                           // 最多1个实例
        0,                           // 输出缓冲区大小
        0,                           // 输入缓冲区大小
        0,                           // 默认超时时间
        NULL                         // 默认安全属性
    );
    if (hPipe == INVALID_HANDLE_VALUE) {
        printf("Failed to create named pipe, error code: %d\n", GetLastError());
        return 1;
    }

    printf("Waiting for client...\n");

    if (!ConnectNamedPipe(hPipe, NULL)) {
        printf("Failed to connect named pipe, error code: %d\n", GetLastError());
        CloseHandle(hPipe);
        return 1;
    }

    printf("Client connected\n");

    while (1) {
        char data[256];
        DWORD dwBytesRead, dwBytesWritten;

        if (!ReadFile(hPipe, data, sizeof(data), &dwBytesRead, NULL)) {
            printf("Failed to read from pipe, error code: %d\n", GetLastError());
            CloseHandle(hPipe);
            return 1;
        }

        if (strcmp(data, "READ") == 0) {
            // 读取员工信息
            int num;
            Employee emp;
            char response[256];

            if (!ReadFile(hPipe, &num, sizeof(num), &dwBytesRead, NULL)) {
                printf("Failed to read from pipe, error code: %d\n", GetLastError());
                CloseHandle(hPipe);
                return 1;
            }

            WaitForSingleObject(hMutex, INFINITE);
            read_employee(fp, num, &emp);
            ReleaseMutex(hMutex);

            sprintf(response, "%d,%s,%.2f", emp.num, emp.name, emp.hours);
            if (!WriteFile(hPipe, response, strlen(response) + 1, &dwBytesWritten, NULL)) {
                printf("Failed to write to pipe, error code: %d\n", GetLastError());
                CloseHandle(hPipe);
                return 1;
            }
        }
        else if (strcmp(data, "WRITE") == 0) {
            // 写入员工信息
            int num;
            Employee emp;
            char response[256];

            if (!ReadFile(hPipe, &emp, sizeof(emp), &dwBytesRead, NULL)) {
                printf("Failed to read from pipe， error code: %d\n", GetLastError());
                CloseHandle(hPipe);
                return 1;
            }

            if (!ReadFile(hPipe, &num, sizeof(num), &dwBytesRead, NULL)) {
                printf("Failed to read from pipe, error code: %d\n", GetLastError());
                CloseHandle(hPipe);
                return 1;
            }

            WaitForSingleObject(hMutex, INFINITE);
            write_employee(fp, num, &emp);
            ReleaseMutex(hMutex);

            strcpy(response, "OK");
            if (!WriteFile(hPipe, response, strlen(response) + 1, &dwBytesWritten, NULL)) {
                printf("Failed to write to pipe, error code: %d\n", GetLastError());
                CloseHandle(hPipe);
                return 1;
            }
        }
        else if (strcmp(data, "EXIT") == 0) {
            // 退出
            break;
        }
        else {
            printf("Invalid request: %s\n", data);
        }
    }

    // 关闭句柄和文件
    CloseHandle(hPipe);
    CloseHandle(hMutex);
    fclose(fp);

    return 0;
}