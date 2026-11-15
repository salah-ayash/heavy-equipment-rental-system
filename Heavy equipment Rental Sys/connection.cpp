
#include "connection.h"

#include <iostream>
#include <cstdlib>
#include <thread>
#include <chrono>

using namespace std;

// Static MySQL connection
MYSQL* Connection::conn = nullptr;

void Connection::connection()
{
    // Read the password from a Windows environment variable
    char* dbPassword = nullptr;
    size_t passwordLength = 0;

    errno_t result = _dupenv_s(
        &dbPassword,
        &passwordLength,
        "RENTAL_DB_PASSWORD"
    );

    // Check whether the password is available
    if (result != 0 || dbPassword == nullptr || dbPassword[0] == '\0')
    {
        cerr << "Database password is not configured!" << endl;
        cerr << "Please set the RENTAL_DB_PASSWORD environment variable." << endl;

        free(dbPassword);
        return;
    }

    // Initialize MySQL
    MYSQL* mysql = mysql_init(nullptr);

    if (mysql == nullptr)
    {
        cerr << "Failed to initialize MySQL!" << endl;

        free(dbPassword);
        return;
    }

    // Connect without hardcoding the password
    MYSQL* connected = mysql_real_connect(
        mysql,
        "localhost",
        "root",
        dbPassword,
        "heavy_equipment",
        3306,
        nullptr,
        0
    );

    // Free the temporary password buffer
    free(dbPassword);

    // Verify the connection
    if (connected == nullptr)
    {
        cerr << "Failed to connect to the database!" << endl;
        cerr << "MySQL Error: " << mysql_error(mysql) << endl;

        mysql_close(mysql);
        conn = nullptr;
        return;
    }

    // Store the successful connection
    conn = connected;

    cout << "The database has been connected successfully!" << endl;
    cout << "Please wait a moment..." << endl;

    this_thread::sleep_for(chrono::seconds(2));

    // Clear the console on Windows
    system("cls");
}

void Connection::connect()
{
    connection();
}
