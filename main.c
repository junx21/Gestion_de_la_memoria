#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SHARED_MEM_SIZE 4096
#define SHARED_MEM_NAME "Local\\SharedMemory"

int main(int argc, char *argv[]) {
    HANDLE hMapFile;
    LPSTR pBuf;

    // Verificar si es el proceso hijo
    if (argc > 1 && strcmp(argv[1], "child") == 0) {
        // Código del proceso hijo
        hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, SHARED_MEM_NAME);
        if (hMapFile == NULL) {
            printf("Could not open file mapping object (%d).\n", GetLastError());
            return 1;
        }

        pBuf = (LPSTR)MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, SHARED_MEM_SIZE);
        if (pBuf == NULL) {
            printf("Could not map view of file (%d).\n", GetLastError());
            CloseHandle(hMapFile);
            return 1;
        }

        printf("Child reads: %s\n", pBuf);

        UnmapViewOfFile(pBuf);
        CloseHandle(hMapFile);
    } else {
        // Código del proceso padre
        hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, SHARED_MEM_SIZE, SHARED_MEM_NAME);
        if (hMapFile == NULL) {
            printf("Could not create file mapping object (%d).\n", GetLastError());
            return 1;
        }

        pBuf = (LPSTR)MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, SHARED_MEM_SIZE);
        if (pBuf == NULL) {
            printf("Could not map view of file (%d).\n", GetLastError());
            CloseHandle(hMapFile);
            return 1;
        }

        // Escribir en la memoria compartida
        strcpy(pBuf, "Hello, child process!");

        // Crear proceso hijo
        STARTUPINFO si;
        PROCESS_INFORMATION pi;
        char cmdLine[MAX_PATH];

        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));

        // Preparar la línea de comando para el proceso hijo
        snprintf(cmdLine, sizeof(cmdLine), "%s child", argv[0]);

        // Crear el proceso hijo
        if (!CreateProcess(NULL, cmdLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
            printf("CreateProcess failed (%d).\n", GetLastError());
            UnmapViewOfFile(pBuf);
            CloseHandle(hMapFile);
            return 1;
        }

        // Esperar a que el proceso hijo termine
        WaitForSingleObject(pi.hProcess, INFINITE);

        // Cerrar handles del proceso e hilo
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        UnmapViewOfFile(pBuf);
        CloseHandle(hMapFile);
    }

    return 0;
}