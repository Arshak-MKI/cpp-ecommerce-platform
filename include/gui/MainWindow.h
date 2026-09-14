#pragma once

#include <QMainWindow>

#include "database/Database.h"
#include "repositories/ProductRepository.h"
#include "repositories/OrderRepository.h"

class QLineEdit;
class QComboBox;
class QListWidget;
class QPushButton;

class MainWindow : public QMainWindow
{
public:
    explicit MainWindow(QWidget* parent = nullptr);

private:
    void loadProducts();
    void filterProducts();
    void buySelectedProduct();

    Database database;
    ProductRepository productRepository;
    OrderRepository orderRepository;

    QLineEdit* searchEdit;
    QComboBox* categoryComboBox;
    QListWidget* productList;
    QPushButton* refreshButton;
    QPushButton* buyButton;

    int currentUserId = 1;
};
