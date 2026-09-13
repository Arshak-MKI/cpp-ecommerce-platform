#pragma once

#include <vector>

#include "models/Product.h"
#include "database/Database.h"

class ProductRepository {
public:
    explicit ProductRepository(Database& database);

    std::vector<Product> getAll();

private:
    Database& database;
};

