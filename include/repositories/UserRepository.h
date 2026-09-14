
#pragma once

#include <optional>
#include <string>

#include "database/database.h"
#include "models/User.h"

class UserRepository {
public:
    explicit UserRepository(Database& database);

    bool registerUser(
        const std::string& username,
        const std::string& email,
        const std::string& password
    );

    std::optional<User> login(
        const std::string& email,
        const std::string& password
    );

private:
    Database& database;
};

