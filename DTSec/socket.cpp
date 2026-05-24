#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <iphlpapi.h>
#include <ws2tcpip.h>
#include <wininet.h>
#include <iostream>
#include <string>
#include <icmpapi.h>
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "wininet.lib")
std::string getLocalIPv4() {
    ULONG bufLen = 15000;
    IP_ADAPTER_ADDRESSES* adapter = (IP_ADAPTER_ADDRESSES*)malloc(bufLen);

    std::string ipStr = "";

    if (GetAdaptersAddresses(AF_INET, GAA_FLAG_INCLUDE_PREFIX, NULL, adapter, &bufLen) == NO_ERROR) {
        for (IP_ADAPTER_ADDRESSES* a = adapter; a; a = a->Next) {
            if (a->OperStatus == IfOperStatusUp) { // aktif interface
                for (IP_ADAPTER_UNICAST_ADDRESS* ua = a->FirstUnicastAddress; ua; ua = ua->Next) {
                    SOCKADDR_IN* sa_in = (SOCKADDR_IN*)ua->Address.lpSockaddr;
                    char ip[INET_ADDRSTRLEN];
                    inet_ntop(AF_INET, &(sa_in->sin_addr), ip, sizeof(ip));
                    ipStr = ip;
                    break; // ilk aktif IP’yi al
                }
            }
            if (!ipStr.empty()) break;
        }
    }
    free(adapter);
    return ipStr;
}

bool pingHost(const std::string& ip) {
    std::string cmd = "ping -n 1 -w 200 " + ip + " >nul";
    return system(cmd.c_str()) == 0;
}

void scanLAN(const std::string& subnet) {
    for (int i = 1; i < 255; i++) {
        std::string ip = subnet + "." + std::to_string(i);
        if (pingHost(ip)) {
            std::cout << "Active: " << ip << "\n";
        }
    }
}

void sendPing(const std::string& ip) {
    HANDLE hIcmp = IcmpCreateFile();
    char sendData[] = "PingData";
    BYTE replyBuffer[1024];
    DWORD replySize = sizeof(replyBuffer);

    if (IcmpSendEcho(hIcmp, inet_addr(ip.c_str()), sendData, sizeof(sendData),
        NULL, replyBuffer, replySize, 1000)) {
        std::cout << "Ping success: " << ip << "\n";
    }
    else {
        std::cout << "Ping fail: " << ip << "\n";
    }
    IcmpCloseHandle(hIcmp);
}

std::string getWANIP() {
    HINTERNET hInternet = InternetOpen(L"WANIP", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
    HINTERNET hConnect = InternetOpenUrl(hInternet, L"http://api.ipify.org", NULL, 0, INTERNET_FLAG_RELOAD, 0);

    char buffer[128];
    DWORD bytesRead;
    std::string ip;

    if (hConnect) {
        while (InternetReadFile(hConnect, buffer, sizeof(buffer) - 1, &bytesRead) && bytesRead) {
            buffer[bytesRead] = 0;
            ip += buffer;
        }
        InternetCloseHandle(hConnect);
    }
    InternetCloseHandle(hInternet);
    return ip;
}

void renewDHCP() {
    ULONG bufLen = 0;
    if (GetAdaptersInfo(NULL, &bufLen) != ERROR_BUFFER_OVERFLOW || bufLen == 0) {
        printf("GetAdaptersInfo failed to get buffer size.\n");
        return;
    }

    IP_ADAPTER_INFO* pAdapter = (IP_ADAPTER_INFO*)malloc(bufLen);
    if (!pAdapter) {
        printf("Memory allocation failed for pAdapter.\n");
        return;
    }

    if (GetAdaptersInfo(pAdapter, &bufLen) == NO_ERROR) {
        for (IP_ADAPTER_INFO* a = pAdapter; a; a = a->Next) {
            ULONG outBufLen = 0;
            if (GetInterfaceInfo(NULL, &outBufLen) != ERROR_INSUFFICIENT_BUFFER || outBufLen == 0) {
                printf("GetInterfaceInfo failed to get buffer size.\n");
                continue;
            }

            IP_INTERFACE_INFO* pInfo = (IP_INTERFACE_INFO*)malloc(outBufLen);
            if (!pInfo) {
                printf("Memory allocation failed for pInfo.\n");
                continue;
            }

            if (GetInterfaceInfo(pInfo, &outBufLen) == NO_ERROR) {
                if (pInfo->NumAdapters > 0) {
                    DWORD ret = IpReleaseAddress(&pInfo->Adapter[0]);
                    if (ret == NO_ERROR) {
                        printf("IP released successfully.\n");
                    }
                    else {
                        printf("IpReleaseAddress failed: %lu\n", ret);
                    }

                    ret = IpRenewAddress(&pInfo->Adapter[0]);
                    if (ret == NO_ERROR) {
                        printf("IP renewed successfully.\n");
                    }
                    else {
                        printf("IpRenewAddress failed: %lu\n", ret);
                    }
                }
                else {
                    printf("No adapters found in pInfo.\n");
                }
            }
            else {
                printf("GetInterfaceInfo failed.\n");
            }

            free(pInfo);
        }
    }
    else {
        printf("GetAdaptersInfo failed.\n");
    }

    free(pAdapter);
}
