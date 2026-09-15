#include <QAbstractItemView>
#include <QApplication>
#include <QComboBox>
#include <QDialog>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QPixmap>
#include <QPushButton>
#include <QSize>
#include <QSplitter>
#include <QVBoxLayout>
#include <QWidget>

#include <algorithm>
#include <optional>
#include <set>
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
    // DATABASE TABLES
    // =================================================

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
        QMessageBox::critical(
            nullptr,
            "Database Error",
            "Failed to create products table."
        );

        return 1;
    }


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
        QMessageBox::critical(
            nullptr,
            "Database Error",
            "Failed to create users table."
        );

        return 1;
    }


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
        QMessageBox::critical(
            nullptr,
            "Database Error",
            "Failed to create orders table."
        );

        return 1;
    }


    // =================================================
    // REPOSITORIES
    // =================================================

    ProductRepository productRepository(database);
    UserRepository userRepository(database);
    OrderRepository orderRepository(database);


    // =================================================
    // LOGIN / REGISTER
    // =================================================

    std::optional<User> loggedUser;


    QDialog loginDialog;

    loginDialog.setWindowTitle(
        "Online Market - Login"
    );

    loginDialog.resize(
        450,
        300
    );


    QVBoxLayout loginLayout(
        &loginDialog
    );


    QLabel loginTitle(
        "Welcome to Online Market"
    );

    loginTitle.setAlignment(
        Qt::AlignCenter
    );

    loginTitle.setStyleSheet(
        "font-size: 26px;"
        "font-weight: bold;"
        "padding: 20px;"
    );


    loginLayout.addWidget(
        &loginTitle
    );


    QFormLayout loginForm;


    QLineEdit emailEdit;

    emailEdit.setPlaceholderText(
        "Enter your email"
    );


    QLineEdit passwordEdit;

    passwordEdit.setPlaceholderText(
        "Enter your password"
    );

    passwordEdit.setEchoMode(
        QLineEdit::Password
    );


    loginForm.addRow(
        "Email:",
        &emailEdit
    );


    loginForm.addRow(
        "Password:",
        &passwordEdit
    );


    loginLayout.addLayout(
        &loginForm
    );


    QHBoxLayout loginButtons;


    QPushButton loginButton(
        "Login"
    );


    QPushButton registerButton(
        "Register"
    );


    loginButtons.addWidget(
        &loginButton
    );


    loginButtons.addWidget(
        &registerButton
    );


    loginLayout.addLayout(
        &loginButtons
    );


    QLabel loginStatus;

    loginStatus.setAlignment(
        Qt::AlignCenter
    );

    loginStatus.setWordWrap(
        true
    );

    loginStatus.setStyleSheet(
        "padding: 10px;"
    );


    loginLayout.addWidget(
        &loginStatus
    );


    // =================================================
    // LOGIN
    // =================================================

    QObject::connect(
        &loginButton,
        &QPushButton::clicked,
        [&]()
        {
            const std::string email =
                emailEdit
                    .text()
                    .trimmed()
                    .toStdString();


            const std::string password =
                passwordEdit
                    .text()
                    .toStdString();


            if (email.empty() ||
                password.empty())
            {
                loginStatus.setText(
                    "Please enter email and password."
                );

                return;
            }


            auto result =
                userRepository.login(
                    email,
                    password
                );


            if (result.has_value())
            {
                loggedUser =
                    result;

                loginDialog.accept();
            }
            else
            {
                loginStatus.setText(
                    "Invalid email or password."
                );
            }
        }
    );


    // =================================================
    // REGISTER
    // =================================================

    QObject::connect(
        &registerButton,
        &QPushButton::clicked,
        [&]()
        {
            QDialog registerDialog(
                &loginDialog
            );


            registerDialog.setWindowTitle(
                "Online Market - Register"
            );


            registerDialog.resize(
                450,
                350
            );


            QVBoxLayout registerLayout(
                &registerDialog
            );


            QLabel registerTitle(
                "Create Your Account"
            );


            registerTitle.setAlignment(
                Qt::AlignCenter
            );


            registerTitle.setStyleSheet(
                "font-size: 24px;"
                "font-weight: bold;"
                "padding: 15px;"
            );


            registerLayout.addWidget(
                &registerTitle
            );


            QFormLayout registerForm;


            QLineEdit usernameEdit;

            usernameEdit.setPlaceholderText(
                "Enter username"
            );


            QLineEdit registerEmailEdit;

            registerEmailEdit.setPlaceholderText(
                "Enter email"
            );


            QLineEdit registerPasswordEdit;

            registerPasswordEdit.setPlaceholderText(
                "Enter password"
            );

            registerPasswordEdit.setEchoMode(
                QLineEdit::Password
            );


            registerForm.addRow(
                "Username:",
                &usernameEdit
            );


            registerForm.addRow(
                "Email:",
                &registerEmailEdit
            );


            registerForm.addRow(
                "Password:",
                &registerPasswordEdit
            );


            registerLayout.addLayout(
                &registerForm
            );


            QPushButton createAccountButton(
                "Create Account"
            );


            registerLayout.addWidget(
                &createAccountButton
            );


            QLabel registerStatus;

            registerStatus.setAlignment(
                Qt::AlignCenter
            );

            registerStatus.setWordWrap(
                true
            );

            registerStatus.setStyleSheet(
                "padding: 10px;"
            );


            registerLayout.addWidget(
                &registerStatus
            );


            QObject::connect(
                &createAccountButton,
                &QPushButton::clicked,
                [&]()
                {
                    const std::string username =
                        usernameEdit
                            .text()
                            .trimmed()
                            .toStdString();


                    const std::string email =
                        registerEmailEdit
                            .text()
                            .trimmed()
                            .toStdString();


                    const std::string password =
                        registerPasswordEdit
                            .text()
                            .toStdString();


                    if (username.empty() ||
                        email.empty() ||
                        password.empty())
                    {
                        registerStatus.setText(
                            "Please fill all fields."
                        );

                        return;
                    }


                    const bool registered =
                        userRepository.registerUser(
                            username,
                            email,
                            password
                        );


                    if (!registered)
                    {
                        registerStatus.setText(
                            "Registration failed.\n"
                            "Username or email may already exist."
                        );

                        return;
                    }


                    auto newUser =
                        userRepository.login(
                            email,
                            password
                        );


                    if (!newUser.has_value())
                    {
                        registerStatus.setText(
                            "Account created, "
                            "but login failed."
                        );

                        return;
                    }


                    loggedUser =
                        newUser;


                    registerDialog.accept();
                    loginDialog.accept();
                }
            );


            registerDialog.exec();
        }
    );


    // =================================================
    // OPEN LOGIN
    // =================================================

    if (loginDialog.exec() !=
            QDialog::Accepted ||
        !loggedUser.has_value())
    {
        return 0;
    }


    const int currentUserId =
        loggedUser->getId();


    // =================================================
    // MAIN WINDOW
    // =================================================

    QWidget window;

    window.setWindowTitle(
        "Online Market"
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
            "Online Market"
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
                        loggedUser->getUsername()
                    )
                )
                .arg(
                    QString::fromStdString(
                        loggedUser->getEmail()
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
    // SEARCH / CATEGORY / SORT / REFRESH
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


    QComboBox* sortComboBox =
        new QComboBox();


    sortComboBox->addItems(
        {
            "Default",
            "Price: Low to High",
            "Price: High to Low",
            "Name: A to Z",
            "Name: Z to A"
        }
    );


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
        sortComboBox
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


    // =================================================
    // PRODUCT LIST
    // =================================================

    QListWidget* productList =
        new QListWidget();


    productList->setIconSize(
        QSize(
            80,
            80
        )
    );


    productList->setSpacing(
        6
    );


    productList->setVerticalScrollMode(
        QAbstractItemView::ScrollPerPixel
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


    // =================================================
    // PRODUCT DETAILS
    // =================================================

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
    // BUTTONS
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


        std::vector<Product> products =
            productRepository.getAll();


        // -------------------------------------------------
        // SORT
        // -------------------------------------------------

        const QString sortMode =
            sortComboBox->currentText();


        if (sortMode ==
            "Price: Low to High")
        {
            std::sort(
                products.begin(),
                products.end(),
                [](
                    const Product& a,
                    const Product& b
                )
                {
                    return a.getPrice()
                        < b.getPrice();
                }
            );
        }
        else if (
            sortMode ==
            "Price: High to Low")
        {
            std::sort(
                products.begin(),
                products.end(),
                [](
                    const Product& a,
                    const Product& b
                )
                {
                    return a.getPrice()
                        > b.getPrice();
                }
            );
        }
        else if (
            sortMode ==
            "Name: A to Z")
        {
            std::sort(
                products.begin(),
                products.end(),
                [](
                    const Product& a,
                    const Product& b
                )
                {
                    return a.getName()
                        < b.getName();
                }
            );
        }
        else if (
            sortMode ==
            "Name: Z to A")
        {
            std::sort(
                products.begin(),
                products.end(),
                [](
                    const Product& a,
                    const Product& b
                )
                {
                    return a.getName()
                        > b.getName();
                }
            );
        }


        // -------------------------------------------------
        // CATEGORIES
        // -------------------------------------------------

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


        // -------------------------------------------------
        // PRODUCTS
        // -------------------------------------------------

        for (const Product& product :
             products)
        {
            QListWidgetItem* item =
                new QListWidgetItem();


            // Only product name
            item->setText(
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


            // Image
            item->setData(
                Qt::UserRole + 6,
                QString::fromStdString(
                    product.getImagePath()
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
    // SEARCH
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
    // CATEGORY
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
    // SORT
    // =================================================

    QObject::connect(
        sortComboBox,
        &QComboBox::currentTextChanged,
        [&]()
        {
            loadProducts();
            filterProducts();
            showProductDetails();
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
                700,
                550
            );


            QVBoxLayout layout(
                &dialog
            );


            QLabel titleLabel(
                "Recommended Products"
            );


            titleLabel.setAlignment(
                Qt::AlignCenter
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


            list.setIconSize(
                QSize(
                    70,
                    70
                )
            );


            list.setVerticalScrollMode(
                QAbstractItemView::ScrollPerPixel
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


            titleLabel.setAlignment(
                Qt::AlignCenter
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


            std::vector<OrderDisplay>
                orderDisplays;


            const std::string sql =
                "SELECT "
                "p.name, "
                "p.price, "
                "o.created_at "
                "FROM orders o "
                "INNER JOIN products p "
                "ON p.id = o.product_id "
                "WHERE o.user_id = " +
                std::to_string(
                    currentUserId
                ) +
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
                for (
                    const OrderDisplay& order :
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


                    list.addItem(
                        new QListWidgetItem(
                            text
                        )
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
                    loggedUser->getUsername()
                );


            message +=
                "\nEmail: " +
                QString::fromStdString(
                    loggedUser->getEmail()
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
    // INITIAL LOAD
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
