# tcp_cl_1

 Version: 0.9.1

 date    : 2026/09/19
 
 update :

***

C++ , DB client TUI

* LLVM CLang use
* node 24
* Visual studio
* Windows

***
## Image

![img1](/images/tcp_cl_1.png)

***
### DB TCP Server

https://github.com/kuc-arc-f/cpp_mem_db_win

***
* Server info
* server_ip: 127.0.0.1
* port: 54000;

***
* C++ build
```
nmake all
```

* node start
```
pnpm i
npm run start
```

***
* TestCode example
* update
```
update:INSERT INTO temp (title) VALUES ('title_19_1');
```
***
* select
```
select:temp
select:temp:SELECT COUNT(*) FROM temp;
select:temp:SELECT * FROM temp ORDER BY id DESC LIMIT 3;
select:temp:SELECT id , title FROM temp ORDER BY id DESC LIMIT 3;
select:temp:SELECT id , title FROM temp WHERE id = 12295;
```
***
### blog

https://zenn.dev/link/comments/2f995d9bfb7de7


