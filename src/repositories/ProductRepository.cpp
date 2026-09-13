#include "repositories/ProductRepository.h"

ProductRepository::ProductRepository(Database& database)
    : database(database) {
}

std::vector<Product> ProductRepository::getAll() {
    std::vector<Product> products;

    auto callback = [](void* data, int argc, char** argv, char**) -> int {
        auto* products =
            static_cast<std::vector<Product>*>(data);

        if (argc < 7) {
            return 1;
        }

        Product product(
            std::stoi(argv[0]),
            argv[1] ? argv[1] : "",
            std::stod(argv[2]),
            argv[3] ? argv[3] : "",
            argv[4] ? argv[4] : "",
            argv[5] ? argv[5] : "",
            argv[6] && std::stoi(argv[6]) == 1
        );

        products->push_back(product);

        return 0;
    };

    const std::string sql = R"(
        SELECT
            id,
            name,
            price,
            description,
            category,
            image_path,
            is_available
        FROM products
        ORDER BY id;
    )";

    if (!database.query(sql, callback, &products)) {
        return {};
    }

    return products;
}
