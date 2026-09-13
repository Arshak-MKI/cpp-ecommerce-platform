
#pragma once

#include <string>

class Product {
public:
    Product(
        int id,
        const std::string& name,
        double price,
        const std::string& description,
        const std::string& category,
        const std::string& imagePath,
        bool isAvailable
    );

    int getId() const;
    const std::string& getName() const;
    double getPrice() const;
    const std::string& getDescription() const;
    const std::string& getCategory() const;
    const std::string& getImagePath() const;
    bool isAvailable() const;

private:
    int id;
    std::string name;
    double price;
    std::string description;
    std::string category;
    std::string imagePath;
    bool available;
};

