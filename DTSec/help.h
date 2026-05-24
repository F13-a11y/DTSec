#define USESTD using namespace std;

#pragma once
#include <iostream>
#include <string>
#include "color_utils.h"
#include "tokenizer.h"
USESTD
void _help() {
	cout << "Available commands:\n";
	cout << "  help - Show this help message\n";
	cout << "  exit - Exit the program\n";
	cout << "  dwsl <distro> - Download WSL\n";
	cout << "  pymodules - Show installed Python modules in C:/DTSec/Python Scripts\n";
	cout << "  shmodules - Show installed Shell modules in C:/DTSec/Shell Scripts\n";
	cout << "  pushsh <path> - Push a shell script to WSL\n";
	cout << "  pushpy <path> - Push a Python script to WSL\n";
	cout << "  runsh <path> - Run a shell script in WSL\n";
	cout << "  runpy <path> - Run a Python script in Windows Environment\n";
	cout << "  wslrunpy <path> - Run a Python script in WSL\n";
	cout << "  lan - Show local IP address\n";
	cout << "  wan/whoami - Show public IP address\n";
	cout << "  sysinfo - Show system information\n";
	cout << "  lanchange - Change local IP address randomly (Requires admin privileges)\n";
	cout << "  resetnet - Reset network settings\n";
	cout << "  clear - Clear the console\n";
	cout << "  cleardns - Clears dns cache\n";
	cout << "  distrolist - List of the distros avaliable for WSL\n";
	cout << "  runwsl - Opens WSL\n";
}