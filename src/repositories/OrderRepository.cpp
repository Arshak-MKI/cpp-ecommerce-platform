
#include "repositories/OrderRepository.h"

#include <iostream>
#include <string>
#include <vector>

OrderRepository::OrderRepository(Database& database)
    : database(database) {
}

bool OrderRepository::purchase(
    int userId,
    int productId,
    const std::string& deliveryAddress
) {
    std::cout
        << "[Purchase] User "
        << userId
        << " trying product "
        << productId
        << std::endl;

    if (!database.execute("BEGIN IMMEDIATE TRANSACTION;")) {
        std::cout
            << "[Purchase] Failed to begin transaction."
            << std::endl;

        return false;
    }

    const std::string updateProduct =
        "UPDATE products "
        "SET is_available = 0 "
        "WHERE id = " +
        std::to_string(productId) +
        " AND is_available = 1;";

    if (!database.execute(updateProduct)) {
        std::cout
            << "[Purchase] Failed to update product."
            << std::endl;

        database.execute("ROLLBACK;");
        return false;
    }

    struct ChangesData {
        int changes = 0;
    };

    ChangesData data;

    auto callback = [](
        void* rawData,
        int argc,
        char** argv,
        char**
    ) -> int {
        auto* data =
            static_cast<ChangesData*>(rawData);

        if (argc > 0 && argv[0] != nullptr) {
            data->changes = std::stoi(argv[0]);
        }

        return 0;
    };

    if (!database.query(
            "SELECT changes();",
            callback,
            &data)) {

        std::cout
            << "[Purchase] Failed to read affected rows."
            << std::endl;

        database.execute("ROLLBACK;");
        return false;
    }

    if (data.changes != 1) {
        std::cout
            << "[Purchase] Product "
            << productId
            << " is already unavailable."
            << std::endl;

        database.execute("ROLLBACK;");
        return false;
    }

    const std::string insertOrder =
        "INSERT INTO orders "
        "(user_id, product_id, delivery_address) "
        "VALUES (" +
        std::to_string(userId) + ", " +
        std::to_string(productId) + ", '" +
        deliveryAddress + "');";

    if (!database.execute(insertOrder)) {
        std::cout
            << "[Purchase] Failed to create order."
            << std::endl;

        database.execute("ROLLBACK;");
        return false;
    }

    if (!database.execute("COMMIT;")) {
        std::cout
            << "[Purchase] Failed to commit transaction."
            << std::endl;

        database.execute("ROLLBACK;");
        return false;
    }

    std::cout
        << "[Purchase] User "
        << userId
        << " successfully purchased product "
        << productId
        << std::endl;

    return true;
}

std::vector<Order> OrderRepository::getOrdersByUser(
    int userId
) {
    std::vector<Order> orders;

    auto callback = [](
        void* data,
        int argc,
        char** argv,
        char**
    ) -> int {
        auto* orders =
            static_cast<std::vector<Order>*>(data);

        if (argc < 4) {
            return 1;
        }

        Order order(
            std::stoi(argv[0]),
            std::stoi(argv[1]),
            std::stoi(argv[2]),
            argv[3] ? argv[3] : ""
        );

        orders->push_back(order);

        return 0;
    };

    const std::string sql =
        "SELECT "
        "id, user_id, product_id, delivery_address "
        "FROM orders "
        "WHERE user_id = " +
        std::to_string(userId) +
        " ORDER BY id DESC;";

    if (!database.query(
            sql,
            callback,
            &orders)) {
        return {};
    }

    return orders;
}
