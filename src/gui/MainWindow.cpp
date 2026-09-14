
#include "gui/MainWindow.h"

#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

#include <set>
#include <sstream>
#include <string>
#include <vector>

#include "models/Product.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      database("database/shop.db"),
      productRepository(database),
      orderRepository(database),
      searchEdit(nullptr),
      categoryComboBox(nullptr),
      productList(nullptr),
      refreshButton(nullptr),
      buyButton(nullptr)
{
    setWindowTitle("C++ E-Commerce Platform");
    resize(1100, 700);

    if (!database.open())
    {
        QMessageBox::critical(
            this,
            "Database Error",
            "Failed to open database."
        );

        return;
    }

    QWidget* centralWidget =
        new QWidget(this);

    QVBoxLayout* mainLayout =
        new QVBoxLayout(centralWidget);

    QLabel* title =
        new QLabel(
            "C++ E-Commerce Platform"
        );

    title->setAlignment(
        Qt::AlignCenter
    );

    title->setStyleSheet(
        "font-size: 28px;"
        "font-weight: bold;"
        "margin: 10px;"
    );

    mainLayout->addWidget(title);

    // Search and category
    QHBoxLayout* filterLayout =
        new QHBoxLayout();

    searchEdit =
        new QLineEdit();

    searchEdit->setPlaceholderText(
        "Search products..."
    );

    categoryComboBox =
        new QComboBox();

    refreshButton =
        new QPushButton("Refresh");

    filterLayout->addWidget(searchEdit);
    filterLayout->addWidget(categoryComboBox);
    filterLayout->addWidget(refreshButton);

    mainLayout->addLayout(filterLayout);

    // Products
    productList =
        new QListWidget();

    productList->setStyleSheet(
        "QListWidget {"
        "    font-size: 16px;"
        "}"
        "QListWidget::item {"
        "    padding: 10px;"
        "}"
    );

    mainLayout->addWidget(productList);

    // Buy button
    buyButton =
        new QPushButton(
            "Buy Selected Product"
        );

    buyButton->setMinimumHeight(45);

    buyButton->setStyleSheet(
        "font-size: 17px;"
        "font-weight: bold;"
    );

    mainLayout->addWidget(buyButton);

    setCentralWidget(centralWidget);

    // Signals
    connect(
        searchEdit,
        &QLineEdit::textChanged,
        this,
        [this]()
        {
            filterProducts();
        }
    );

    connect(
        categoryComboBox,
        &QComboBox::currentTextChanged,
        this,
        [this]()
        {
            filterProducts();
        }
    );

    connect(
        refreshButton,
        &QPushButton::clicked,
        this,
        [this]()
        {
            loadProducts();
        }
    );

    connect(
        buyButton,
        &QPushButton::clicked,
        this,
        [this]()
        {
            buySelectedProduct();
        }
    );

    loadProducts();
}


// =====================================================
// LOAD PRODUCTS
// =====================================================

void MainWindow::loadProducts()
{
    productList->clear();

    categoryComboBox->blockSignals(true);

    categoryComboBox->clear();

    categoryComboBox->addItem(
        "All Categories"
    );

    std::vector<Product> products =
        productRepository.getAll();

    std::set<std::string> categories;

    for (const Product& product : products)
    {
        categories.insert(
            product.getCategory()
        );
    }

    for (const std::string& category :
         categories)
    {
        categoryComboBox->addItem(
            QString::fromStdString(category)
        );
    }

    categoryComboBox->blockSignals(false);

    for (const Product& product : products)
    {
        std::ostringstream itemText;

        itemText
            << product.getName()
            << "    |    $"
            << product.getPrice()
            << "    |    "
            << product.getCategory()
            << "    |    "
            << (
                product.isAvailable()
                    ? "Available"
                    : "Unavailable"
            );

        QListWidgetItem* item =
            new QListWidgetItem(
                QString::fromStdString(
                    itemText.str()
                )
            );

        item->setData(
            Qt::UserRole,
            product.getId()
        );

        productList->addItem(item);
    }
}


// =====================================================
// FILTER PRODUCTS
// =====================================================

void MainWindow::filterProducts()
{
    const QString searchText =
        searchEdit->text().trimmed();

    const QString category =
        categoryComboBox->currentText();

    for (int i = 0;
         i < productList->count();
         ++i)
    {
        QListWidgetItem* item =
            productList->item(i);

        const QString text =
            item->text();

        bool matchesSearch = true;
        bool matchesCategory = true;

        if (!searchText.isEmpty())
        {
            matchesSearch =
                text.contains(
                    searchText,
                    Qt::CaseInsensitive
                );
        }

        if (category !=
            "All Categories")
        {
            matchesCategory =
                text.contains(
                    " | " +
                    category +
                    " | ",
                    Qt::CaseInsensitive
                );
        }

        item->setHidden(
            !(matchesSearch &&
              matchesCategory)
        );
    }
}


// =====================================================
// BUY SELECTED PRODUCT
// =====================================================

void MainWindow::buySelectedProduct()
{
    QListWidgetItem* item =
        productList->currentItem();

    if (item == nullptr)
    {
        QMessageBox::warning(
            this,
            "No Product Selected",
            "Please select a product first."
        );

        return;
    }

    if (!item->text().contains(
            "Available",
            Qt::CaseInsensitive))
    {
        QMessageBox::warning(
            this,
            "Product Unavailable",
            "This product is no longer available."
        );

        return;
    }

    const int productId =
        item->data(
            Qt::UserRole
        ).toInt();

    const bool result =
        orderRepository.purchase(
            currentUserId,
            productId,
            "Yerevan, Armenia"
        );

    if (result)
    {
        QMessageBox::information(
            this,
            "Purchase Successful",
            "The product was purchased successfully."
        );
    }
    else
    {
        QMessageBox::warning(
            this,
            "Purchase Failed",
            "The product could not be purchased."
        );
    }

    loadProducts();
}

