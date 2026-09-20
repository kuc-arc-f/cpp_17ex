# test_api_4

 Version: 0.9.1

 date    : 2026/09/19
 
 update :

***

C++ cpp-httplib  , API Server

* LLVM CLang
* cpp-httplib
* Windows

***
### related DB Server

https://github.com/kuc-arc-f/cpp_mem_db_win

***
### related

https://github.com/yhirose/cpp-httplib

***
* LIB add
```
.\vcpkg install nlohmann-json:x64-windows
```

***
* DB SERVER info
* SERVER_IP: 127.0.0.1
* TCP_PORT: 54000

***
* build
```
.\build.bat
```

* (option) clang++ build
```
clang++ -std=c++17 -O2 -I./include ^
-I/prog/vcpkg/installed/x64-windows/include ^
main.cpp -o api_server.exe

```

* start API
```
.\api_server.exe
```
***
* Test-code
* test1.js : INSERT
```
node test1.js
```
* test2.js : SELECT
```
node test2.js
```

***
