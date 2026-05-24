#pragma once
#include <string>
std::string getLocalIPv4();
void scanLAN(const std::string& subnet);
bool pingHost(const std::string& ip);
void sendPing(const std::string& ip);
std::string getWANIP();
void renewDHCP();