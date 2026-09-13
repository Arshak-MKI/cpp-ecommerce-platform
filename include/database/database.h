
#pragma once

#include <string>

struct sqlite3;

class Database {
public:
    explicit Database(const std::string& path);
    ~Database();

    bool open();
    bool execute(const std::string& sql);

    bool query(
        const std::string& sql,
        int (*callback)(void*, int, char**, char**),
        void* data = nullptr
    );

private:
    sqlite3* db;
    std::string path;
};
