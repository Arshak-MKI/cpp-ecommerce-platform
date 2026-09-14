
#pragma once

#include <string>
#include <vector>

#include "database/Database.h"
#include "models/Product.h"

class ProductRepository {
public:
    explicit ProductRepository(Database& database);

    std::vector<Product> getAll();

    std::vector<Product> search(
        const std::string& keyword
    );

    std::vector<Product> getByCategory(
        const std::string& category
    );

    bool addProduct(
        const Product& product
    );

    bool addProducts(
        const std::vector<Product>& products
    );

    std::vector<Product> getRecommendations(
        int userId,
        int limit = 5
    );

private:
    Database& database;
};
