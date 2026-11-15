#ifndef STAFF_H
#define STAFF_H

#include <string>
#include <mysql.h>
#include <iostream>
#include <thread>
#include <map>

using namespace std;

// Staff Class
class Staff
{
private:
    string staff_id;
    string user_id;

    int menu;
    string query;
public:
    // Staff menu
    void home(string id, string type);
};

#endif