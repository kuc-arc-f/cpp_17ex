#pragma once
#include <iostream>
#include "TcpClient.hpp"

class TcpWrap {
private:
    std::string m_name;

public:
    explicit TcpWrap(std::string str){}
    ~TcpWrap() {}


    std::string send_message(std::string input) 
    {
        std::string ret = "";
        WinsockInit wsa;
        if (!wsa.ok()) return ret;

        // 1. ソケット作成
        SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (sock == INVALID_SOCKET) {
            std::cerr << "socket() failed: " << WSAGetLastError() << std::endl;
            return ret;
        }

        // 2. サーバーアドレス設定
        sockaddr_in serverAddr{};
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port   = htons(DEFAULT_PORT);
        inet_pton(AF_INET, DEFAULT_ADDR, &serverAddr.sin_addr);

        // 3. connect
        if (connect(sock, reinterpret_cast<sockaddr*>(&serverAddr),
                    sizeof(serverAddr)) == SOCKET_ERROR) {
            std::cerr << "connect() failed: " << WSAGetLastError() << std::endl;
            closesocket(sock);
            return ret;
        }
        std::cout << "[Client] Connected to " << DEFAULT_ADDR
                  << ":" << DEFAULT_PORT << std::endl;

        TcpClient client("");
        std::string line = input;
        if (!client.sendAll(sock, line)) return ret;

        std::string recvData = client.recvAll(sock, static_cast<int>(line.size()));
        if (recvData.empty()) {
            std::cout << "[Client] Server closed connection.\n";
            return ret;
        }
        std::cout << "[Client] Recv: " << recvData
                    << "  (" << recvData.size() << "/" << line.size()
                    << " bytes)" << std::endl;

        if (recvData.size() < line.size()) {
            std::cout << "[Client] Warning: 受信が不完全なままサーバーが切断\n";
            return ret;
        }
        ret = recvData;

        closesocket(sock);
        std::cout << "[Client] Disconnected.\n";
        return ret;
    } 

};


 

