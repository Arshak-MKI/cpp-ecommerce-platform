
#include "database/Database.h"

#include <iostream>
#include <sqlite3.h>

Database::Database(const std::string& path)
    : db(nullptr), path(path) {
}

Database::~Database() {
    if (db != nullptr) {
        sqlite3_close(db);
    }
}

bool Database::open() {
    int result = sqlite3_open(path.c_str(), &db);

    if (result != SQLITE_OK) {
        std::cerr << "Database opening failed: "
                  << sqlite3_errmsg(db) << std::endl;

        return false;
    }

    return true;
}

bool Database::execute(const std::string& sql) {
    char* error_message = nullptr;

    int result = sqlite3_exec(
        db,
        sql.c_str(),
        nullptr,
        nullptr,
        &error_message
    );

    if (result != SQLITE_OK) {
        std::cerr << "SQL execution failed: "
                  << error_message << std::endl;

        sqlite3_free(error_message);
        return false;
    }

    return true;
}

bool Database::query(
    const std::string& sql,
    int (*callback)(void*, int, char**, char**),
    void* data
) {
    char* error_message = nullptr;

    int result = sqlite3_exec(
        db,
        sql.c_str(),
        callback,
        data,
        &error_message
    );

    if (result != SQLITE_OK) {
        std::cerr << "SQL query failed: "
                  << error_message << std::endl;

        sqlite3_free(error_message);
        return false;
    }

    return true;
}
