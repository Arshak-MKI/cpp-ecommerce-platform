#include "models/User.h"

User::User(
    int id,
    const std::string& username,
    const std::string& email
)
    : id(id),
      username(username),
      email(email) {
}

int User::getId() const {
    return id;
}

const std::string& User::getUsername() const {
    return username;
}

const std::string& User::getEmail() const {
    return email;
}
