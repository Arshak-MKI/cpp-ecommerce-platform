#pragma once

#include <vector>

#include "models/Product.h"
#include "repositories/ProductRepository.h"

class ProductService {
public:
    explicit ProductService(ProductRepository& repository);

    std::vector<Product> getProductsSortedByPrice(
        bool ascending
    );

private:
    ProductRepository& repository;
};