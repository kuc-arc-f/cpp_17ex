#include <windows.h>
#include <algorithm>
#include <ctime>
#include <shellapi.h>    // CommandLineToArgvW
#include <fcntl.h>       // _O_U16TEXT
#include <io.h>          // _setmode

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "nlohmann/json.hpp"

#pragma comment(lib, "shell32.lib")

using json = nlohmann::json;

const std::string FILE_PATH = "todos.json";

struct Todo {
    int id;
    std::string title;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Todo, id, title)

struct TodoData {
    int max_id;
    std::vector<Todo> items;
};

//
std::wstring StringToWString(const std::string& str)
{
    if (str.empty()) return L"";

    int size_needed = MultiByteToWideChar(
        CP_UTF8, 0,
        str.c_str(), (int)str.size(),
        NULL, 0
    );

    std::wstring wstr(size_needed, 0);

    MultiByteToWideChar(
        CP_UTF8, 0,
        str.c_str(), (int)str.size(),
        &wstr[0], size_needed
    );

    return wstr;
}

// std::wstring を UTF-8 の std::string に変換するヘルパー
std::string to_utf8(const std::wstring& wstr) {
    if (wstr.empty()) return std::string();
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
    std::string strTo(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
    return strTo;
}

class MyTodo {
private:
    std::string m_name;

public:
    explicit MyTodo(std::string str){
        // ① stdout をワイド文字モードに切り替え
        _setmode(_fileno(stdout), _O_U16TEXT);

        // ② コンソールの出力コードページを UTF-8 に設定
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);            
    }

    ~MyTodo() {}


    // JSONファイルからデータ読み込み
    TodoData load_data() {
        TodoData data{0, {}};
        std::ifstream file(FILE_PATH);
        if (file.is_open()) {
            try {
                json j = json::parse(file);
                if (j.contains("max_id") && j["max_id"].is_number()) {
                    data.max_id = j["max_id"].get<int>();
                }
                if (j.contains("items") && j["items"].is_array()) {
                    for (const auto& item : j["items"]) {
                        if (item.contains("id") && item.contains("title")) {
                            data.items.push_back({item["id"].get<int>(), item["title"].get<std::string>()});
                        }
                    }
                }
            } catch (const json::parse_error& e) {
                std::cerr << "JSON解析エラー: " << e.what() << "\n";
            }
        }
        return data;
    }

    // データをJSONファイルに保存
    void save_data(const TodoData& data) {
        json j;
        j["max_id"] = data.max_id;
        j["items"] = json::array();
        for (const auto& item : data.items) {
            j["items"].push_back({{"id", item.id}, {"title", item.title}});
        }
        std::ofstream file(FILE_PATH);
        if (file.is_open()) {
            file << j.dump(2); // インデント2で整形出力
            file.close();
        } else {
            std::cerr << "エラー: ファイルに書き込めません。\n";
        }
    }

    // TODO追加
    void add_todo(TodoData& data, const std::string& title) {
        data.max_id++;
        data.items.push_back({data.max_id, title});
        save_data(data);
    }

    // TODO一覧表示
    void list_todos(const TodoData& data) {
        if (data.items.empty()) {
            std::cout << "TODO none\n";
            return;
        }
        for (const auto& item : data.items) {
            //std::cout << "[" << item.id << "] " << item.title << "\n";
            std::wcout << L"[" << item.id << L"] " << StringToWString(item.title) << L"\n";
        }
    } 
    std::string list_json(const TodoData& data) {
        std::string ret = "";
        std::vector<Todo> todoItems;
        if (data.items.empty()) {
            std::cout << "TODO none\n";
            return "[]";
        }
        std::string out_str = "";
        for (const auto& item : data.items) {
            Todo row;
            row.id = item.id;
            row.title = item.title;
            todoItems.push_back(row);
        }
        json j1 = todoItems;
        std::string json_str = j1.dump();
        ret = json_str;
        return ret;
    }   


    // TODO削除
    void delete_todo(TodoData& data, int id) {
        auto it = std::remove_if(data.items.begin(), data.items.end(),
                                [id](const Todo& t) { return t.id == id; });
        if (it == data.items.end()) {
            //std::cout << "ID #" << id << " none \n";
            std::wcout << L"ID #" << id << L" none \n";
        } else {
            data.items.erase(it, data.items.end());
            save_data(data);
            //std::cout << "delete: #" << id << "\n";
            std::wcout << L"delete: #" << id << L"\n";
        }
    }

    void print_help() {
        std::cout << "使用方法: todo <コマンド> [引数]\n"
                << "コマンド:\n"
                << "  add <タイトル>      TODOを追加（複数単語可）\n"
                << "  list                TODO一覧を表示\n"
                << "  delete <ID>         IDを指定して削除\n"
                << "  help                ヘルプを表示\n";
    }

    void add_handler(std::string title){
        TodoData data = load_data();
        add_todo(data, title);
    }

    //void todo_list_handler(){
    std::string todo_list_handler(){
        std::string ret = "";
        try{
            TodoData data = load_data(); 
            ret = list_json(data); 
            return ret;
        } catch (const std::exception& e) {
            std::cout << "Error , main" << std::endl;
            return ret;
        }  
    }

    void todo_delete_handler(int id){
        try{
            TodoData data = load_data();
            delete_todo(data, id); 
        } catch (const std::exception& e) {
            std::cout << "Error , main" << std::endl;
        }  
    }
};

