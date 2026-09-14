#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "database/Database.h"

#include "models/Product.h"
#include "models/User.h"
#include "models/Order.h"

#include "repositories/ProductRepository.h"
#include "repositories/UserRepository.h"
#include "repositories/OrderRepository.h"

#include "services/ProductService.h"


void printProducts(const std::vector<Product>& products)
{
    for (const Product& product : products)
    {
        std::cout
            << product.getName()
            << " | "
            << product.getPrice()
            << " | "
            << product.getCategory()
            << " | "
            << (product.isAvailable()
                    ? "Available"
                    : "Unavailable")
            << std::endl;
    }
}


std::vector<Product> createSeedProducts()
{
    std::vector<Product> products;

    const int totalProducts = 1000;

    const std::vector<std::string> categories =
    {
        "Phones",
        "Irons",
        "Home Appliances"
    };

    products.reserve(totalProducts);

    for (int i = 1; i <= totalProducts; ++i)
    {
        const int categoryIndex =
            (i - 1) %
            static_cast<int>(categories.size());

        const std::string& category =
            categories[categoryIndex];

        const std::string name =
            category +
            " Product " +
            std::to_string(i);

        const double price =
            50.0 +
            (i % 500) * 10.0;

        const std::string description =
            "Product number " +
            std::to_string(i) +
            " in category " +
            category;

        const std::string imagePath =
            "images/product_" +
            std::to_string(i) +
            ".jpg";

        products.emplace_back(
            0,
            name,
            price,
            description,
            category,
            imagePath,
            true
        );
    }

    return products;
}


