# client_2

 Version: 0.9.1

 date    : 2026/09/19
 
 update :

***

C++ TCP client , CLI

* LLVM CLang use
* Visual studio
* Windows

***
* build
```
clang++ -std=c++17 client.cpp -o client.exe -lws2_32
```

* start
```
.\client.exe
```
***
* TestCode
* update
```
{"action_name": "update", "table":"temp","sql":"INSERT INTO temp (title) VALUES ('title1');"}
```
* select
```
{"action_name": "select", "table":"temp","sql":"SELECT * FROM temp;"}
```
***
* TestCode : node - INSERT
* ./test1.js

```
node test1.js
```
***
