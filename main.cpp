
#include <QApplication>
#include <QComboBox>
#include <QDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QPushButton>
#include <QPixmap>
#include <QSize>
#include <QSplitter>
#include <QVBoxLayout>
#include <QWidget>
#include <QAbstractItemView>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include "database/Database.h"

#include "models/Product.h"
#include "models/User.h"
#include "models/Order.h"

#include "repositories/ProductRepository.h"
#include "repositories/UserRepository.h"
#include "repositories/OrderRepository.h"


int main(int argc, char* argv[])
{
    QApplication app(argc, argv);


    // =================================================
    // DATABASE
    // =================================================

    Database database("database/shop.db");

    if (!database.open())
    {
        QMessageBox::critical(
            nullptr,
            "Database Error",
            "Failed to open database."
        );

        return 1;
    }


    // =================================================
    // REPOSITORIES
    // =================================================

    ProductRepository productRepository(
        database
    );

    UserRepository userRepository(
        database
    );

    OrderRepository orderRepository(
        database
    );


    // =================================================
    // LOGIN
    // =================================================

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
            QMessageBox::critical(
                nullptr,
                "Login Error",
                "Could not create user."
            );

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
        QMessageBox::critical(
            nullptr,
            "Login Error",
            "Could not login user."
        );

        return 1;
    }


    const int currentUserId =
        user->getId();


    // =================================================
    // MAIN WINDOW
    // =================================================

    QWidget window;

    window.setWindowTitle(
        "C++ E-Commerce Platform"
    );

    window.resize(
        1250,
        800
    );


    QVBoxLayout* mainLayout =
        new QVBoxLayout(
            &window
        );


    // =================================================
    // TITLE
    // =================================================

    QLabel* title =
        new QLabel(
            "C++ E-Commerce Platform"
        );

    title->setAlignment(
        Qt::AlignCenter
    );

    title->setStyleSheet(
        "font-size: 30px;"
        "font-weight: bold;"
        "padding: 15px;"
    );

    mainLayout->addWidget(
        title
    );


    // =================================================
    // USER
    // =================================================

    QLabel* userLabel =
        new QLabel(
            QString(
                "Logged in as: %1 | %2"
            )
                .arg(
                    QString::fromStdString(
                        user->getUsername()
                    )
                )
                .arg(
                    QString::fromStdString(
                        user->getEmail()
                    )
                )
        );

    userLabel->setAlignment(
        Qt::AlignCenter
    );

    mainLayout->addWidget(
        userLabel
    );


    // =================================================
    // SEARCH + CATEGORY
    // =================================================

    QHBoxLayout* filterLayout =
        new QHBoxLayout();


    QLineEdit* searchEdit =
        new QLineEdit();


    searchEdit->setPlaceholderText(
        "Search products..."
    );


    QComboBox* categoryComboBox =
        new QComboBox();


    QPushButton* refreshButton =
        new QPushButton(
            "Refresh"
        );


    filterLayout->addWidget(
        searchEdit
    );

    filterLayout->addWidget(
        categoryComboBox
    );

    filterLayout->addWidget(
        refreshButton
    );


    mainLayout->addLayout(
        filterLayout
    );


    // =================================================
    // PRODUCT AREA
    // =================================================

    QSplitter* productSplitter =
        new QSplitter(
            Qt::Horizontal
        );


    // -------------------------------------------------
    // PRODUCT LIST
    // -------------------------------------------------

    QListWidget* productList =
        new QListWidget();


    productList->setIconSize(
        QSize(80, 80)
    );


    productList->setSpacing(
        6
    );


    productList->setStyleSheet(
        "QListWidget {"
        "    font-size: 16px;"
        "}"
        "QListWidget::item {"
        "    padding: 8px;"
        "}"
    );


    productSplitter->addWidget(
        productList
    );


    // -------------------------------------------------
    // PRODUCT DETAILS
    // -------------------------------------------------

    QWidget* detailWidget =
        new QWidget();


    QVBoxLayout* detailLayout =
        new QVBoxLayout(
            detailWidget
        );


    QLabel* imageLabel =
        new QLabel(
            "Select a product"
        );


    imageLabel->setAlignment(
        Qt::AlignCenter
    );


    imageLabel->setMinimumSize(
        320,
        280
    );


    imageLabel->setStyleSheet(
        "border: 1px solid #cccccc;"
        "background: #f5f5f5;"
        "font-size: 18px;"
        "padding: 10px;"
    );


    QLabel* nameLabel =
        new QLabel(
            "Product"
        );


    nameLabel->setStyleSheet(
        "font-size: 22px;"
        "font-weight: bold;"
    );


    QLabel* priceLabel =
        new QLabel(
            "Price: -"
        );


    priceLabel->setStyleSheet(
        "font-size: 20px;"
    );


    QLabel* categoryLabel =
        new QLabel(
            "Category: -"
        );


    QLabel* availabilityLabel =
        new QLabel(
            "Availability: -"
        );


    QLabel* descriptionLabel =
        new QLabel(
            "Description: -"
        );


    descriptionLabel->setWordWrap(
        true
    );


    detailLayout->addWidget(
        imageLabel
    );


    detailLayout->addWidget(
        nameLabel
    );


    detailLayout->addWidget(
        priceLabel
    );


    detailLayout->addWidget(
        categoryLabel
    );


    detailLayout->addWidget(
        availabilityLabel
    );


    detailLayout->addWidget(
        descriptionLabel
    );


    detailLayout->addStretch();


    productSplitter->addWidget(
        detailWidget
    );


    productSplitter->setStretchFactor(
        0,
        3
    );


    productSplitter->setStretchFactor(
        1,
        2
    );


    mainLayout->addWidget(
        productSplitter
    );


    // =================================================
    // ACTION BUTTONS
    // =================================================

    QHBoxLayout* actionLayout =
        new QHBoxLayout();


    QPushButton* buyButton =
        new QPushButton(
            "Buy Selected Product"
        );


    QPushButton* recommendationButton =
        new QPushButton(
            "Show Recommendations"
        );


    QPushButton* ordersButton =
        new QPushButton(
            "My Orders"
        );


    QPushButton* profileButton =
        new QPushButton(
            "My Profile"
        );


    actionLayout->addWidget(
        buyButton
    );


    actionLayout->addWidget(
        recommendationButton
    );


    actionLayout->addWidget(
        ordersButton
    );


    actionLayout->addWidget(
        profileButton
    );


    mainLayout->addLayout(
        actionLayout
    );


    // =================================================
    // LOAD PRODUCTS
    // =================================================

    auto loadProducts =
        [&]()
    {
        productList->clear();


        categoryComboBox->blockSignals(
            true
        );


        categoryComboBox->clear();


        categoryComboBox->addItem(
            "All Categories"
        );


        const std::vector<Product>
            products =
                productRepository.getAll();


        std::set<std::string>
            categories;


        for (const Product& product :
             products)
        {
            categories.insert(
                product.getCategory()
            );
        }


        for (const std::string& category :
             categories)
        {
            categoryComboBox->addItem(
                QString::fromStdString(
                    category
                )
            );
        }


        categoryComboBox->blockSignals(
            false
        );


        for (const Product& product :
             products)
        {
           QListWidgetItem* item =
    new QListWidgetItem(
        QString::fromStdString(
            product.getName()
        )
    );

            // Product ID
            item->setData(
                Qt::UserRole,
                product.getId()
            );


            // Category
            item->setData(
                Qt::UserRole + 1,
                QString::fromStdString(
                    product.getCategory()
                )
            );


            // Availability
            item->setData(
                Qt::UserRole + 2,
                product.isAvailable()
            );


            // Name
            item->setData(
                Qt::UserRole + 3,
                QString::fromStdString(
                    product.getName()
                )
            );


            // Price
            item->setData(
                Qt::UserRole + 4,
                product.getPrice()
            );


            // Description
            item->setData(
                Qt::UserRole + 5,
                QString::fromStdString(
                    product.getDescription()
                )
            );


            // Image path
            item->setData(
                Qt::UserRole + 6,
                QString::fromStdString(
                    product.getImagePath()
                )
            );


            // Product icon
            const QString imagePath =
                QString::fromStdString(
                    product.getImagePath()
                );


            QPixmap pixmap(
                imagePath
            );


            if (!pixmap.isNull())
            {
                item->setIcon(
                    QIcon(
                        pixmap.scaled(
                            80,
                            80,
                            Qt::KeepAspectRatio,
                            Qt::SmoothTransformation
                        )
                    )
                );
            }


            productList->addItem(
                item
            );
        }


        if (productList->count() > 0)
        {
            productList->setCurrentRow(
                0
            );
        }
    };


    // =================================================
    // FILTER PRODUCTS
    // =================================================

    auto filterProducts =
        [&]()
    {
        const QString searchText =
            searchEdit
                ->text()
                .trimmed();


        const QString selectedCategory =
            categoryComboBox
                ->currentText();


        for (int i = 0;
             i < productList->count();
             ++i)
        {
            QListWidgetItem* item =
                productList->item(i);


            const QString productName =
                item->data(
                    Qt::UserRole + 3
                ).toString();


            const QString category =
                item->data(
                    Qt::UserRole + 1
                ).toString();


            const bool searchMatches =
                searchText.isEmpty() ||
                productName.contains(
                    searchText,
                    Qt::CaseInsensitive
                );


            const bool categoryMatches =
                selectedCategory ==
                    "All Categories" ||
                category ==
                    selectedCategory;


            item->setHidden(
                !(searchMatches &&
                  categoryMatches)
            );
        }
    };


    // =================================================
    // PRODUCT DETAILS
    // =================================================

    auto showProductDetails =
        [&]()
    {
        QListWidgetItem* item =
            productList->currentItem();


        if (item == nullptr)
        {
            return;
        }


        const QString name =
            item->data(
                Qt::UserRole + 3
            ).toString();


        const double price =
            item->data(
                Qt::UserRole + 4
            ).toDouble();


        const QString category =
            item->data(
                Qt::UserRole + 1
            ).toString();


        const bool available =
            item->data(
                Qt::UserRole + 2
            ).toBool();


        const QString description =
            item->data(
                Qt::UserRole + 5
            ).toString();


        const QString imagePath =
            item->data(
                Qt::UserRole + 6
            ).toString();


        nameLabel->setText(
            name
        );


        priceLabel->setText(
            QString(
                "Price: $%1"
            )
                .arg(
                    price,
                    0,
                    'f',
                    2
                )
        );


        categoryLabel->setText(
            "Category: " +
            category
        );


        availabilityLabel->setText(
            available
                ? "Availability: Available"
                : "Availability: Unavailable"
        );


        descriptionLabel->setText(
            "Description: " +
            description
        );


        QPixmap pixmap(
            imagePath
        );


        if (!pixmap.isNull())
        {
            imageLabel->setPixmap(
                pixmap.scaled(
                    320,
                    280,
                    Qt::KeepAspectRatio,
                    Qt::SmoothTransformation
                )
            );


            imageLabel->setText(
                QString()
            );
        }
        else
        {
            imageLabel->setPixmap(
                QPixmap()
            );


            imageLabel->setText(
                "Image not found\n\n" +
                imagePath
            );
        }
    };


    // =================================================
    // SEARCH EVENT
    // =================================================

    QObject::connect(
        searchEdit,
        &QLineEdit::textChanged,
        [&]()
        {
            filterProducts();
        }
    );


    // =================================================
    // CATEGORY EVENT
    // =================================================

    QObject::connect(
        categoryComboBox,
        &QComboBox::currentTextChanged,
        [&]()
        {
            filterProducts();
        }
    );


    // =================================================
    // PRODUCT SELECTION
    // =================================================

    QObject::connect(
        productList,
        &QListWidget::currentItemChanged,
        [&](
            QListWidgetItem*,
            QListWidgetItem*
        )
        {
            showProductDetails();
        }
    );


    // =================================================
    // REFRESH
    // =================================================

    QObject::connect(
        refreshButton,
        &QPushButton::clicked,
        [&]()
        {
            loadProducts();
            filterProducts();
            showProductDetails();
        }
    );


    // =================================================
    // BUY
    // =================================================

    QObject::connect(
        buyButton,
        &QPushButton::clicked,
        [&]()
        {
            QListWidgetItem* item =
                productList->currentItem();


            if (item == nullptr)
            {
                QMessageBox::warning(
                    &window,
                    "No Product Selected",
                    "Please select a product first."
                );

                return;
            }


            const bool available =
                item->data(
                    Qt::UserRole + 2
                ).toBool();


            if (!available)
            {
                QMessageBox::warning(
                    &window,
                    "Unavailable",
                    "This product is unavailable."
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
                    &window,
                    "Purchase Successful",
                    "Purchase completed successfully."
                );


                loadProducts();
                filterProducts();
                showProductDetails();
            }
            else
            {
                QMessageBox::warning(
                    &window,
                    "Purchase Failed",
                    "Purchase could not be completed."
                );
            }
        }
    );


    // =================================================
    // RECOMMENDATIONS
    // =================================================

    QObject::connect(
        recommendationButton,
        &QPushButton::clicked,
        [&]()
        {
            QDialog dialog(
                &window
            );


            dialog.setWindowTitle(
                "Recommended Products"
            );


            dialog.resize(
                650,
                500
            );


            QVBoxLayout layout(
                &dialog
            );


            QLabel titleLabel(
                "Recommended Products"
            );


            titleLabel.setStyleSheet(
                "font-size: 22px;"
                "font-weight: bold;"
                "padding: 10px;"
            );


            layout.addWidget(
                &titleLabel
            );


            QListWidget list;


            list.setIconSize(
                QSize(70, 70)
            );


            const std::vector<Product>
                recommendations =
                    productRepository
                        .getRecommendations(
                            currentUserId,
                            10
                        );


            if (recommendations.empty())
            {
                list.addItem(
                    "No recommendations available."
                );
            }
            else
            {
                for (const Product& product :
                     recommendations)
                {
                    QListWidgetItem* item =
                        new QListWidgetItem();


                    item->setText(
                        QString::fromStdString(
                            product.getName()
                        ) +
                        " | $" +
                        QString::number(
                            product.getPrice(),
                            'f',
                            2
                        ) +
                        " | " +
                        QString::fromStdString(
                            product.getCategory()
                        )
                    );


                    const QString imagePath =
                        QString::fromStdString(
                            product.getImagePath()
                        );


                    QPixmap pixmap(
                        imagePath
                    );


                    if (!pixmap.isNull())
                    {
                        item->setIcon(
                            QIcon(
                                pixmap.scaled(
                                    70,
                                    70,
                                    Qt::KeepAspectRatio,
                                    Qt::SmoothTransformation
                                )
                            )
                        );
                    }


                    list.addItem(
                        item
                    );
                }
            }


            layout.addWidget(
                &list
            );


            QPushButton closeButton(
                "Close"
            );


            layout.addWidget(
                &closeButton
            );


            QObject::connect(
                &closeButton,
                &QPushButton::clicked,
                &dialog,
                &QDialog::accept
            );


            dialog.exec();
        }
    );


    // =================================================
    // MY ORDERS
    // =================================================

