
#pragma once

#include <string>
#include <vector>

#include "database/database.h"
#include "models/Order.h"

class OrderRepository {
public:
    explicit OrderRepository(Database& database);

    bool purchase(
        int userId,
        int productId,
        const std::string& deliveryAddress
    );

    std::vector<Order> getOrdersByUser(
        int userId
    );

private:
    Database& database;
};
