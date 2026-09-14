#include "repositories/UserRepository.h"

#include <string>

UserRepository::UserRepository(Database& database)
    : database(database) {
}

bool UserRepository::registerUser(
    const std::string& username,
    const std::string& email,
    const std::string& password
) {
    const std::string sql =
        "INSERT INTO users (username, email, password) VALUES ('" +
        username + "', '" +
        email + "', '" +
        password + "');";

    return database.execute(sql);
}

std::optional<User> UserRepository::login(
    const std::string& email,
    const std::string& password
) {
    struct LoginData {
        std::optional<User> user;
    };

    LoginData data;

    auto callback = [](void* rawData,
                       int argc,
                       char** argv,
                       char**) -> int {
        auto* data =
            static_cast<LoginData*>(rawData);

        if (argc < 3) {
            return 1;
        }

        data->user = User(
            std::stoi(argv[0]),
            argv[1] ? argv[1] : "",
            argv[2] ? argv[2] : ""
        );

        return 0;
    };

    const std::string sql =
        "SELECT id, username, email "
        "FROM users "
        "WHERE email = '" + email +
        "' AND password = '" + password +
        "' LIMIT 1;";

    if (!database.query(sql, callback, &data)) {
        return std::nullopt;
    }

    return data.user;
}