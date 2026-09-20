#include <iostream>
#include <string>

#include "common.h"
#include "include/TcpClient.hpp"

// extern "C" でC言語リンクとし、__declspec(dllexport) でエクスポート
extern "C" {

    __declspec(dllexport) const char* send_tcp(const char* input)
    {
        std::string in_text = input;
        std::string resp = tcp_main(in_text);

        static std::string msg;
        msg = resp;
        return msg.c_str();        
    }    

    __declspec(dllexport) int add(int a, int b) {
        return a + b;
    }

    __declspec(dllexport) double multiply(double a, double b) {
        return a * b;
    }

    __declspec(dllexport) const char* get_message()
    {
        return "Hello World";
    }

    __declspec(dllexport) const char* greet(const char* name)
    {
        static std::string msg;
        msg = "Hello ";
        msg += name;
        return msg.c_str();
    }

}