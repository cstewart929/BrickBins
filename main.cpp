#include <iostream>
#include <string>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

using namespace std;

static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
    int i;
    for (i = 0; i < argc; i++)
    {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

int getOperation(const char *operation)
{
    if (std::strcmp(operation, "CREATE") == 0) {
        return 1;
    } else if (std::strcmp(operation, "INSERT") == 0) {
        return 2;
    } else if (std::strcmp(operation, "UPDATE") == 0) {
        return 3;
    } else if (std::strcmp(operation, "LIST") == 0) {
        return 4;
    } else if (std::strcmp(operation, "DELETE") == 0) {
        return 5;
    } else {
        return -1;
    }
}

int createDatabase(const char *User)
{
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    std::string user_filename = std::string(User) + ".db";

    /* Open database */
    rc = sqlite3_open(user_filename.c_str(), &db);

    if (rc)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return (0);
    }
    else
    {
        fprintf(stdout, "Opened database successfully\n");
    }

    /* Create SQL statement */
    std::string sql_stmt = "CREATE TABLE " + std::string(User) + "("
                                                                 "ID INT PRIMARY KEY     NOT NULL,"
                                                                 "NAME           TEXT    NOT NULL,"
                                                                 "THEME          TEXT    NOT NULL,"
                                                                 "PIECES         INT,"
                                                                 "YEAR           INT,"
                                                                 "ASSEMBLED      BOOLEAN,"
                                                                 "ORGANIZED      BOOLEAN)";

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql_stmt.c_str(), callback, 0, &zErrMsg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
    {
        fprintf(stdout, "Table created successfully\n");
    }

    sqlite3_close(db);
    return 0;
}

int create(std::string user)
{
    return createDatabase(user.c_str());
}

int insert(std::string user, int ID, std::string name, std::string theme, int pieces, int year, bool assembled, bool organized)
{
    return 0;
}

int remove(std::string user, int ID)
{
    return 0;
}

template <typename T>
int update(std::string user, int ID, std::string field, T value)
{
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc < 2) // FIXED: Check if the required argument argv[1] exists
    {
        std::cout << "ERROR: Missing command operation. Usage: ./main [OPERATION] [ARGUMENTS]" << "\n";
        return 1;
    }

    int operation = getOperation(argv[1]);
    if (operation != -1)
    {
        std::cout << operation << "\n";
        switch (operation) {
            case 1:
                createDatabase(argv[2]);
                break;
        }
        
    }
    else
    {
        std::cout << "YOU FUCKED UP!" << "\n";
    }
    return 0; // Added explicit return 0
};