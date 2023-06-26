#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define PIPE_NAME L"\\\\.\\pipe\\my_pipe"
#define BUFFER_SIZE 1024

struct employee {
    int num;
    char name[10];
    double hours;
};

int main() {
    HANDLE hPipe;
    DWORD dwBytesWritten, dwBytesRead;
    char data[BUFFER_SIZE];
    struct employee emp;
    int choice;

    // ���������ܵ�
    hPipe = CreateFile(PIPE_NAME, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (hPipe == INVALID_HANDLE_VALUE) {
        printf("Failed to connect to server, error code: %d\n", GetLastError());
        return 1;
    }

    while (1) {
        printf("Enter operation (1=modify, 2=read, 3=exit): ");
        scanf("%d", &choice);

        if (choice == 1) {
            // �޸�Ա����Ϣ
            printf("Enter employee number: ");
            scanf("%d", &emp.num);

            // ��������
            strcpy(data, "MODIFY");
            if (!WriteFile(hPipe, data, strlen(data) + 1, &dwBytesWritten, NULL)) {
                printf("Failed to write to pipe, error code: %d\n", GetLastError());
                CloseHandle(hPipe);
                return 1;
            }

            // ��������
            if (!WriteFile(hPipe, &emp, sizeof(emp), &dwBytesWritten, NULL)) {
                printf("Failed to write to pipe, error code: %d\n", GetLastError());
                CloseHandle(hPipe);
                return 1;
            }

            // �ȴ���Ӧ
            if (!ReadFile(hPipe, &emp, sizeof(emp), &dwBytesRead, NULL)) {
                printf("Failed to read from pipe, error code: %d\n", GetLastError());
                CloseHandle(hPipe);
                return 1;
            }

            printf("Employee details: %d %s %.2f\n", emp.num, emp.name, emp.hours);
        }
        else if (choice == 2) {
            // ��ȡԱ����Ϣ
            printf("Enter employee number: ");
            scanf("%d", &emp.num);

            // ��������
            strcpy(data, "READ");
            if (!WriteFile(hPipe, data, strlen(data) + 1, &dwBytesWritten, NULL)) {
                printf("Failed to write to pipe, error code: %d\n", GetLastError());
                CloseHandle(hPipe);
                return 1;
            }

            // ��������
            if (!WriteFile(hPipe, &emp, sizeof(emp), &dwBytesWritten, NULL)) {
                printf("Failed to write to pipe, error code: %d\n", GetLastError());
                CloseHandle(hPipe);
            }
            // �ȴ���Ӧ
            if (!ReadFile(hPipe, &emp, sizeof(emp), &dwBytesRead, NULL)) {
                printf("Failed to read from pipe, error code: %d\n", GetLastError());
                CloseHandle(hPipe);
                return 1;
            }

            printf("Employee details: %d %s %.2f\n", emp.num, emp.name, emp.hours);
        }
        else if (choice == 3) {
            // �˳�
            strcpy(data, "EXIT");
            if (!WriteFile(hPipe, data, strlen(data) + 1, &dwBytesWritten, NULL)) {
                printf("Failed to write to pipe, error code: %d\n", GetLastError());
                CloseHandle(hPipe);
                return 1;
            }
            break;
        }
        else {
            printf("Invalid choice: %d\n", choice);
        }
    }

    // �رվ��
    CloseHandle(hPipe);

    return 0;
}