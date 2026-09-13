
cmake_minimum_required(VERSION 3.20)

project(CppEcommercePlatform VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SQLite3 REQUIRED)

add_executable(cpp_ecommerce
    main.cpp

    src/database/Database.cpp
    src/models/Product.cpp
    src/repositories/ProductRepository.cpp
)

target_include_directories(cpp_ecommerce PRIVATE include)

target_link_libraries(cpp_ecommerce PRIVATE SQLite3::SQLite3)
