#ifndef EQUIPMENT_H
#define EQUIPMENT_H

#include <mysql.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <regex>
#include <thread>
#include <map>

using namespace std;

// Equipment Class
class Equipment
{
private:
    string model;
    string color;
    string price;
    string type_id;
    string status;

    string oldDate[5];

    int menu;
    int rowId;
    string query;
    int qstate;

public:
    string equipment_id;

    // model validation using regular expression
    bool is_valid_model(const string& model);

    // price validation using regular expression
    bool is_valid_price(const string& price);

    // status validation using regular expression
    bool is_valid_status(const string& status);

    // Equipment Module
    void equipment(string id, string type);

    // List Equipment
    void list_equipment(string id, string type);

    // Add New Equipment
    void add_equipment(string id, string type);

    // Edit Equipment
    void edit_equipment(string id, string type);

    // Delete Equipment
    void delete_equipment(string id, string type);

    // Search Equipment
    void find_equipment(string id, string type);

};

#endif