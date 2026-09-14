
#pragma once

#include <string>

class Order {
public:
    Order(
        int id,
        int userId,
        int productId,
        const std::string& deliveryAddress
    );

    int getId() const;
    int getUserId() const;
    int getProductId() const;
    const std::string& getDeliveryAddress() const;

private:
    int id;
    int userId;
    int productId;
    std::string deliveryAddress;
};

