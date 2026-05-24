#include <iostream>
#include <filesystem>
#include <vector>
#include <string>

namespace fs = std::filesystem;
using namespace std;

wstring chooseFolder(const wstring& path, const wstring& prompt) {
    vector<wstring> folders;
    for (const auto& entry : fs::directory_iterator(path)) {
        if (entry.is_directory()) {
            folders.push_back(entry.path().filename().wstring());
        }
    }

    if (folders.empty()) {
        wcout << L"No folders found in: " << path << L"\n";
        return L"";
    }

    if (folders.size() > 1) {
        wcout << prompt << L"\n";
        for (size_t i = 0; i < folders.size(); i++) {
            wcout << i << L": " << folders[i] << L"\n";
        }
        wcout << L"Enter choice: ";
        int choice;
        wcin >> choice;
        if (choice >= 0 && choice < (int)folders.size()) {
            return (fs::path(path) / folders[choice]).wstring();
        }
        else {
            wcout << L"Invalid choice.\n";
            return L"";
        }
    }
    else {
        // Only one folder, auto-select
        return (fs::path(path) / folders[0]).wstring();
    }
}