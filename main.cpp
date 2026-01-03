#include <iostream>
#include <string>
#include <vector>
#include <map>

extern "C"
{
#include "sqlite3.h"
}

enum class Operation
{
    CREATE,
    INSERT,
    UPDATE,
    LIST,
    DELETE,
    UNKNOWN
};

struct LegoSet
{
    int id;
    std::string name;
    std::string theme;
    int pieces;
    int year;
    std::string status;
};

class LegoDatabase
{
private:
    std::string dbName;
    std::string tableName;

public:
    LegoDatabase(const std::string &username)
        : dbName(username + ".db"), tableName(username) {}

    bool createTable()
    {
        sqlite3 *db;
        if (sqlite3_open(dbName.c_str(), &db) != SQLITE_OK)
        {
            std::cerr << "Database Error: " << sqlite3_errmsg(db) << "\n";
            return false;
        }

        std::string sql = "CREATE TABLE IF NOT EXISTS " + tableName + " ("
                                                                      "ID INT PRIMARY KEY NOT NULL, "
                                                                      "NAME TEXT NOT NULL, "
                                                                      "THEME TEXT NOT NULL, "
                                                                      "PIECES INT, "
                                                                      "YEAR INT, "
                                                                      "STATUS TEXT);";

        char *errMsg = nullptr;
        if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK)
        {
            std::cerr << "SQL Error: " << errMsg << "\n";
            sqlite3_free(errMsg);
            sqlite3_close(db);
            return false;
        }

        std::cout << "Table '" << tableName << "' initialized successfully.\n";
        sqlite3_close(db);
        return true;
    }

    bool insertSet(const LegoSet &set)
    {
        sqlite3 *db;
        sqlite3_stmt *stmt;
        sqlite3_open(dbName.c_str(), &db);

        std::string sql = "INSERT INTO " + tableName + " (ID, NAME, THEME, PIECES, YEAR, STATUS) VALUES (?, ?, ?, ?, ?, ?);";

        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
        {
            std::cerr << "Prepare Error: " << sqlite3_errmsg(db) << "\n";
            sqlite3_close(db);
            return false;
        }

        sqlite3_bind_int(stmt, 1, set.id);
        sqlite3_bind_text(stmt, 2, set.name.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, set.theme.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 4, set.pieces);
        sqlite3_bind_int(stmt, 5, set.year);
        sqlite3_bind_text(stmt, 6, set.status.c_str(), -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) != SQLITE_DONE)
        {
            std::cerr << "Execution Error: " << sqlite3_errmsg(db) << "\n";
        }
        else
        {
            std::cout << "Set '" << set.name << "' added to database.\n";
        }

        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return true;
    }

    void listSets()
    {
        sqlite3 *db;
        sqlite3_stmt *stmt;
        sqlite3_open(dbName.c_str(), &db);

        std::string sql = "SELECT * FROM " + tableName + ";";

        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
        {
            std::cout << "\n--- Current Inventory ---\n";
            while (sqlite3_step(stmt) == SQLITE_ROW)
            {
                std::cout << "ID: " << sqlite3_column_int(stmt, 0) << " | "
                          << "Name: " << sqlite3_column_text(stmt, 1) << " | "
                          << "Theme: " << sqlite3_column_text(stmt, 2) << " | "
                          << "Pieces: " << sqlite3_column_int(stmt, 3) << " | "
                          << "Year: " << sqlite3_column_int(stmt, 4) << " | "
                          << "Status: " << sqlite3_column_text(stmt, 5) << "\n";
            }
        }

        sqlite3_finalize(stmt);
        sqlite3_close(db);
    }

    void deleteSet(const int id)
    {
        sqlite3 *db;
        sqlite3_stmt *stmt;
        sqlite3_open(dbName.c_str(), &db);

        std::string sql = "DELETE FROM " + tableName + " WHERE ID='" + std::to_string(id) + "';";

        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
        {
            std::cerr << "Prepare Error: " << sqlite3_errmsg(db) << "\n";
            sqlite3_close(db);
            return;
        }

        if (sqlite3_step(stmt) != SQLITE_DONE)
        {
            std::cerr << "Execution Error: " << sqlite3_errmsg(db) << "\n";
        }
        else
        {
            std::cout << "Set '" << id << "' removed from database.\n";
        }

        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return;
    }

    void updateSet(int id, const std::string &field, const std::string &value)
    {
        sqlite3 *db;
        sqlite3_stmt *stmt;
        sqlite3_open(dbName.c_str(), &db);

        static const std::vector<std::string> validFields = {
            "NAME", "THEME", "PIECES", "YEAR", "STATUS"};

        bool isValid = false;
        for (const auto &f : validFields)
        {
            if (field == f)
            {
                isValid = true;
                break;
            }
        }

        if (!isValid)
        {
            std::cerr << "Error: '" << field << "' is not a valid field.\n";
            sqlite3_close(db);
            return;
        }

        std::string sql = "UPDATE " + tableName + " SET " + field + " = ? WHERE ID = ?;";

        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
        {
            std::cerr << "Prepare Error: '" << sqlite3_errmsg(db) << "\n";
            sqlite3_close(db);
            return;
        }

        sqlite3_bind_text(stmt, 1, value.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 2, id);

        if (sqlite3_step(stmt) != SQLITE_DONE)
        {
            std::cerr << "Update Error: " << sqlite3_errmsg(db) << "\n";
        }
        else
        {
            if (sqlite3_changes(db) > 0)
            {
                std::cout << "Successfully updated " << field << " for Set " << id << ".\n";
            }
            else
            {
                std::cout << "No set found with ID " << id << ".\n";
            }
        }

        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return;
    }
};

