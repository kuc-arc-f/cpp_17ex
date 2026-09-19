#pragma once

#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#include <iostream>
#include <string>
#include <thread>
#include <atomic>

#pragma comment(lib, "ws2_32.lib")

// ポート番号
constexpr int    DEFAULT_PORT = 54000;
constexpr char   DEFAULT_ADDR[] = "127.0.0.1";
//constexpr int    BUFFER_SIZE  = 1024;
constexpr int    BUFFER_SIZE  = 4096;

// Winsock 初期化 RAII
class WinsockInit {
public:
    WinsockInit() {
        WSADATA wsaData;
        int ret = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (ret != 0) {
            std::cerr << "WSAStartup failed: " << ret << std::endl;
            ok_ = false;
        } else {
            ok_ = true;
        }
    }
    ~WinsockInit() {
        if (ok_) WSACleanup();
    }
    bool ok() const { return ok_; }
private:
    bool ok_ = false;
};