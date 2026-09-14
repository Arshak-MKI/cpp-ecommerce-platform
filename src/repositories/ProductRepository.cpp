
#include "repositories/ProductRepository.h"

#include <string>
#include <vector>

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

std::vector<Product> ProductRepository::search(
    const std::string& keyword
) {
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

    const std::string pattern = "%" + keyword + "%";

    const std::string sql =
        "SELECT "
        "id, name, price, description, category, image_path, is_available "
        "FROM products "
        "WHERE name LIKE '" + pattern + "' "
        "OR description LIKE '" + pattern + "' "
        "OR category LIKE '" + pattern + "' "
        "ORDER BY id;";

    if (!database.query(sql, callback, &products)) {
        return {};
    }

    return products;
}

std::vector<Product> ProductRepository::getByCategory(
    const std::string& category
) {
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

    const std::string sql =
        "SELECT "
        "id, name, price, description, category, image_path, is_available "
        "FROM products "
        "WHERE category = '" + category + "' "
        "ORDER BY id;";

    if (!database.query(sql, callback, &products)) {
        return {};
    }

    return products;
}

bool ProductRepository::addProduct(const Product& product) {
    const std::string sql =
        "INSERT OR IGNORE INTO products "
        "(name, price, description, category, image_path, is_available) "
        "VALUES ('" +
        product.getName() + "', " +
        std::to_string(product.getPrice()) + ", '" +
        product.getDescription() + "', '" +
        product.getCategory() + "', '" +
        product.getImagePath() + "', " +
        std::to_string(product.isAvailable() ? 1 : 0) +
        ");";

    return database.execute(sql);
}

bool ProductRepository::addProducts(
    const std::vector<Product>& products
) {
    if (!database.execute("BEGIN TRANSACTION;")) {
        return false;
    }

    for (const Product& product : products) {
        if (!addProduct(product)) {
            database.execute("ROLLBACK;");
            return false;
        }
    }

    if (!database.execute("COMMIT;")) {
        database.execute("ROLLBACK;");
        return false;
    }

    return true;
}
std::vector<Product> ProductRepository::getRecommendations(
    int userId,
    int limit
) {
    std::vector<Product> recommendations;

    auto callback = [](
        void* data,
        int argc,
        char** argv,
        char**
    ) -> int {

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
            argv[6] &&
            std::stoi(argv[6]) == 1
        );

        products->push_back(product);

        return 0;
    };

    const std::string sql =
        "SELECT DISTINCT "
        "p.id, "
        "p.name, "
        "p.price, "
        "p.description, "
        "p.category, "
        "p.image_path, "
        "p.is_available "
        "FROM products p "
        "WHERE p.is_available = 1 "
        "AND p.category IN ("
        "    SELECT DISTINCT p2.category "
        "    FROM products p2 "
        "    INNER JOIN orders o "
        "    ON p2.id = o.product_id "
        "    WHERE o.user_id = " +
        std::to_string(userId) +
        ") "
        "AND p.id NOT IN ("
        "    SELECT product_id "
        "    FROM orders "
        "    WHERE user_id = " +
        std::to_string(userId) +
        ") "
        "ORDER BY p.id "
        "LIMIT " +
        std::to_string(limit) +
        ";";

    if (!database.query(
            sql,
            callback,
            &recommendations
        )) {
        return {};
    }

    return recommendations;
}