Operation getOperation(const std::string &op)
{
    static const std::map<std::string, Operation> opMap = {
        {"CREATE", Operation::CREATE},
        {"INSERT", Operation::INSERT},
        {"LIST", Operation::LIST},
        {"DELETE", Operation::DELETE},
        {"UPDATE", Operation::UPDATE}};
    auto it = opMap.find(op);
    return (it != opMap.end()) ? it->second : Operation::UNKNOWN;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " [OPERATION] [DB_NAME] [ARGS...]\n";
        return 1;
    }

    Operation op = getOperation(argv[1]);
    std::string user = (argc >= 3) ? argv[2] : "default";
    LegoDatabase db(user);

    switch (op)
    {
    case Operation::CREATE:
        db.createTable();
        break;

    case Operation::INSERT:
        if (argc < 9)
        {
            std::cerr << "Error: Missing fields \n";
            std::cerr << "Usage: " << argv[0] << " INSERT [db] [ID] [Name] [Theme] [Pieces] [Year] [Status]\n";
        }
        else
        {
            try
            {
                int id = std::stoi(argv[3]);
                std::string name = argv[4];
                std::string theme = argv[5];
                int pieces = std::stoi(argv[6]);
                int year = std::stoi(argv[7]);
                std::string status = argv[8];

                db.insertSet({id, name, theme, pieces, year, status});
            }
            catch (const std::invalid_argument &e)
            {
                std::cerr << "Error: ID, Pieces, and Year must be numbers.\n";
            }
            catch (const std::out_of_range &e)
            {
                std::cerr << "Error: Number provided is too large.\n";
            }
        }
        break;

    case Operation::LIST:
        db.listSets();
        break;

    case Operation::DELETE:
        if (argc < 4)
        {
            std::cerr << "Error: Missing fields \n";
            std::cerr << "Usage: " << argv[0] << " DELETE [db] [ID]\n";
        }
        else
        {
            try
            {
                int id = std::stoi(argv[3]);

                db.deleteSet(id);
            }
            catch (const std::invalid_argument &e)
            {
                std::cerr << "Error: Please specify set ID. \n";
            }
        }
        break;

    case Operation::UPDATE:
        if (argc < 6)
        {
            std::cerr << "Usage: " << argv[0] << " UPDATE [db] [ID] [FIELD] [VALUE]\n";
            std::cerr << "Fields: NAME, THEME, PIECES, YEAR, STATUS\n";
        }
        else
        {
            try
            {
                int id = std::stoi(argv[3]);
                std::string field = argv[4];
                std::string value = argv[5];

                db.updateSet(id, field, value);
            }
            catch (const std::exception &e)
            {
                std::cerr << "Error: ID must be a number.\n";
            }
        }
        break;

    default:
        std::cerr << "Invalid or unimplemented operation.\n";
        return 1;
    }

    return 0;
}