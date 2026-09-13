
#include <iostream>
#include <vector>

#include "database/Database.h"
#include "models/Product.h"
#include "repositories/ProductRepository.h"

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

    const std::string insert_product = R"(
        INSERT OR IGNORE INTO products (
            name,
            price,
            description,
            category,
            image_path
        )
        VALUES (
            'iPhone 17',
            999.99,
            'Apple smartphone with modern features',
            'Phones',
            'images/iphone17.jpg'
        );
    )";

    if (!database.execute(insert_product)) {
        return 1;
    }

    ProductRepository productRepository(database);

    std::vector<Product> products = productRepository.getAll();

    std::cout << "\nProducts found: "
              << products.size()
              << std::endl;

    for (const Product& product : products) {
        std::cout << "\n--- Product ---" << std::endl;
        std::cout << "ID: "
                  << product.getId()
                  << std::endl;

        std::cout << "Name: "
                  << product.getName()
                  << std::endl;

        std::cout << "Price: "
                  << product.getPrice()
                  << std::endl;

        std::cout << "Description: "
                  << product.getDescription()
                  << std::endl;

        std::cout << "Category: "
                  << product.getCategory()
                  << std::endl;

        std::cout << "Image: "
                  << product.getImagePath()
                  << std::endl;

        std::cout << "Available: "
                  << (product.isAvailable() ? "Yes" : "No")
                  << std::endl;
    }

    return 0;
}
