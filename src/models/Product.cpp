
#include "models/Product.h"

Product::Product(
    int id,
    const std::string& name,
    double price,
    const std::string& description,
    const std::string& category,
    const std::string& imagePath,
    bool isAvailable
)
    : id(id),
      name(name),
      price(price),
      description(description),
      category(category),
      imagePath(imagePath),
      available(isAvailable) {
}

int Product::getId() const {
    return id;
}

const std::string& Product::getName() const {
    return name;
}

double Product::getPrice() const {
    return price;
}

const std::string& Product::getDescription() const {
    return description;
}

const std::string& Product::getCategory() const {
    return category;
}

const std::string& Product::getImagePath() const {
    return imagePath;
}

bool Product::isAvailable() const {
    return available;
}
