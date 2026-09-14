#include "services/ProductService.h"

#include <algorithm>

ProductService::ProductService(ProductRepository& repository)
    : repository(repository) {
}

std::vector<Product> ProductService::getProductsSortedByPrice(
    bool ascending
) {
    std::vector<Product> products = repository.getAll();

    std::sort(
        products.begin(),
        products.end(),
        [ascending](const Product& a, const Product& b) {
            if (ascending) {
                return a.getPrice() < b.getPrice();
            }

            return a.getPrice() > b.getPrice();
        }
    );

    return products;
}