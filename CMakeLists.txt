
cmake_minimum_required(VERSION 3.20)

project(CppEcommercePlatform VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SQLite3 REQUIRED)
find_package(Threads REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(cpp_ecommerce
    main.cpp

    # GUI
    src/gui/MainWindow.cpp

    # Database
    src/database/database.cpp

    # Models
    src/models/Product.cpp
    src/models/User.cpp
    src/models/Order.cpp

    # Repositories
    src/repositories/ProductRepository.cpp
    src/repositories/UserRepository.cpp
    src/repositories/OrderRepository.cpp

    # Services
    src/services/ProductService.cpp
)

target_include_directories(cpp_ecommerce PRIVATE
    include
)

target_link_libraries(cpp_ecommerce PRIVATE
    SQLite3::SQLite3
    Threads::Threads
    Qt6::Widgets
)