int main()
{
    // -------------------------------------------------
    // DATABASE
    // -------------------------------------------------

    Database database("database/shop.db");

    if (!database.open())
    {
        return 1;
    }

    std::cout
        << "Database opened successfully!"
        << std::endl;


    // -------------------------------------------------
    // PRODUCTS TABLE
    // -------------------------------------------------

    const std::string create_products_table = R"(
        CREATE TABLE IF NOT EXISTS products (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL UNIQUE,
            price REAL NOT NULL,
            description TEXT,
            category TEXT,
            image_path TEXT,
            is_available INTEGER NOT NULL DEFAULT 1
        );
    )";

    if (!database.execute(create_products_table))
    {
        return 1;
    }


    // -------------------------------------------------
    // USERS TABLE
    // -------------------------------------------------

    const std::string create_users_table = R"(
        CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT NOT NULL UNIQUE,
            email TEXT NOT NULL UNIQUE,
            password TEXT NOT NULL
        );
    )";

    if (!database.execute(create_users_table))
    {
        return 1;
    }


    // -------------------------------------------------
    // ORDERS TABLE
    // -------------------------------------------------

    const std::string create_orders_table = R"(
        CREATE TABLE IF NOT EXISTS orders (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            user_id INTEGER NOT NULL,
            product_id INTEGER NOT NULL,
            delivery_address TEXT NOT NULL,
            created_at TEXT DEFAULT CURRENT_TIMESTAMP,
            FOREIGN KEY (user_id) REFERENCES users(id),
            FOREIGN KEY (product_id) REFERENCES products(id)
        );
    )";

    if (!database.execute(create_orders_table))
    {
        return 1;
    }


    // -------------------------------------------------
    // REPOSITORIES
    // -------------------------------------------------

    ProductRepository productRepository(database);
    UserRepository userRepository(database);
    OrderRepository orderRepository(database);


    // -------------------------------------------------
    // SERVICE
    // -------------------------------------------------

    ProductService productService(productRepository);


    // -------------------------------------------------
    // SEED PRODUCTS
    // -------------------------------------------------

    std::vector<Product> seedProducts =
        createSeedProducts();

    if (!productRepository.addProducts(seedProducts))
    {
        std::cerr
            << "Failed to seed products."
            << std::endl;

        return 1;
    }


    // -------------------------------------------------
    // MAIN USER LOGIN
    // -------------------------------------------------

    auto user =
        userRepository.login(
            "arshak@example.com",
            "123456"
        );

    if (!user.has_value())
    {
        if (!userRepository.registerUser(
                "Arshak",
                "arshak@example.com",
                "123456"))
        {
            std::cerr
                << "Failed to register main user."
                << std::endl;

            return 1;
        }

        user =
            userRepository.login(
                "arshak@example.com",
                "123456"
            );
    }

    if (!user.has_value())
    {
        std::cerr
            << "Failed to login main user."
            << std::endl;

        return 1;
    }


    std::cout
        << "\n=== LOGIN SUCCESS ==="
        << std::endl;

    std::cout
        << "User ID: "
        << user->getId()
        << std::endl;

    std::cout
        << "Username: "
        << user->getUsername()
        << std::endl;

    std::cout
        << "Email: "
        << user->getEmail()
        << std::endl;


    // -------------------------------------------------
    // ALL PRODUCTS
    // -------------------------------------------------

    std::vector<Product> products =
        productRepository.getAll();

    std::cout
        << "\n=== DATASET ==="
        << std::endl;

    std::cout
        << "Products found: "
        << products.size()
        << std::endl;


    // -------------------------------------------------
    // SEARCH
    // -------------------------------------------------

    std::cout
        << "\n=== SEARCH: Product 100 ==="
        << std::endl;

    std::vector<Product> searchResults =
        productRepository.search(
            "Product 100"
        );

    printProducts(searchResults);


    // -------------------------------------------------
    // CATEGORY FILTER
    // -------------------------------------------------

    std::cout
        << "\n=== CATEGORY: Phones ==="
        << std::endl;

    std::vector<Product> phoneProducts =
        productRepository.getByCategory(
            "Phones"
        );

    std::cout
        << "Phones found: "
        << phoneProducts.size()
        << std::endl;


    // -------------------------------------------------
    // PRICE ASCENDING
    // -------------------------------------------------

    std::cout
        << "\n=== PRICE: ASCENDING ==="
        << std::endl;

    std::vector<Product> ascendingProducts =
        productService.getProductsSortedByPrice(
            true
        );

    for (std::size_t i = 0;
         i < 10 &&
         i < ascendingProducts.size();
         ++i)
    {
        std::cout
            << ascendingProducts[i].getName()
            << " | "
            << ascendingProducts[i].getPrice()
            << std::endl;
    }


    // -------------------------------------------------
    // PRICE DESCENDING
    // -------------------------------------------------

    std::cout
        << "\n=== PRICE: DESCENDING ==="
        << std::endl;

    std::vector<Product> descendingProducts =
        productService.getProductsSortedByPrice(
            false
        );

    for (std::size_t i = 0;
         i < 10 &&
         i < descendingProducts.size();
         ++i)
    {
        std::cout
            << descendingProducts[i].getName()
            << " | "
            << descendingProducts[i].getPrice()
            << std::endl;
    }


    // -------------------------------------------------
    // NORMAL PURCHASE
    // -------------------------------------------------

    std::cout
        << "\n=== PURCHASE TEST ==="
        << std::endl;

    int normalPurchaseProductId = -1;

    for (const Product& product : products)
    {
        if (product.isAvailable() &&
            product.getCategory() == "Phones")
        {
            normalPurchaseProductId =
                product.getId();

            break;
        }
    }

    if (normalPurchaseProductId != -1)
    {
        bool purchaseResult =
            orderRepository.purchase(
                user->getId(),
                normalPurchaseProductId,
                "Yerevan, Armenia"
            );

        if (purchaseResult)
        {
            std::cout
                << "Purchase successful!"
                << std::endl;
        }
        else
        {
            std::cout
                << "Purchase failed."
                << std::endl;
        }
    }


    // -------------------------------------------------
    // USER ORDERS
    // -------------------------------------------------

    std::cout
        << "\n=== USER ORDERS ==="
        << std::endl;

    std::vector<Order> orders =
        orderRepository.getOrdersByUser(
            user->getId()
        );

    if (orders.empty())
    {
        std::cout
            << "No orders found."
            << std::endl;
    }
    else
    {
        for (const Order& order :
             orders)
        {
            std::cout
                << "Order ID: "
                << order.getId()
                << " | Product ID: "
                << order.getProductId()
                << " | Address: "
                << order.getDeliveryAddress()
                << std::endl;
        }
    }


    // -------------------------------------------------
    // USER PROFILE
    // -------------------------------------------------

    std::cout
        << "\n=== USER PROFILE ==="
        << std::endl;

    std::cout
        << "User ID: "
        << user->getId()
        << std::endl;

    std::cout
        << "Username: "
        << user->getUsername()
        << std::endl;

    std::cout
        << "Email: "
        << user->getEmail()
        << std::endl;

    std::cout
        << "Orders count: "
        << orders.size()
        << std::endl;


    // -------------------------------------------------
    // RECOMMENDATIONS
    // -------------------------------------------------

    std::cout
        << "\n=== RECOMMENDATIONS ==="
        << std::endl;

    std::vector<Product> recommendations =
        productRepository.getRecommendations(
            user->getId(),
            5
        );

    if (recommendations.empty())
    {
        std::cout
            << "No recommendations found."
            << std::endl;
    }
    else
    {
        printProducts(recommendations);
    }


    // -------------------------------------------------
    // CONCURRENT PURCHASE TEST
    // -------------------------------------------------

    std::cout
        << "\n=== CONCURRENT PURCHASE TEST ==="
        << std::endl;

    std::vector<Product> freshProducts =
        productRepository.getAll();

    int testProductId = -1;

    for (const Product& product :
         freshProducts)
    {
        if (product.isAvailable())
        {
            testProductId =
                product.getId();

            break;
        }
    }

    if (testProductId == -1)
    {
        std::cout
            << "No available product for "
               "concurrency test."
            << std::endl;
    }
    else
    {
        std::cout
            << "Testing product ID: "
            << testProductId
            << std::endl;


        Database databaseA(
            "database/shop.db"
        );

        Database databaseB(
            "database/shop.db"
        );


        if (!databaseA.open() ||
            !databaseB.open())
        {
            std::cerr
                << "Failed to open test databases."
                << std::endl;

            return 1;
        }


        UserRepository userRepositoryA(
            databaseA
        );

        UserRepository userRepositoryB(
            databaseB
        );


        auto userA =
            userRepositoryA.login(
                "usera@example.com",
                "passwordA"
            );

        if (!userA.has_value())
        {
            userRepositoryA.registerUser(
                "UserA",
                "usera@example.com",
                "passwordA"
            );

            userA =
                userRepositoryA.login(
                    "usera@example.com",
                    "passwordA"
                );
        }


        auto userB =
            userRepositoryB.login(
                "userb@example.com",
                "passwordB"
            );

        if (!userB.has_value())
        {
            userRepositoryB.registerUser(
                "UserB",
                "userb@example.com",
                "passwordB"
            );

            userB =
                userRepositoryB.login(
                    "userb@example.com",
                    "passwordB"
                );
        }


        if (!userA.has_value() ||
            !userB.has_value())
        {
            std::cerr
                << "Failed to login test users."
                << std::endl;

            return 1;
        }


        OrderRepository orderRepositoryA(
            databaseA
        );

        OrderRepository orderRepositoryB(
            databaseB
        );


        bool resultA = false;
        bool resultB = false;


        std::thread threadA([&]()
        {
            resultA =
                orderRepositoryA.purchase(
                    userA->getId(),
                    testProductId,
                    "Yerevan - User A"
                );
        });


        std::thread threadB([&]()
        {
            resultB =
                orderRepositoryB.purchase(
                    userB->getId(),
                    testProductId,
                    "Yerevan - User B"
                );
        });


        threadA.join();
        threadB.join();


        std::cout
            << "User A result: "
            << (resultA
                    ? "SUCCESS"
                    : "FAILED")
            << std::endl;

        std::cout
            << "User B result: "
            << (resultB
                    ? "SUCCESS"
                    : "FAILED")
            << std::endl;


        if (resultA != resultB)
        {
            std::cout
                << "Concurrency test PASSED: "
                   "only one user purchased "
                   "the product."
                << std::endl;
        }
        else
        {
            std::cout
                << "Concurrency test FAILED."
                << std::endl;
        }
    }


    return 0;
}

