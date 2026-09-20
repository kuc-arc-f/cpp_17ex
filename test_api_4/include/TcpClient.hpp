#pragma once
#include <iostream>

class TcpClient {
private:
    std::string m_name;

public:
    explicit TcpClient(std::string str){}
    ~TcpClient() {}

    // ------------------------------------------------------------------
    // sendAll : データ全体を送信し切るまで send() を繰り返す
    //   ※ TCP の send() は 1 回で全て送れるとは限らない(部分送信)ため、
    //     送信済みバイト数を積み上げて、残りを送り直す
    // ------------------------------------------------------------------
    bool sendAll(SOCKET sock, const std::string& data) {
        int total = 0;                          // 送信済みバイト数
        int len   = static_cast<int>(data.size());

        while (total < len) {
            int n = send(sock, data.c_str() + total, len - total, 0);
            if (n == SOCKET_ERROR) {
                std::cerr << "send() failed: " << WSAGetLastError() << std::endl;
                return false;
            }
            total += n;                         // 今回送れた分を加算
        }
        return true;
    }

    // ------------------------------------------------------------------
    // recvAll : 期待する文字数(expectLen)に達するまで recv() を繰り返す
    //   ※ 1 回の recv() は「今届いている分」しか返さない。
    //     長いデータは途中までしか届かないことがあるため、
    //     受信データを動的バッファに積み上げ、全部揃うまで待つ。
    //   recv() が 0 を返す = サーバー切断 / 負の値 = エラー → 打ち切り
    //   前提: サーバーは送信した文字列をそのまま返す(エコー)サーバー
    // ------------------------------------------------------------------
    std::string recvAll(SOCKET sock, int expectLen) {
        std::string data;                       // 受信データを溜める動的バッファ
        char buf[BUFFER_SIZE];                  // 1回の受信バッファ(1024)

        while (true) {
            fd_set read_fds;
            FD_ZERO(&read_fds);
            FD_SET(sock, &read_fds);

            // 200ms以内に追加データが来なければメッセージ終端とみなす
            // 継続的なストリームではなくリクエスト-レスポンス型を想定
            struct timeval tv;
            tv.tv_sec = 0;
            tv.tv_usec = 200 * 1000; // 200ms
            int sel = select(sock + 1, &read_fds, nullptr, nullptr, &tv);
            if (sel < 0) {
                if (errno == EINTR) continue;
                std::cerr << "selectエラー: " << strerror(errno) << std::endl;
                break;
            }  
            if (sel == 0) {
                // タイムアウト = 追加データなし、受信完了
                break;
            }
            if (!FD_ISSET(sock, &read_fds)) {
                break;
            }
            int n = recv(sock, buf, sizeof(buf), 0);
            if (n <= 0) break;                  // 切断・エラーで打ち切り
            data.append(buf, n);                // 受信分をバッファに積み上げる
        }
        return data;
    } 

};


    std::string tcp_main(std::string input) 
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

