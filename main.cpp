#include <iostream>

#include "database/Database.h"

int main() {
    Database database("database/shop.db");

    if (!database.open()) {
        return 1;
    }

    std::cout << "Database opened successfully!" << std::endl;

    const std::string create_products_table = R"(
        CREATE TABLE IF NOT EXISTS products (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL UNIQUE,
            price REAL NOT NULL,
            description TEXT,
            category TEXT,
            image_path TEXT,
            is_available INTEGER NOT NULL DEFAULT 1
        );
    )";

    if (!database.execute(create_products_table)) {
        return 1;
    }

    std::cout << "Products table created successfully!" << std::endl;

    return 0;
}
