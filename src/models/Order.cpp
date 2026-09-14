
#include "models/Order.h"

Order::Order(
    int id,
    int userId,
    int productId,
    const std::string& deliveryAddress
)
    : id(id),
      userId(userId),
      productId(productId),
      deliveryAddress(deliveryAddress) {
}

int Order::getId() const {
    return id;
}

int Order::getUserId() const {
    return userId;
}

int Order::getProductId() const {
    return productId;
}

const std::string& Order::getDeliveryAddress() const {
    return deliveryAddress;
}