// =================================================
// MY ORDERS
// =================================================

QObject::connect(
    ordersButton,
    &QPushButton::clicked,
    [&]()
    {
        QDialog dialog(
            &window
        );


        dialog.setWindowTitle(
            "My Orders"
        );


        dialog.resize(
            800,
            600
        );


        QVBoxLayout layout(
            &dialog
        );


        QLabel titleLabel(
            "My Orders"
        );


        titleLabel.setStyleSheet(
            "font-size: 24px;"
            "font-weight: bold;"
            "padding: 10px;"
        );


        layout.addWidget(
            &titleLabel
        );


        QListWidget list;


        list.setStyleSheet(
            "QListWidget {"
            "    font-size: 16px;"
            "}"
            "QListWidget::item {"
            "    padding: 14px;"
            "}"
        );


        list.setVerticalScrollMode(
            QAbstractItemView::ScrollPerPixel
        );


        struct OrderDisplay
        {
            std::string productName;
            double price;
            std::string purchasedAt;
        };


        std::vector<OrderDisplay> orderDisplays;


        const std::string sql =
            "SELECT "
            "p.name, "
            "p.price, "
            "o.created_at "
            "FROM orders o "
            "INNER JOIN products p "
            "ON p.id = o.product_id "
            "WHERE o.user_id = " +
            std::to_string(currentUserId) +
            " "
            "ORDER BY o.created_at DESC;";


        auto callback = [](
            void* data,
            int argc,
            char** argv,
            char**
        ) -> int
        {
            auto* orders =
                static_cast<
                    std::vector<OrderDisplay>*
                >(data);


            if (argc < 3)
            {
                return 1;
            }


            OrderDisplay order;


            order.productName =
                argv[0]
                    ? argv[0]
                    : "";


            order.price =
                argv[1]
                    ? std::stod(argv[1])
                    : 0.0;


            order.purchasedAt =
                argv[2]
                    ? argv[2]
                    : "";


            orders->push_back(
                order
            );


            return 0;
        };


        if (!database.query(
                sql,
                callback,
                &orderDisplays))
        {
            list.addItem(
                "Failed to load orders."
            );
        }
        else if (orderDisplays.empty())
        {
            list.addItem(
                "You have no orders yet."
            );
        }
        else
        {
            for (const OrderDisplay& order :
                 orderDisplays)
            {
                QString text;


                text +=
                    "Product: ";


                text +=
                    QString::fromStdString(
                        order.productName
                    );


                text +=
                    "\nPrice: $";


                text +=
                    QString::number(
                        order.price,
                        'f',
                        2
                    );


                text +=
                    "\nPurchased: ";


                text +=
                    QString::fromStdString(
                        order.purchasedAt
                    );


                QListWidgetItem* item =
                    new QListWidgetItem(
                        text
                    );


                list.addItem(
                    item
                );
            }
        }


        layout.addWidget(
            &list
        );


        QPushButton closeButton(
            "Close"
        );


        layout.addWidget(
            &closeButton
        );


        QObject::connect(
            &closeButton,
            &QPushButton::clicked,
            &dialog,
            &QDialog::accept
        );


        dialog.exec();
    }
);

    // =================================================
    // PROFILE
    // =================================================

    QObject::connect(
        profileButton,
        &QPushButton::clicked,
        [&]()
        {
            const std::vector<Order>
                orders =
                    orderRepository
                        .getOrdersByUser(
                            currentUserId
                        );


            QString message;


            message +=
                "My Profile\n\n";


            message +=
                "User ID: " +
                QString::number(
                    currentUserId
                );


            message +=
                "\nUsername: " +
                QString::fromStdString(
                    user->getUsername()
                );


            message +=
                "\nEmail: " +
                QString::fromStdString(
                    user->getEmail()
                );


            message +=
                "\nOrders: " +
                QString::number(
                    orders.size()
                );


            QMessageBox::information(
                &window,
                "My Profile",
                message
            );
        }
    );


    // =================================================
    // INITIAL DATA
    // =================================================

    loadProducts();
    filterProducts();
    showProductDetails();


    // =================================================
    // SHOW
    // =================================================

    window.show();


    return app.exec();
}
