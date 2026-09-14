# Online Market

A C++17 desktop e-commerce application built with **Qt 6**, **SQLite**, **CMake**, and **MinGW/MSYS2**.

The project provides a simple online marketplace where users can register and log in, browse products, search and filter by category, view product details and images, purchase products, view their orders, and receive product recommendations.

## Features

### User Authentication

* User registration
* User login
* Email and password authentication
* User profile information

### Product Management

* 1000+ seeded products
* Multiple product categories
* Product names, prices, descriptions, images, and availability
* Product list with scrolling
* Product details panel

### Search and Filtering

* Product search by name
* Category filtering
* Refresh products

### Shopping

* Select a product
* View product information
* Purchase available products
* Purchase success/failure notifications

### Orders

The **My Orders** section displays:

* Product name
* Product price
* Purchase date and time
* Scrollable order history

### Recommendations

The application provides product recommendations based on the user's previous purchases and product categories.

### Concurrency

The backend includes a concurrent purchase test using two users and two database connections. The test verifies that only one user can successfully purchase the same available product.

## Technologies

* **C++17**
* **Qt 6**
* **SQLite**
* **CMake**
* **MinGW / MSYS2 UCRT64**
* **Ninja**

## Project Structure

```text
cpp-ecommerce-platform/
│
├── CMakeLists.txt
├── README.md
├── main.cpp
├── backend_demo.cpp
│
├── database/
│   └── shop.db
│
├── include/
│   ├── database/
│   │   └── Database.h
│   │
│   ├── gui/
│   │   └── MainWindow.h
│   │
│   ├── models/
│   │   ├── Product.h
│   │   ├── User.h
│   │   └── Order.h
│   │
│   ├── repositories/
│   │   ├── ProductRepository.h
│   │   ├── UserRepository.h
│   │   └── OrderRepository.h
│   │
│   └── services/
│       └── ProductService.h
│
├── src/
│   ├── database/
│   │   └── database.cpp
│   │
│   ├── gui/
│   │   └── MainWindow.cpp
│   │
│   ├── models/
│   │   ├── Product.cpp
│   │   ├── User.cpp
│   │   └── Order.cpp
│   │
│   ├── repositories/
│   │   ├── ProductRepository.cpp
│   │   ├── UserRepository.cpp
│   │   └── OrderRepository.cpp
│   │
│   └── services/
│       └── ProductService.cpp
│
└── images/
```

## Database

The application uses SQLite for persistent data storage.

### Products

```text
id
name
price
description
category
image_path
is_available
```

### Users

```text
id
username
email
password
```

### Orders

```text
id
user_id
product_id
delivery_address
created_at
```

## Build Requirements

Install the following:

* Qt 6.x with MinGW
* MSYS2 UCRT64
* CMake
* Ninja
* SQLite3 development libraries

For the current development environment, Qt is located at:

```text
C:\Qt\6.11.2\mingw_64
```

## Build on Windows

Open **MSYS2 UCRT64** and go to the project directory:

```bash
cd /c/Users/DELL/Documents/cpp-ecommerce-platform
```

Configure the project:

```bash
cmake -S . -B build -DCMAKE_PREFIX_PATH=/c/Qt/6.11.2/mingw_64
```

Build:

```bash
cmake --build build
```

## Run

Before running the Qt application, make sure the Qt runtime libraries are available:

```bash
export PATH="/ucrt64/bin:/usr/bin:/c/Qt/6.11.2/mingw_64/bin"
```

Then run:

```bash
./build/cpp_ecommerce.exe
```

## Using the Application

### 1. Login

Enter your email and password.

### 2. Register

New users can select **Register**, enter:

```text
Username
Email
Password
```

and create an account.

### 3. Browse Products

The main window displays the available products.

Only the product name is shown in the list. Selecting a product displays its:

* Image
* Price
* Category
* Availability
* Description

### 4. Search

Use the search field to find products by name.

### 5. Category

Use the category dropdown to filter products.

### 6. Buy

Select an available product and click:

```text
Buy Selected Product
```

### 7. Recommendations

Click:

```text
Show Recommendations
```

to see recommended products.

### 8. Orders

Click:

```text
My Orders
```

to view the user's purchase history. The order list is scrollable.

### 9. Profile

Click:

```text
My Profile
```

to view the current user's information.

## Example Categories

The application supports categories such as:

```text
Phones
Laptops
Tablets
Televisions
Headphones
Cameras
Irons
Microwaves
Refrigerators
Washing Machines
Vacuum Cleaners
Kitchen Appliances
Home Appliances
Gaming
Accessories
```

## Architecture

The project follows a layered structure:

```text
GUI
 │
 ▼
Repositories
 │
 ▼
Database
 │
 ▼
SQLite
```

Models represent application data, repositories handle database operations, services provide business logic, and the Qt GUI provides the user interface.

## Security Note

This project is intended as an educational C++ e-commerce application. Passwords are currently stored using the project's basic authentication implementation and should be securely hashed in a production application.

## Future Improvements

Possible future improvements include:

* Secure password hashing
* Shopping cart
* Product quantity management
* Product images bundled with the application
* Payment integration
* Delivery tracking
* Admin dashboard
* Better recommendation algorithms
* Persistent login sessions
* Improved UI styling

## Author

**Arshak Eranosyan**

## License

This project is for educational and demonstration purposes.