#include "includeset.h"
#include "tokenizer.h"
#include "help.h"
#include <shellapi.h>
#include <Shlwapi.h>
#include <filesystem>
#include "pathtranslator.h"
#include "setupfolders.h"
#include "socket.h"
#include "wslfinder.h"
namespace fs = std::filesystem;
USESTD

void listFiles(const std::string& path, string extension) {
    for (const auto& entry : fs::directory_iterator(path)) {
        std::string name = entry.path().filename().string();

        if (name.find(extension) != std::string::npos) {
            std::cout << name << "\n";
        }
    }
}

int main(int argc, char** argv)
{
    createAllFolders();
    string username;
    cout << "Session Username:\n";
    cin >> username;
    string cmd;
    system("cls");
    cout << ANSI_CYAN << "DTSec - A simple security toolkit for Windows" << ANSI_RESET << "\n";
    cout << ANSI_BLUE << "Build Date: " << __DATE__ << " " << __TIME__ << ANSI_RESET << "\n";
    _help();
    cout << ANSI_GREEN << "Welcome, " << username << "!" << ANSI_RESET << "\n";
    while (true) {
        if (!getline(cin, cmd)) break;
        if (cmd.empty()) continue;

        auto token = tokenize(cmd);
        if (token.empty()) continue;

        const std::string& command = token[0];

        if (command == "help") {
            _help();
        }
        else if (command == "exit") {
            cout << "Exiting...\n";
            return 0;
        }
        else if (command == "dwsl") {
            if (token.size() != 2) {
                cout << "Usage: dwsl <distro>\n";
            }
            else {
                std::string distro = token[1];
                std::string psCmd = "powershell -Command \"wsl --install -d " + distro + "\"";
                system(psCmd.c_str());
            }
        }
        else if (command == "distrolist") {
            cout << "Available WSL distros:\n"
                << "Ubuntu\nDebian\nKali-Linux\nArchLinux\nopenSUSE-Leap-15.6\n"
                << "openSUSE-Tumbleweed\nFedora-Remix\nAlpine\nOracleLinux\n";
        }
        else if (command == "pymodules") {
            string path = "C:\\DTsec\\Python Scripts";
            if (!fs::exists(path)) {
                cout << "Directory does not exist: " << path <<"\n";
            }
            else if (fs::is_empty(path)) {
                cout << "No scripts in " << path <<"\n";
            }
            else {
                listFiles(path, ".py");
            }
            }
        else if (command == "shmodules") {
            std::string path = "C:\\DTsec\\bashscripts";

            if (!fs::exists(path)) {
                std::cout << "Directory does not exist: " << path << "\n";
            }
            else if (fs::is_empty(path)) {
                std::cout << "No scripts in " << path << "\n";
            }
            else {
                listFiles(path, ".sh");
            }
        }

        else if (command == "clear") {
            system("cls");
        }
        else if (command == "lan") {
            cout << getLocalIPv4() << "\n";
        }
        else if (command == "wan" || command == "whoami") {
            cout << getWANIP() << "\n";
        }
        else if (command == "runwsl") {
            std::string psCmd = "powershell -Command \"wsl\"";
            system(psCmd.c_str());
        }
        else if (command == "sysinfo") {
            system("sysinfo");
        }
        else if (command == "lanchange") {
            renewDHCP();
        }
        else if (command == "cleardns") {

            system("ipconfig /flushdns");
        }
        else if (command == "resetnet") {
            system("netsh int ip reset");
            system("netsh winsock reset");
        }
        else if (command == "wslrunpy") {
            string scriptname;
            wstring wsl = L"\\\\wsl.localhost";

            wstring distroPath = chooseFolder(wsl, L"Choose distro:");
            if (distroPath.empty()) continue;

            wstring homePath = (fs::path(distroPath) / L"home").wstring();

            wstring userPath = chooseFolder(homePath, L"Choose user:");
            if (userPath.empty()) continue;

            cout << "Write the python script name with extension, e.g. 'script.py'\n"
                << "PUT SCRIPTS IN C:\\DTsec\\Python Scripts\n";
            cin >> scriptname;

            string srcPath = "C:\\DTsec\\Python Scripts\\" + scriptname;

            string dstPath = string(userPath.begin(), userPath.end()) + "\\" + scriptname;
            string copyCmd = "powershell Copy-Item \"" + srcPath + "\" \"" + dstPath + "\"";
            string distroName = fs::path(distroPath).filename().string();
            string userName = fs::path(userPath).filename().string();
            string linuxPath = "/home/" + userName + "/" + scriptname;

            string runCmd = "powershell wsl -d " + distroName + " python3 " + linuxPath;

            system(copyCmd.c_str());
            system(runCmd.c_str());

        }
        
        else {
            cout << "Unknown command: " << command << "\n";
        }
    }
}