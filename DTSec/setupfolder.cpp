#include <windows.h>
#include <iostream>
using namespace std;

void createFolder(const char* path) {
    if (CreateDirectoryA(path, NULL)) {}

    else {
        DWORD err = GetLastError();
        if (err == ERROR_ALREADY_EXISTS) {}
        else {
            cout << "Failed to create " << path << " (Error " << err << ")\n";
        }
    }
}

void createAllFolders() {
    createFolder("C:\\DTsec");
    createFolder("C:\\DTsec\\Python Scripts");
    createFolder("C:\\DTsec\\bashscripts");
}
