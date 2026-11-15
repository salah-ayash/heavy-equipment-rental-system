#ifndef CONNECTION_H
#define CONNECTION_H

#include <mysql.h>
#include <iostream>
#include <string>
#include <thread>

class Connection
{
private:
    // Private field for storing the MySQL connection handle
    // Private method for establishing a connection to the database
    void connection();

public:
    void connect();
    static MYSQL *conn;
};

#endif
