#include "httplib.h"
#include <iostream>
#include <nlohmann/json.hpp> // JSONライブラリ
#include <vector>
#include <string>
#include <mutex>
#include <sstream>

#include "common.h"
#include "include/models.hpp"
#include "include/TcpWrap.hpp"

using json = nlohmann::json;
// ─────────────────────────────────────────
// データ構造
// ─────────────────────────────────────────
struct Todo {
    int         id;
    std::string title;
    bool        done;
};
static std::mutex        g_mutex;

// ─────────────────────────────────────────
// main
// ─────────────────────────────────────────
int main() {
    httplib::Server svr;

    svr.Post("/api/update", [](const httplib::Request& req, httplib::Response& res) {
        std::lock_guard<std::mutex> lk(g_mutex);
        // 1. Content-Typeの確認
        if (req.get_header_value("Content-Type") != "application/json") {
            res.status = 400;
            res.set_content("Expected application/json", "text/plain");
            return;
        }
        try {
            // 2. JSONデコード (req.body をパース)
            json j = json::parse(req.body);

            // 3. データの取り出し (例: {"name": "Gopher", "id": 123})
            std::string action_name = j.at("action_name").get<std::string>();
            std::cout << "action_name=" << action_name << "\n";
            std::string table = j.at("table").get<std::string>();
            std::cout << "table=" << table << "\n";
            std::string sql = j.at("sql").get<std::string>();
            std::cout << "sql=" << sql << "\n";

            ActionReq actReq;
            actReq.action_name = action_name;
            actReq.table = table;
            actReq.sql = sql;
            json j1 = actReq; // 構造体を代入するだけ！
            std::string json_str = j1.dump();
            std::cout << json_str << std::endl;

            TcpWrap tLib("");
            std::string resp = tLib.update_message(json_str);

            res.status = 201;
            res.set_content(resp, "application/json");
            return;
        }
        catch (const std::exception& e) {
            // キーが存在しない場合など
            res.status = 500;
            res.set_content("Internal Server Error", "text/plain");
        }
    });

    svr.Post("/api/select", [](const httplib::Request& req, httplib::Response& res) {
        std::lock_guard<std::mutex> lk(g_mutex);
        // 1. Content-Typeの確認
        if (req.get_header_value("Content-Type") != "application/json") {
            res.status = 400;
            res.set_content("Expected application/json", "text/plain");
            return;
        }
        try {
            // 2. JSONデコード (req.body をパース)
            json j = json::parse(req.body);

            // 3. データの取り出し (例: {"name": "Gopher", "id": 123})
            std::string action_name = j.at("action_name").get<std::string>();
            std::cout << "action_name=" << action_name << "\n";
            std::string table = j.at("table").get<std::string>();
            std::cout << "table=" << table << "\n";
            std::string sql = j.at("sql").get<std::string>();
            std::cout << "sql=" << sql << "\n";

            ActionReq actReq;
            actReq.action_name = action_name;
            actReq.table = table;
            actReq.sql = sql;
            json j1 = actReq; // 構造体を代入するだけ！
            std::string json_str = j1.dump();
            std::cout << json_str << std::endl;

            TcpWrap tLib("");
            std::string resp = tLib.send_message(json_str);

            res.status = 200;
            res.set_content(resp, "application/json");
            return;
        }
        catch (const std::exception& e) {
            // キーが存在しない場合など
            res.status = 500;
            res.set_content("Internal Server Error", "text/plain");
        }
    });

    // ── 起動 ────────────────────────────────
    int port_no = 8000;
    std::cout << "TODO Server running on http://localhost:8000\n";

    svr.listen("0.0.0.0", port_no);
    return 0;
}
