#ifndef EQUIPMENT_TYPE_H
#define EQUIPMENT_TYPE_H

#include <mysql.h>
#include <iostream>
#include <string>
#include <iomanip>
#include <regex>
#include <map>
#include <thread>

using namespace std;

// Equipment Type class
class Equipment_Type
{
private:
    int menu;
    int rowId;
    string query;
    int qstate;
    string status;
    string date;

    string oldDate[2];

public:
    string equipment_type_id;
    string type_name;

    // string validation using regular expression
    bool is_string(const string& str);

    // type validation using regular expression
    bool is_valid_type(const string& type_name);

    // Tested Successfully
    void home();

    // Equipment Type Module
    void equipment_type(string id, string type);

    // List Equipment Type
    void list_equipment_type(string id, string type);

    // Add New Equipment Type
    void add_equipment_type(string id, string type);

    // Edit Equipment Type
    void edit_equipment_type(string id, string type);

    // Delete Equipment Type
    void delete_equipment_type(string id, string type);

    // Search Equipment Type
    void find_equipment_type(string id, string type);

};

#endif