#ifndef USER_H
#define USER_H

#include <mysql.h>
#include <iostream>
#include <string>
#include <iomanip>
#include <regex>
#include <map>
#include <thread>

using namespace std;

// User class
class User
{
private:
    string user_id;
    string fname;
    string lname;
    string email;
    string password;
    string user_type;
    string address;
    string registration_date;

    string oldDate[6];

    int menu;
    int rowId;
    string query;
    int qstate;

public:
    // header function
    static void display_header(const string& title);

    // string validation using regular expression
    bool is_string(const string& str);

    // email validation using regular expression
    bool is_valid_email(const string& email);

    // password validation using regular expression
    bool is_valid_password(const string& pass);

    // license No. validation using regular expression
    bool is_valid_license_no(const string& license_no);
    // Tested Successfully
    void home();

    // Tested Successfully
    void login();

    // Tested Successfully
    void forget_password();

    // Tested Successfully
    void signup();

    // Logout
    void logout();

    // User Module
    void user(string id, string type);

    // List Users
    void list_user(string id, string type);

    // Add New Users
    void add_user(string id, string type);

    // Edit User
    void edit_user(string id, string type);

    // Delete User
    void delete_user(string id, string type);

    // Archive User
    void archive_user(string id, string type);

    // Search User
    void find_user(string id, string type);

    // customer menu
    // void customer();

    // Profile Menu
    void profile(string id, string type);

    // Edit Profile
    void edit_profile(string id, string type);

    // Change Password 
    void change_password(string id, string type);

};

#endif