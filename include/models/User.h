
#pragma once

#include <string>

class User {
public:
    User(
        int id,
        const std::string& username,
        const std::string& email
    );

    int getId() const;

    const std::string& getUsername() const;

    const std::string& getEmail() const;

private:
    int id;
    std::string username;
    std::string email;
};
