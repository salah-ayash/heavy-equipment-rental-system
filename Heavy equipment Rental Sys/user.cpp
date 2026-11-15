#include "connection.h"
#include "user.h"
#include "customer.h"
#include "staff.h"

#include <sstream>

using namespace std;

void User::display_header(const string &title)
{

    system("cls");

    cout << "                 -------------------------------------------------  " << endl;
    cout << "*****************| Heavy Equipment Rental Management System |*****************" << endl;
    cout << "                 --------------------------------------------  " << endl;
    cout << "                >> " << title << " <<" << endl
         << endl;
}

// string validation using regular expression
bool User::is_string(const string& str)
{
    regex pattern("^[a-zA-Z]*$");
    return regex_match(str, pattern);
}

// email validation using regular expression
bool User::is_valid_email(const string& email)
{
    regex pattern("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");
    return regex_match(email, pattern);
}

// password validation using regular expression
bool User::is_valid_password(const string& pass) 
{;
    regex pattern("^(?=.*[A-Z])(?=.*[a-z]).{8,32}$");
    return regex_match(pass, pattern);
}

// license No. validation using regular expression
bool User::is_valid_license_no(const string& license_no)
{
    regex pattern("^[A-Z0-9]{8}$");
    return regex_match(license_no, pattern);
}

// Tested Successfully
void User::home()
{
    display_header("Main Screen");

    cout << "1. Login" << endl;
    cout << "2. Signup" << endl;

    do
    {
        cout << "--------------------------------------------" << endl;
        cout << "Enter a menu No.: ";
        string input;
        getline(cin >> ws, input);

        try {
            menu = stoi(input);
        } catch(invalid_argument&) {
            cout << "--------------------------------------------" << endl;
            cout << "Invalid input! Please enter a valid menu No." << endl;
            continue;
        } catch(out_of_range&) {
            cout << "--------------------------------------------" << endl;
            cout << "Invalid input! Please enter a valid menu No." << endl;
            continue;
        }
        
        if (menu == 1)
        {
            login();
            break;
        }
        else if (menu == 2)
        {
            signup();
            break;
        }
        else
        {
            cout << "--------------------------------------------" << endl;
            cout << "You entered an invalid menu No.!" << endl;
            continue;
        }

    } while (true);
}

// Tested Successfully
void User::login()
{

    display_header("Login Screen");

    do
    {
        cout << "Enter your email: ";
        getline(cin >> ws, email);

        if (email.empty()) 
        {
            cout << "--------------------------------------------" << endl;
            cout << "Email cannot be empty! Please try again." << endl;
            cout << "--------------------------------------------" << endl;
            continue;
        }
        else if (!is_valid_email(email))
        {
            cout << "--------------------------------------------" << endl;
            cout << "Invalid email format! Please enter a valid email." << endl;
            cout << "--------------------------------------------" << endl;
            continue;
        }
        else
        {
            break;
        }

    } while (true);

    do
    { 
        cout << "--------------------------------------------" << endl;
        cout << "Enter your password: ";
        getline(cin >> ws, password);

        if (password.empty()) 
        {
            cout << "--------------------------------------------" << endl;
            cout << "Password cannot be empty! Please try again." << endl;
            continue;
        }
        else
        {
            break;
        }

    } while (true);

    if (Connection::conn == nullptr)
    {
        cerr << "ERROR: MySQL connection is NULL!" << endl;
        cerr << "Check RENTAL_DB_PASSWORD and your connection settings." << endl;
        return;
    }

    if (mysql_ping(Connection::conn) != 0)
    {
        cerr << "ERROR: MySQL connection is not active!" << endl;
        cerr << mysql_error(Connection::conn) << endl;
        return;
    }

    query = "SELECT user_type, user_id FROM user WHERE email = '" + email + "' AND password = '" + password + "'";

    qstate = mysql_query(Connection::conn, query.c_str());


    if (!qstate)
    {

        MYSQL_RES *res = mysql_store_result(Connection::conn);
        MYSQL_ROW row;

        if (mysql_num_rows(res) > 0)
        {
            
            while ((row = mysql_fetch_row(res)) != nullptr)
            {

                cout << "--------------------------------------------" << endl;
                cout << "You have successfully logged in!" << endl;
                cout << "Please wait a moment..." << endl;
                this_thread::sleep_for(chrono::seconds(2));


                if (string(row[0]) == "Customer")
                {
                    string userId = row[1] ? row[1] : "";
                    string userType = row[0] ? row[0] : "";

                    mysql_free_result(res);

                    Customer customer;
                    customer.customer(userId, userType);

                    return;
                }
                else if (string(row[0]) == "Staff")
                {
                    string userId = row[1] ? row[1] : "";
                    string userType = row[0] ? row[0] : "";

                    mysql_free_result(res);

                    Staff staff;
                    staff.home(userId, userType);

                    return;
                }
                else
                {
                    mysql_free_result(res);

                    cout << "--------------------------------------------" << endl;
                    cout << "Invalid user type!" << endl;

                    return;
                }

                
            }
        }
        else
        {
            mysql_free_result(res);
            
            cout << "--------------------------------------------" << endl;
            cout << "Your email or password is wrong!" << endl;
            cout << "1. Try to login again?" << endl;
            cout << "2. Forget your password?" << endl;

            do
            {
                cout << "--------------------------------------------" << endl;
                cout << "Enter a menu No.: ";
                string input;
                getline(cin >> ws, input);

                try {
                    menu = stoi(input);
                } catch(invalid_argument&) {
                    cout << "--------------------------------------------" << endl;
                    cout << "Invalid input! Please enter a valid menu No." << endl;
                    continue;
                } catch(out_of_range&) {
                    cout << "--------------------------------------------" << endl;
                    cout << "Invalid input! Please enter a valid menu No." << endl;
                    continue;
                }

                if (menu == 1)
                {
                    login();
                    break;
                } else if (menu == 2)
                {
                    forget_password();
                    break;
                } else {
                    cout << "--------------------------------------------" << endl;
                    cout << "You entered an invalid menu No.!" << endl;
                    continue;
                }


            } while (true);
        }

    }
    else
    {
        cout << "--------------------------------------------" << endl;
        cout << "Failed to execute the query! " << mysql_errno(Connection::conn) << endl;
        cout << "Please wait a moment..." << endl;
        
        this_thread::sleep_for(chrono::seconds(2));
        
        login();
    }
}

// Tested Successfully
void User::forget_password()
{
    display_header("Forget Password Screen");

    do 
    {
        cout << "Enter your email: ";
        getline(cin >> ws, email);

        if (email.empty()) 
        {
            cout << "--------------------------------------------" << endl;
            cout << "Email cannot be empty! Please try again." << endl;
            cout << "--------------------------------------------" << endl;
            continue;
        }
        else if (!is_valid_email(email))
        {
            cout << "--------------------------------------------" << endl;
            cout << "Invalid email format! Please enter a valid email." << endl;
            cout << "--------------------------------------------" << endl;
            continue;
        }
        else
        {
            query = "SELECT email FROM user WHERE email = '" + email + "'";
            qstate = mysql_query(Connection::conn, query.c_str());
            
            if (!qstate)
            {

                MYSQL_RES *res = mysql_store_result(Connection::conn);
                MYSQL_ROW row;

                if (mysql_num_rows(res) > 0)
                {
                    mysql_free_result(res);
                    
                    do
                    {
                        cout << "--------------------------------------------" << endl;
                        cout << "Enter your a new password: ";
                        string new_password;
                        getline(cin >> ws, new_password);

                        if (new_password.empty())
                        {
                            cout << "--------------------------------------------" << endl;
                            cout << "Password cannot be empty! Please try again." << endl;
                            continue;
                        }
                        else if (!is_valid_password(new_password))
                        {
                            cout << "--------------------------------------------" << endl;
                            cout << "Invalid password! Password must contain at least one uppercase and one lowercase letter and be between 8 and 32 characters long." << endl;
                            continue;
                        }
                        else
                        {
                            query = "UPDATE user SET password = '" + new_password + "' WHERE email = '" + email + "'";
                            qstate = mysql_query(Connection::conn, query.c_str());

                            if (!qstate)
                            {
                                cout << "--------------------------------------------" << endl;
                                cout << "Your password has been updated successfully!" << endl;
                                cout << "Please wait a moment..." << endl;

                                this_thread::sleep_for(chrono::seconds(2));
                                home();
                            }
                            else
                            {
                                cout << "--------------------------------------------" << endl;
                                cout << "Failed to execute the query! " << mysql_errno(Connection::conn) << endl;
                                cout << "Please wait a moment..." << endl;

                                this_thread::sleep_for(chrono::seconds(2));
                                forget_password();
                            }
                            break;
                        }

                    } while (true);
                }
                else
                {
                       
                    mysql_free_result(res);

                    cout << "--------------------------------------------" << endl;
                    cout << "This user not found!" << endl;
                    
                    cout << "--------------------------------------------" << endl;
                    cout << "Do you want to try again? (Y/N)?" << endl;
                    
                    do
                    {
                        cout << "--------------------------------------------" << endl;
                        cout << "Enter a your choice: ";
                        char choice;
                        cin >> ws >> choice;

                        if (choice == 'Y' || choice == 'y')
                        {
                            forget_password();
                            break;
                        }
                        else if (choice == 'N' || choice == 'n')
                        {
                            home();
                            break;
                        }
                        else
                        {
                            cout << "--------------------------------------------" << endl;
                            cout << "You entered an invalid choice!" << endl;
                            cout << "--------------------------------------------" << endl;
                            continue;
                        }

                    } while (true);
                }
            }
            else
            {
                cout << "--------------------------------------------" << endl;
                cout << "Failed to execute the query! " << mysql_errno(Connection::conn) << endl;
                cout << "Please wait a moment..." << endl;

                this_thread::sleep_for(chrono::seconds(2));
                home();
                break;
            }
        }

    } while (true);
}

// Tested Successfully
void User::signup()
{

    display_header("Registration Screen");

    Customer customer;

    cout << "1. Continue Registering" << endl;
    cout << "2. Go back" << endl;

    do
    {
        cout << "--------------------------------------------" << endl;
        cout << "Enter a menu No.: ";
        string input;
        getline(cin >> ws, input);

        try {
            menu = stoi(input);
        } catch(invalid_argument&) {
            cout << "--------------------------------------------" << endl;
            cout << "Invalid input! Please enter a valid menu No." << endl;
            continue;
        } catch(out_of_range&) {
            cout << "--------------------------------------------" << endl;
            cout << "Invalid input! Please enter a valid menu No." << endl;
            continue;
        }
    
        if (menu == 1)
        {

            do
            {
                cout << "--------------------------------------------" << endl;
                cout << "Enter your first name: ";
                getline(cin >> ws, fname);

                if (fname.empty())
                {
                    cout << "--------------------------------------------" << endl;
                    cout << "First name cannot be empty! Please try again." << endl;
                    continue;
                }
                else if (!is_string(fname))
                {
                    cout << "--------------------------------------------" << endl;
                    cout << "Invalid first name! First name must contain only alphabetical characters." << endl;
                    continue;
                }
                else
                {
                    break;
                }

            } while (true);

            do
            {
                cout << "--------------------------------------------" << endl;
                cout << "Enter your last name: ";
                getline(cin >> ws, lname);

                if (lname.empty())
                {
                    cout << "--------------------------------------------" << endl;
                    cout << "Last name cannot be empty! Please try again." << endl;
                    continue;
                }
                else if (!is_string(lname))
                {
                    cout << "--------------------------------------------" << endl;
                    cout << "Invalid last name! Last name must contain only alphabetical characters." << endl;
                    continue;
                }
                else
                {
                    break;
                }

            } while (true);

            do
            {
                cout << "--------------------------------------------" << endl;
                cout << "Enter your email: ";
                getline(cin >> ws, email);

                if (email.empty())
                {
                    cout << "--------------------------------------------" << endl;
                    cout << "Email cannot be empty! Please try again." << endl;
                    continue;
                }
                else if (!is_valid_email(email))
                {
                    cout << "--------------------------------------------" << endl;
                    cout << "Invalid email format! Please enter a valid email." << endl;
                    continue;
                }
                else
                {
                    query = "SELECT email FROM user WHERE email = '" + email + "'";
                    qstate = mysql_query(Connection::conn, query.c_str());

                    if (!qstate)
                    {

                        MYSQL_RES *res = mysql_store_result(Connection::conn);
                        MYSQL_ROW row;

                        if (mysql_num_rows(res) > 0)
                        {

                            mysql_free_result(res);

                            cout << "--------------------------------------------" << endl;
                            cout << "This email has been already taken!" << endl;
                            continue;

                        }
                        else
                        {
                            mysql_free_result(res);
                            break;
                        }
                    }
                    else
                    {
                        cout << "--------------------------------------------" << endl;
                        cout << "Failed to execute the query! " << mysql_errno(Connection::conn) << endl;
                        cout << "Please wait a moment..." << endl;

                        this_thread::sleep_for(chrono::seconds(2));
                        home();
                        break;
                    }
                }

            } while (true);

            do
            {
                cout << "--------------------------------------------" << endl;
                cout << "Enter your password: ";
                getline(cin >> ws, password);

                if (password.empty())
                {
                    cout << "--------------------------------------------" << endl;
                    cout << "Password cannot be empty! Please try again." << endl;
                    continue;
                }
                else if (!is_valid_password(password))
                {
                    cout << "--------------------------------------------" << endl;
                    cout << "Invalid password! Password must contain at least one uppercase and one lowercase letter and be between 8 and 32 characters long." << endl;
                    continue;
                }
                else
                {
                    break;
                }

            } while (true);

            do
            {

                cout << "--------------------------------------------" << endl;
                cout << "Enter your license No.: ";
                getline(cin >> ws, customer.license_no);

                if (customer.license_no.empty())
                {
                    cout << "--------------------------------------------" << endl;
                    cout << "License No. cannot be empty! Please try again." << endl;
                    continue;
                }
                else if (!is_valid_license_no(customer.license_no))
                {
                    cout << "--------------------------------------------" << endl;
                    cout << "Invalid license No.! License No. must be 8 characters long and contain only uppercase letters and digits." << endl;
                    continue;
                }
                else
                {
                    break;
                }

            } while (true);

            do
            {
                cout << "--------------------------------------------" << endl;
                cout << "Enter your address: ";
                getline(cin >> ws, address);

                if (address.empty())
                {
                    cout << "Address cannot be empty! Please try again." << endl;
                    continue;
                }
                else
                {
                    break;
                }

            } while (true);

            query = "INSERT INTO user (fname, lname, email, password, address) VALUES ('" + fname + "','" + lname + "','" + email + "','" + password + "','" + address + "')";
            qstate = mysql_query(Connection::conn, query.c_str());

            // Retrieve the last inserted ID
            unsigned long user_id = mysql_insert_id(Connection::conn);

            query = "INSERT INTO customer (user_id, license_no) VALUES ('" + to_string(user_id) + "','" + customer.license_no + "')";
            qstate = mysql_query(Connection::conn, query.c_str());

            if (!qstate)
            {
                cout << "--------------------------------------------" << endl;
                cout << "You have been registered successfully!" << endl;
                cout << "Please wait a moment..." << endl;

                this_thread::sleep_for(chrono::seconds(2));
                home();
            }
            else
            {
                cout << "--------------------------------------------" << endl;
                cout << "Failed to execute the query! " << mysql_errno(Connection::conn) << endl;
                cout << "Please wait a moment..." << endl;

                this_thread::sleep_for(chrono::seconds(2));
                signup();
            }

            break;
        }
        else if(menu == 2) 
        {
            home();
            break;
        }
        else
        {
            cout << "--------------------------------------------" << endl;
            cout << "You entered an invalid menu No.!" << endl;
            continue;
        }

    } while (true);
}

// Tested Successfully
void User::logout()
{
    display_header("Logout Screen");
    cout << "--------------------------------------------" << endl;
    cout << "You have been logged out successfully.!" << endl;
    cout << "Please wait a moment..." << endl;
    this_thread::sleep_for(chrono::seconds(2));
    home();
}

// User Module
void User::user(string id, string type) 
{

    display_header("Manage Users Screen");

    cout << "1. List Users" << endl;
    cout << "2. Add a new user" << endl;
    cout << "3. Edit a user" << endl;
    cout << "4. Delete a user" << endl;
    cout << "5. Archive a user" << endl;
    cout << "6. Search for a user" << endl;
    cout << "7. Go back" << endl;

    do
    {
        cout << "--------------------------------------------" << endl;
        cout << "Enter a menu No.: ";
        string input;
        getline(cin >> ws, input);

        try {
            menu = stoi(input);
        } catch(invalid_argument&) {
            cout << "--------------------------------------------" << endl;
            cout << "Invalid input! Please enter a valid menu No." << endl;
            continue;
        } catch(out_of_range&) {
            cout << "--------------------------------------------" << endl;
            cout << "Invalid input! Please enter a valid menu No." << endl;
            continue;
        }

        if (menu == 1)
        {
            list_user(id, type);
            break;
        }
        else if (menu == 2)
        {
            add_user(id, type);
            break;
        }
        else if (menu == 3)
        {
            edit_user(id, type);
            break;
        }
        else if (menu == 4)
        {
            delete_user(id, type);
            break;
        }
        else if (menu == 5)
        {
            archive_user(id, type);
            break;
        }
        else if (menu == 6)
        {
            find_user(id, type);
            break;
        }
        else if (menu == 7)
        {
            Staff staff;
            staff.home(id, type);
            break;
        }
        else
        {
            cout << "--------------------------------------------" << endl;
            cout << "You entered an invalid menu No.!" << endl;
            continue;
        }

    } while (true);
    
}

// List Users
void User::list_user(string id, string type) 
{

    display_header("List Users Screen");

    query = "SELECT * FROM user JOIN customer ON user.user_id = customer.user_id WHERE status = 'Unarchive' AND user_type = 'Customer' ORDER BY user.user_id DESC";
    qstate = mysql_query(Connection::conn, query.c_str());

    if (!qstate)
    {
        int rowNum = 1;

        cout << left << setw(5) << "No." << setw(20) << "First Name" << setw(20) << "Last Name" << setw(30) << "Email" << setw(20) << "License No." << setw(20) << "Address" << setw(20) << "Status" << setw(20) << "Last Update" << endl;

        cout << setfill('-') << setw(150) << "" << endl;
        cout << setfill(' ');

        MYSQL_RES *res = mysql_store_result(Connection::conn);
        MYSQL_ROW row;

        if (mysql_num_rows(res) > 0)
        {

            while ((row = mysql_fetch_row(res)) != nullptr)
            {
                cout << left << setw(5) << rowNum++
                    << setw(20) << row[1]
                    << setw(20) << row[2]
                    << setw(30) << row[3]
                    << setw(20) << row[10]
                    << setw(20) << row[5]
                    << setw(20) << row[11]
                    << setw(20) << row[8]
                    << endl;
            }
            mysql_free_result(res);

            cout << setfill('-') << setw(150) << "" << endl;
            cout << setfill(' ');

            cout << "1. Go back" << endl;

            do
            {
                cout << "--------------------------------------------" << endl;
                cout << "Enter a menu No.: ";
                string input;
                getline(cin >> ws, input);

                try {
                    menu = stoi(input);
                } catch(invalid_argument&) {
                    cout << "--------------------------------------------" << endl;
                    cout << "Invalid input! Please enter a valid menu No." << endl;
                    continue;
                } catch(out_of_range&) {
                    cout << "--------------------------------------------" << endl;
                    cout << "Invalid input! Please enter a valid menu No." << endl;
                    continue;
                }

                if (menu == 1)
                {
                    user(id, type);
                    break;
                }
                else
                {
                    cout << "--------------------------------------------" << endl;
                    cout << "You entered an invalid menu No.!" << endl;
                    continue;
                }

            } while (true);
        }
        else
        {
            mysql_free_result(res);
            
            cout << "--------------------------------------------" << endl;
            cout << "There is no user to list!" << endl;
            cout << "Please wait a moment..." << endl;
            this_thread::sleep_for(chrono::seconds(2));
            user(id, type);
        }
        
    }
    else
    {
        cout << "--------------------------------------------" << endl;
        cout << "Failed to execute the query!" << endl;
        cout << "Error number: " << mysql_errno(Connection::conn) << endl;
        cout << "Error message: " << mysql_error(Connection::conn) << endl;

        system("pause");
    }
}

// Add New Users
void User::add_user(string id, string type) 
{

    display_header("Add a New User Screen");

    Customer customer;

    cout << "1. Continue Adding" << endl;
    cout << "2. Go back" << endl;

    do
    {

        cout << "--------------------------------------------" << endl;
        cout << "Enter a menu No.: ";
        string input;
        getline(cin >> ws, input);

        try {
            menu = stoi(input);
        } catch(invalid_argument&) {
            cout << "--------------------------------------------" << endl;
            cout << "Invalid input! Please enter a valid menu No." << endl;
            continue;
        } catch(out_of_range&) {
            cout << "--------------------------------------------" << endl;
            cout << "Invalid input! Please enter a valid menu No." << endl;
            continue;
        }
    
        if (menu == 1)
        {
            do
            {
                cout << "--------------------------------------------" << endl;
                cout << "Enter the user first name: ";
                getline(cin >> ws, fname);

                if (fname.empty())
                {
                    cout << "--------------------------------------------" << endl;
                    cout << "First name cannot be empty! Please try again." << endl;
                    continue;
                }
                else if (!is_string(fname))
                {
                    cout << "--------------------------------------------" << endl;
                    cout << "Invalid first name! First name must contain only alphabetical characters." << endl;
                    continue;
                }
                else
                {
                    break;
                }

            } while (true);

            do
            {
                cout << "--------------------------------------------" << endl;
                cout << "Enter the user last name: ";
                getline(cin >> ws, lname);

                if (lname.empty())
                {
                    cout << "--------------------------------------------" << endl;
                    cout << "Last name cannot be empty! Please try again." << endl;
                    continue;
                }
                else if (!is_string(lname))
                {
                    cout << "--------------------------------------------" << endl;
                    cout << "Invalid last name! Last name must contain only alphabetical characters." << endl;
                    continue;
                }
                else
                {
                    break;
                }

            } while (true);

            do
            {
                cout << "--------------------------------------------" << endl;
                cout << "Enter the user email: ";
                getline(cin >> ws, email);

                if (email.empty())
                {
                    cout << "--------------------------------------------" << endl;
                    cout << "Email cannot be empty! Please try again." << endl;
                    continue;
                }
                else if (!is_valid_email(email))
                {
                    cout << "--------------------------------------------" << endl;
                    cout << "Invalid email format! Please enter a valid email." << endl;
                    continue;
                }
                else
                {
                    query = "SELECT email FROM user WHERE email = '" + email + "'";
                    qstate = mysql_query(Connection::conn, query.c_str());

                    if (!qstate)
                    {

                        MYSQL_RES *res = mysql_store_result(Connection::conn);
                        MYSQL_ROW row;

                        if (mysql_num_rows(res) > 0)
                        {
                            mysql_free_result(res);

                            cout << "--------------------------------------------" << endl;
                            cout << "This email is already exist!" << endl;
                            continue;
                        }
                        else
                        {
                            break;
                        }
                        
                    }
                    else
                    {
                        cout << "--------------------------------------------" << endl;
                        cout << "Failed to execute the query! " << mysql_errno(Connection::conn) << endl;
                        cout << "Please wait a moment..." << endl;

                        this_thread::sleep_for(chrono::seconds(2));
                        user(id, type);
                    }
                }

            } while (true);

            do
            {
                cout << "--------------------------------------------" << endl;
                cout << "Enter the user password: ";
                getline(cin >> ws, password);

                if (password.empty())
                {
                    cout << "--------------------------------------------" << endl;
                    cout << "Email cannot be empty! Please try again." << endl;
                    continue;
                }
                else if (!is_valid_password(password))
                {
                    cout << "--------------------------------------------" << endl;
                    cout << "Invalid password! Password must contain at least one uppercase and one lowercase letter and be between 8 and 32 characters long." << endl;
                    continue;
                }
                else
                {
                    break;
                }

            } while (true);

            do
            {

                cout << "--------------------------------------------" << endl;
                cout << "Enter the user license No.: ";
                getline(cin >> ws, customer.license_no);

                if (customer.license_no.empty())
                {
                    cout << "--------------------------------------------" << endl;
                    cout << "License No. cannot be empty! Please try again." << endl;
                    continue;
                }
                else if (!is_valid_license_no(customer.license_no))
                {
                    cout << "--------------------------------------------" << endl;
                    cout << "Invalid license No.! License No. must be 8 characters long and contain only uppercase letters and digits." << endl;
                    continue;
                }
                else
                {
                    break;
                }

            } while (true);

            do
            {
                cout << "--------------------------------------------" << endl;
                cout << "Enter the user address: ";
                getline(cin >> ws, address);

                if (address.empty())
                {
                    cout << "--------------------------------------------" << endl;
                    cout << "Address cannot be empty! Please try again." << endl;
                    continue;
                }
                else
                {
                    break;
                }

            } while (true);
            
            query = "INSERT INTO user (fname, lname, email, password, address) VALUES ('" + fname + "','" + lname + "','" + email + "','" + password + "','" + address + "')";
            qstate = mysql_query(Connection::conn, query.c_str());

            // Retrieve the last inserted ID
            unsigned long user_id = mysql_insert_id(Connection::conn);

            query = "INSERT INTO customer (user_id, license_no) VALUES ('" + to_string(user_id) + "','" + customer.license_no + "')";
            qstate = mysql_query(Connection::conn, query.c_str());

            if (!qstate)
            {
                cout << "--------------------------------------------" << endl;
                cout << "This user has been added successfully!" << endl;
                cout << "Please wait a moment..." << endl;

                this_thread::sleep_for(chrono::seconds(2));
                user(id, type);
            }
            else
            {
                cout << "--------------------------------------------" << endl;
                cout << "Failed to execute the query! " << mysql_errno(Connection::conn) << endl;
                cout << "Please wait a moment..." << endl;

                this_thread::sleep_for(chrono::seconds(2));
                user(id, type);
            }
            break;
        }
        else if (menu == 2)
        {
            user(id, type);
            break;
        }
        else
        {
            cout << "--------------------------------------------" << endl;
            cout << "You entered an invalid menu No.!" << endl;
            continue;
        }

    } while (true);
}

// Edit User
void User::edit_user(string id, string type) 
{

    display_header("Edit User Screen");

    Customer customer;
    Staff staff;

    query = "SELECT * FROM user JOIN customer ON user.user_id = customer.user_id WHERE status = 'Unarchive' AND user_type = 'Customer' ORDER BY user.user_id DESC";
    qstate = mysql_query(Connection::conn, query.c_str());

    if (!qstate)
    {

        int rowNum = 1;

        cout << left << setw(5) << "No." << setw(20) << "First Name" << setw(20) << "Last Name" << setw(30) << "Email" << setw(20) << "License No." << setw(20) << "Address" << setw(20) << "Status" << setw(20) << "Last Update" << endl;

        cout << setfill('-') << setw(150) << "" << endl;
        cout << setfill(' ');

        MYSQL_RES *res = mysql_store_result(Connection::conn);
        MYSQL_ROW row;

        while ((row = mysql_fetch_row(res)) != nullptr)
        {
            cout << left << setw(5) << rowNum
                << setw(20) << row[1]
                << setw(20) << row[2]
                << setw(30) << row[3]
                << setw(20) << row[10]
                << setw(20) << row[5]
                << setw(20) << row[11]
                << setw(20) << row[8]
                << endl;
        }
        mysql_free_result(res);

        cout << setfill('-') << setw(150) << "" << endl;
        cout << setfill(' ');

        cout << "1. Continue editing" << endl;
        cout << "2. Go back" << endl << endl;

        do
        {
            cout << "--------------------------------------------" << endl;
            cout << "Enter a menu No.: ";
            string input;
            getline(cin >> ws, input);

            try {
                menu = stoi(input);
            } catch(invalid_argument&) {
                cout << "--------------------------------------------" << endl;
                cout << "Invalid input! Please enter a valid menu No." << endl;
                continue;
            } catch(out_of_range&) {
                cout << "--------------------------------------------" << endl;
                cout << "Invalid input! Please enter a valid menu No." << endl;
                continue;
            }
                    
            if (menu == 1)
            {
                display_header("Edit User Screen");

                map<int, string> rowIdMap;
                int rowNum = 1;

                query = "SELECT * FROM user JOIN customer ON user.user_id = customer.user_id WHERE status = 'Unarchive' AND user_type = 'Customer' ORDER BY user.user_id DESC";
                qstate = mysql_query(Connection::conn, query.c_str());

                if (!qstate)
                {

                    cout << left << setw(5) << "No." << setw(20) << "First Name" << setw(20) << "Last Name" << setw(30) << "Email" << setw(20) << "License No." << setw(20) << "Address" << setw(20) << "Status" << setw(20) << "Last Update" << endl;

                    cout << setfill('-') << setw(150) << "" << endl;
                    cout << setfill(' ');

                    MYSQL_RES *res = mysql_store_result(Connection::conn);
                    MYSQL_ROW row;

                    while ((row = mysql_fetch_row(res)) != nullptr)
                    {
                        cout << left << setw(5) << rowNum
                            << setw(20) << row[1]
                            << setw(20) << row[2]
                            << setw(30) << row[3]
                            << setw(20) << row[10]
                            << setw(20) << row[5]
                            << setw(20) << row[11]
                            << setw(20) << row[8]
                            << endl;

                        rowIdMap[rowNum] = row[0];

                        rowNum++;
                    }
                    mysql_free_result(res);

                    cout << setfill('-') << setw(150) << "" << endl;
                    cout << setfill(' ');
                    
                    do
                    {

                        cout << "Enter the row No. of the user: ";
                        string input;
                        getline(cin >> ws, input);

                        try {
                            rowId = stoi(input);
                        } catch(invalid_argument&) {
                            cout << "--------------------------------------------" << endl;
                            cout << "Invalid input! Please enter a valid row No." << endl;
                            continue;
                        } catch(out_of_range&) {
                            cout << "--------------------------------------------" << endl;
                            cout << "Invalid input! Please enter a valid row No." << endl;
                            continue;
                        }

                        if (rowIdMap.count(rowId) > 0) 
                        {
                            display_header("Edit a User Screen");
                                    
                            user_id = rowIdMap[rowId];

                            query = "SELECT * FROM user JOIN customer ON user.user_id = customer.user_id WHERE status = 'Unarchive' AND user_type = 'Customer' AND user.user_id = '" + user_id + "'";;
                            qstate = mysql_query(Connection::conn, query.c_str());

                            if (!qstate)
                            {

                                cout << left << setw(20) << "First Name" << setw(20) << "Last Name" << setw(30) << "Email" << setw(20) << "License No." << setw(20) << "Address" << setw(20) << "Status" << setw(20) << "Last Update" << endl;

                                cout << setfill('-') << setw(150) << "" << endl;
                                cout << setfill(' ');

                                MYSQL_RES *res = mysql_store_result(Connection::conn);
                                MYSQL_ROW row;

                                while ((row = mysql_fetch_row(res)) != nullptr)
                                {
                                    cout << left << setw(20) << row[1]
                                        << setw(20) << row[2]
                                        << setw(30) << row[3]
                                        << setw(20) << row[10]
                                        << setw(20) << row[5]
                                        << setw(20) << row[11]
                                        << setw(20) << row[8]
                                        << endl;
                                }
                                mysql_free_result(res);

                                cout << setfill('-') << setw(150) << "" << endl;
                                cout << setfill(' ');

                                cout << "Are you sure do you want to edit this user? (Y/N)?" << endl;

                                do
                                {
                                    cout << "--------------------------------------------" << endl;
                                    cout << "Enter a your choice: ";
                                    char choice;
                                    cin >> ws >> choice;

                                    if (choice == 'Y' || choice == 'y')
                                    {
                                        
                                        cout << setfill('-') << setw(150) << "" << endl;
                                        cout << setfill(' ');
                                        cout << "		>> Please let the value blank, if you wish to not update it <<" << endl;
                                        cout << setfill('-') << setw(150) << "" << endl;
                                        cout << setfill(' ');

                                        do
                                        {   
                                            cout << "Enter the user first name: ";
                                            getline(cin >> ws, fname);

                                            if (fname.empty())
                                            {
                                                fname = oldDate[1];
                                                break;
                                            }
                                            else if (!is_string(fname))
                                            {
                                                cout << "--------------------------------------------" << endl;
                                                cout << "Invalid first name! First name must contain only alphabetical characters." << endl;
                                                continue;
                                            }
                                            else
                                            {
                                                break;
                                            }

                                        } while (true);

                                        do
                                        {
                                            cout << "--------------------------------------------" << endl;
                                            cout << "Enter the user last name: ";
                                            getline(cin >> ws, lname);

                                            if (lname.empty())
                                            {
                                                lname = oldDate[1];
                                                break;
                                            }
                                            else if (!is_string(lname))
                                            {
                                                cout << "--------------------------------------------" << endl;
                                                cout << "Invalid last name! Last name must contain only alphabetical characters." << endl;
                                                continue;
                                            }
                                            else
                                            {
                                                break;
                                            }

                                        } while (true);

                                        do
                                        {
                                            cout << "--------------------------------------------" << endl;
                                            cout << "Enter the user email: ";
                                            getline(cin >> ws, email);

                                            if (email.empty())
                                            {
                                                email = oldDate[2];
                                                break;
                                            }
                                            else if (!is_valid_email(email))
                                            {
                                                cout << "--------------------------------------------" << endl;
                                                cout << "Invalid email format! Please enter a valid email." << endl;
                                                continue;
                                            }
                                            else
                                            {
                                                query = "SELECT email FROM user WHERE email = '" + email + "' AND user.user_id != '" + user_id + "'";
                                                qstate = mysql_query(Connection::conn, query.c_str());

                                                if (!qstate)
                                                {

                                                    MYSQL_RES *res = mysql_store_result(Connection::conn);
                                                    MYSQL_ROW row;

                                                    if (mysql_num_rows(res) > 0)
                                                    {
                                                        mysql_free_result(res);

                                                        cout << "--------------------------------------------" << endl;
                                                        cout << "This email is already exist!" << endl;
                                                        continue;
                                                    }
                                                    else
                                                    {
                                                        break;
                                                    }
                                                }
                                                else
                                                {
                                                    cout << "--------------------------------------------" << endl;
                                                    cout << "Failed to execute the query! " << mysql_errno(Connection::conn) << endl;
                                                    cout << "Please wait a moment..." << endl;

                                                    this_thread::sleep_for(chrono::seconds(2));
                                                    user(id, type);
                                                }
                                            }

                                        } while (true);

                                        do
                                        {
                                            cout << "--------------------------------------------" << endl;
                                            cout << "Enter the user password: ";
                                            getline(cin >> ws, password);

                                            if (password.empty())
                                            {
                                                password = oldDate[3];
                                                break;
                                            }
                                            else if (!is_valid_password(password))
                                            {
                                                cout << "--------------------------------------------" << endl;
                                                cout << "Invalid password! Password must contain at least one uppercase and one lowercase letter and be between 8 and 32 characters long." << endl;
                                                continue;
                                            }
                                            else
                                            {
                                                break;
                                            }

                                        } while (true);

                                        do
                                        {
                                            cout << "--------------------------------------------" << endl;
                                            cout << "Enter the user license No.: ";
                                            getline(cin >> ws, customer.license_no);

                                            if (customer.license_no.empty())
                                            {
                                                customer.license_no = oldDate[4];
                                                break;
                                            }
                                            else if (!is_valid_license_no(customer.license_no))
                                            {
                                                cout << "--------------------------------------------" << endl;
                                                cout << "Invalid license No.! License No. must be 8 characters long and contain only uppercase letters and digits." << endl;
                                                continue;
                                            }
                                            else
                                            {
                                                break;
                                            }

                                        } while (true);

                                        cout << "--------------------------------------------" << endl;
                                        cout << "Enter the user address: ";
                                        getline(cin >> ws, address);

                                        if (address.empty())
                                        {
                                            address = oldDate[5];
                                        }

                                        query = "UPDATE user JOIN customer ON user.user_id = customer.user_id SET user.fname='" + fname + "', user.lname = '" + lname + "', user.email = '" + email + "', user.password = '" + password + "', user.address = '" + address + "', customer.license_no = '" + customer.license_no + "' WHERE user.user_id = '" + user_id + "'";
                                        qstate = mysql_query(Connection::conn, query.c_str());

                                        if (!qstate)
                                        {
                                            cout << "--------------------------------------------" << endl;
                                            cout << "This user has been updated successfully!" << endl;
                                            cout << "Please wait a moment..." << endl;

                                            this_thread::sleep_for(chrono::seconds(2));
                                            user(id, type);
                                        }
                                        else
                                        {
                                            cout << "--------------------------------------------" << endl;
                                            cout << "Failed to execute the query! " << mysql_errno(Connection::conn) << endl;
                                            cout << "Please wait a moment..." << endl;

                                            this_thread::sleep_for(chrono::seconds(2));
                                            user(id, type);
                                        }
                                        break;
                                    }
                                    else if (choice == 'N' || choice == 'n')
                                    {
                                        edit_user(id, type);
                                        break;
                                    }
                                    else
                                    {
                                        cout << "--------------------------------------------" << endl;
                                        cout << "You entered an invalid choice!" << endl;
                                        continue;
                                    }

                                } while (true);
                            }
                            else
                            {
                                cout << "--------------------------------------------" << endl;
                                cout << "Failed to execute the query! " << mysql_errno(Connection::conn) << endl;
                                cout << "Please wait a moment..." << endl;

                                this_thread::sleep_for(chrono::seconds(2));
                                user(id, type);
                            }
                            break;
                        }
                        else
                        {
                            cout << "--------------------------------------------" << endl;
                            cout << "You entered an invalid row No.!" << endl;
                            continue;
                        }

                    } while (true);
                }
                else
                {
                    cout << "--------------------------------------------" << endl;
                    cout << "Failed to execute the query! " << mysql_errno(Connection::conn) << endl;
                    cout << "Please wait a moment..." << endl;

                    this_thread::sleep_for(chrono::seconds(2));
                    user(id, type);
                }
            
            }
            else if (menu == 2)
            {
                user(id, type);
                break;
            }
            else
            {
                cout << "--------------------------------------------" << endl;
                cout << "You entered an invalid menu No.!" << endl;
                continue;
            }


        } while (true);
    }
    else
    {
        cout << "--------------------------------------------" << endl;
        cout << "Failed to execute the query! " << mysql_errno(Connection::conn) << endl;
        cout << "Please wait a moment..." << endl;

        this_thread::sleep_for(chrono::seconds(2));
        user(id, type);
    }

}

// Delete User
void User::delete_user(string id, string type) 
{

    User::display_header("Delete Equipment Screen");

    query = "SELECT * FROM user JOIN customer ON user.user_id = customer.user_id WHERE user_type = 'Customer' ORDER BY user.user_id DESC";
    qstate = mysql_query(Connection::conn, query.c_str());

    if (!qstate)
    {

        map<int, string> rowIdMap;
        int rowNum = 1;

        cout << left << setw(5) << "No." << setw(20) << "First Name" << setw(20) << "Last Name" << setw(30) << "Email" << setw(20) << "License No." << setw(20) << "Address" << setw(20) << "Status" << setw(20) << "Last Update" << endl;

        cout << setfill('-') << setw(150) << "" << endl;
        cout << setfill(' ');

        MYSQL_RES *res = mysql_store_result(Connection::conn);
        MYSQL_ROW row;

        while ((row = mysql_fetch_row(res)) != nullptr)
        {
            cout << left << setw(5) << rowNum
                << setw(20) << row[1]
                << setw(20) << row[2]
                << setw(30) << row[3]
                << setw(20) << row[10]
                << setw(20) << row[5]
                << setw(20) << row[11]
                << setw(20) << row[8]
                << endl;

            rowIdMap[rowNum] = row[0];

            rowNum++;
        }
        mysql_free_result(res);
        
        cout << setfill('-') << setw(115) << "" << endl;
        cout << setfill(' ');
        
        do
        {

            cout << "Enter the row No. of the user: ";
            string input;
            getline(cin >> ws, input);

            try {
                rowId = stoi(input);
            } catch(invalid_argument&) {
                cout << "--------------------------------------------" << endl;
                cout << "Invalid input! Please enter a valid row No." << endl;
                continue;
            } catch(out_of_range&) {
                cout << "--------------------------------------------" << endl;
                cout << "Invalid input! Please enter a valid row No." << endl;
                continue;
            }

            if (rowIdMap.count(rowId) > 0) 
            {

                User::display_header("Delete a User Screen");

                user_id = rowIdMap[rowId];

                query = "SELECT * FROM user JOIN customer ON user.user_id = customer.user_id WHERE user_type = 'Customer' AND user.user_id = '" + user_id + "'";
                qstate = mysql_query(Connection::conn, query.c_str());

                if (!qstate)
                {

                    cout << left << setw(20) << "First Name" << setw(20) << "Last Name" << setw(30) << "Email" << setw(20) << "License No." << setw(20) << "Address" << setw(20) << "Status" << setw(20) << "Last Update" << endl;

                    cout << setfill('-') << setw(150) << "" << endl;
                    cout << setfill(' ');

                    MYSQL_RES *res = mysql_store_result(Connection::conn);
                    MYSQL_ROW row;

                    while ((row = mysql_fetch_row(res)) != nullptr)
                    {
                        cout << left << setw(20) << row[1]
                            << setw(20) << row[2]
                            << setw(30) << row[3]
                            << setw(20) << row[10]
                            << setw(20) << row[5]
                            << setw(20) << row[11]
                            << setw(20) << row[8]
                            << endl;
                    }
                    mysql_free_result(res);
                    
                    cout << setfill('-') << setw(115) << "" << endl;
                    cout << setfill(' ');
                    
                    cout << "Are you sure do you want to delete this user? (Y/N)?" << endl;

                    do
                    {

                        cout << "--------------------------------------------" << endl;
                        cout << "Enter a your choice: ";
                        char choice;
                        cin >> ws >> choice;

                        if (choice == 'Y' || choice == 'y')
                        {


                            // Check if the customer type is being used in any rentals
                            query = "SELECT COUNT(customer_id) FROM customer WHERE user_id = '" + user_id + "' AND customer_id IN (SELECT customer_id FROM rental)";
                            qstate = mysql_query(Connection::conn, query.c_str());

                            res = mysql_store_result(Connection::conn);

                            if(mysql_num_rows(res) > 0)
                            {    
                                // mysql_free_result(res);

                                cout << "--------------------------------------------" << endl;
                                cout << "Cannot delete this customer as it is being used in some rentals!" << endl;
                                
                                this_thread::sleep_for(chrono::seconds(2));
                                user(id, type);
                            }
                            else
                            {
                                // mysql_free_result(res);

                                query = "DELETE user, customer FROM user JOIN customer ON user.user_id = customer.user_id WHERE user.user_id = '" + user_id + "'";
                                qstate = mysql_query(Connection::conn, query.c_str());

                                if (!qstate)
                                {
                                    cout << "--------------------------------------------" << endl;
                                    cout << "This user has been deleted successfully!" << endl;
                                    cout << "Please wait a moment..." << endl;

                                    this_thread::sleep_for(chrono::seconds(2));
                                    user(id, type);
                                }
                                else
                                {
                                    cout << "--------------------------------------------" << endl;
                                    cout << "Failed to execute the query! " << mysql_errno(Connection::conn) << endl;
                                    cout << "Please wait a moment..." << endl;

                                    this_thread::sleep_for(chrono::seconds(2));
                                    user(id, type);
                                }
                            }
                            mysql_free_result(res);
                            
                            break;
                        }
                        else if (choice == 'N' || choice == 'n')
                        {
                            user(id, type);
                            break;
                        }
                        else
                        {
                            cout << "--------------------------------------------" << endl;
                            cout << "You entered an invalid choice!" << endl;
                            continue;
                        }

                    } while (true);
                }
                else
                {
                    cout << "--------------------------------------------" << endl;
                    cout << "Failed to execute the query! " << mysql_errno(Connection::conn) << endl;
                    cout << "Please wait a moment..." << endl;

                    this_thread::sleep_for(chrono::seconds(2));
                    user(id, type);
                }
                break;
            }
            else
            {
                cout << "--------------------------------------------" << endl;
                cout << "You entered an invalid row No.!" << endl;
                continue;
            }         

        } while (true);
    }
    else
    {
        cout << "--------------------------------------------" << endl;
        cout << "Failed to execute the query! " << mysql_errno(Connection::conn) << endl;
        cout << "Please wait a moment..." << endl;

        this_thread::sleep_for(chrono::seconds(2));
        user(id, type);
    }
}

// need to cheek it again!
// Archive User
void User::archive_user(string id, string type) 
{

    map<int, string> rowIdMap;
    int rowNum = 1;

    Customer customer;

    display_header("Archive User Screen");

    cout << "1. Continue Archiving" << endl;
    cout << "2. Go back" << endl;

    do
    {
        cout << "--------------------------------------------" << endl;
        cout << "Enter a menu No.: ";
        string input;
        getline(cin >> ws, input);
        
        try {
            menu = stoi(input);
        } catch(invalid_argument&) {
            cout << "--------------------------------------------" << endl;
            cout << "Invalid input! Please enter a valid menu No." << endl;
            continue;
        } catch(out_of_range&) {
            cout << "--------------------------------------------" << endl;
            cout << "Invalid input! Please enter a valid menu No." << endl;
            continue;
        }

        if (menu == 1)
        {

            query = "SELECT * FROM user JOIN customer ON user.user_id = customer.user_id WHERE user_type = 'Customer' ORDER BY user.user_id DESC";
            qstate = mysql_query(Connection::conn, query.c_str());

            if (!qstate)
            {

                cout << left << setw(5) << "No." << setw(20) << "First Name" << setw(20) << "Last Name" << setw(30) << "Email" << setw(20) << "License No." << setw(20) << "Address" << setw(20) << "Status" << setw(20) << "Last Update" << endl;

                cout << setfill('-') << setw(150) << "" << endl;
                cout << setfill(' ');

                MYSQL_RES *res = mysql_store_result(Connection::conn);
                MYSQL_ROW row;

                while ((row = mysql_fetch_row(res)) != nullptr)
                {
                    cout << left << setw(5) << rowNum
                        << setw(20) << row[1]
                        << setw(20) << row[2]
                        << setw(30) << row[3]
                        << setw(20) << row[10]
                        << setw(20) << row[5]
                        << setw(20) << row[11]
                        << setw(20) << row[8]
                        << endl;

                    rowIdMap[rowNum] = row[0];

                    rowNum++;
                }
                mysql_free_result(res);

                cout << setfill('-') << setw(150) << "" << endl;
                cout << setfill(' ');

                cout << "Enter the row No. of the user: ";
                string input;
                getline(cin >> ws, input);

                try {
                    rowId = stoi(input);
                } catch(invalid_argument&) {
                    cout << "--------------------------------------------" << endl;
                    cout << "Invalid input! Please enter a valid row No." << endl;
                    continue;
                } catch(out_of_range&) {
                    cout << "--------------------------------------------" << endl;
                    cout << "Invalid input! Please enter a valid row No." << endl;
                    continue;
                }

                if (rowIdMap.count(rowId) > 0) 
                {
                    user_id = rowIdMap[rowId];

                    query = "SELECT * FROM user JOIN customer ON user.user_id = customer.user_id WHERE user_type = 'Customer' AND user.user_id = '" + user_id + "'";
                    qstate = mysql_query(Connection::conn, query.c_str());

                    if (!qstate)
                    {

                        MYSQL_RES *res = mysql_store_result(Connection::conn);
                        MYSQL_ROW row;

                        while ((row = mysql_fetch_row(res)) != nullptr)
                        {
                            customer.status = string(row[11]);

                            mysql_free_result(res);

                            customer.status = (customer.status == "Archived") ? "Unarchive" : "Archived";

                            query = "UPDATE customer SET status = '" + customer.status + "' WHERE user_id = '" + user_id + "'";
                            qstate = mysql_query(Connection::conn, query.c_str());


                            if (!qstate)
                            {
                                cout << "--------------------------------------------" << endl;
                                transform(customer.status.begin(), customer.status.end(), customer.status.begin(), ::tolower);
                                cout << "This user has been " << customer.status << " successfully!" << endl;
                                cout << "Please wait a moment..." << endl;

                                this_thread::sleep_for(

                                    chrono::seconds(2));
                                user(id, type);
                            }
                            else
                            {
                                cout << "--------------------------------------------" << endl;
                                cout << "Failed to update! " << mysql_errno(Connection::conn) << endl;
                                cout << "Please wait a moment..." << endl;

                                this_thread::sleep_for(

                                    chrono::seconds(2));
                                user(id, type);
                            }
                        }
                    
                    }
                    else 
                    {
                        cout << "--------------------------------------------" << endl;
                        cout << "Failed to execute the query! " << mysql_errno(Connection::conn) << endl;
                        cout << "Please wait a moment..." << endl;

                        this_thread::sleep_for(

                            chrono::seconds(2));
                        user(id, type);
                    }
                }
                else
                {
                    cout << "--------------------------------------------" << endl;
                    cout << "You entered an invalid row No.!" << endl;
                    cout << "Please wait a moment..." << endl;

                    this_thread::sleep_for(chrono::seconds(2));
                    archive_user(id, type);
                }
            }
            else
            {
                cout << "--------------------------------------------" << endl;
                cout << "Failed to execute the query! " << mysql_errno(Connection::conn) << endl;
                cout << "Please wait a moment..." << endl;

                this_thread::sleep_for(chrono::seconds(2));
                user(id, type);
            }

            break;

        }
        else if (menu == 2)
        {
            user(id, type);
            break;
        }
        else
        {
            cout << "--------------------------------------------" << endl;
            cout << "You entered an invalid menu No.!" << endl;
            continue;
        }

    } while (true);
}

// Search User
void User::find_user(string id, string type) 
{

    display_header("Search for Users Screen");

    cout << "1. Continue Searching" << endl;
    cout << "2. Go back" << endl;

    do
    {
        cout << "--------------------------------------------" << endl;
        cout << "Enter a menu No.: ";
        string input;
        getline(cin >> ws, input);

        try {
            menu = stoi(input);
        } catch(invalid_argument&) {
            cout << "--------------------------------------------" << endl;
            cout << "Invalid input! Please enter a valid menu No." << endl;
            continue;
        } catch(out_of_range&) {
            cout << "--------------------------------------------" << endl;
            cout << "Invalid input! Please enter a valid menu No." << endl;
            continue;
        }
    
        if (menu == 1)
        {
            search:
            {
                display_header("Search for a User Screen");

                do
                {
                    cout << "--------------------------------------------" << endl;
                    cout << "Enter the user email: ";
                    getline(cin >> ws, email);

                    if (email.empty())
                    {
                        cout << "--------------------------------------------" << endl;
                        cout << "Email cannot be empty! Please try again." << endl;
                        continue;
                    }
                    else 
                    {   

                        query = "SELECT * FROM user JOIN customer ON user.user_id = customer.user_id WHERE user_type = 'Customer' AND email LIKE '%" + email + "%'";
                        qstate = mysql_query(Connection::conn, query.c_str());

                        if (!qstate)
                        {

                            MYSQL_RES *res = mysql_store_result(Connection::conn);
                            MYSQL_ROW row;

                            if (mysql_num_rows(res) > 0)
                            {
                                
                                int rowNum = 1;

                                cout << left << setw(5) << "No." << setw(20) << "First Name" << setw(20) << "Last Name" << setw(30) << "Email" << setw(20) << "License No." << setw(20) << "Address" << setw(20) << "Status" << setw(20) << "Last Update" << endl;

                                cout << setfill('-') << setw(150) << "" << endl;
                                cout << setfill(' ');

                                while ((row = mysql_fetch_row(res)) != nullptr)
                                {
                                    cout << left << setw(5) << rowNum++
                                        << setw(20) << row[1]
                                        << setw(20) << row[2]
                                        << setw(30) << row[3]
                                        << setw(20) << row[10]
                                        << setw(20) << row[5]
                                        << setw(20) << row[11]
                                        << setw(20) << row[8]
                                        << endl;
                                }

                                mysql_free_result(res);
                                
                                cout << setfill('-') << setw(150) << "" << endl;
                                cout << setfill(' ');

                            }
                            else
                            {
                                mysql_free_result(res);

                                cout << "--------------------------------------------" << endl;
                                cout << "This user email is not found!" << endl;
                            }

                        }
                        else
                        {
                            cout << "--------------------------------------------" << endl;
                            cout << "Failed to execute the query! " << mysql_errno(Connection::conn) << endl;
                            cout << "Please wait a moment..." << endl;

                            this_thread::sleep_for(chrono::seconds(2));
                            user(id, type);
                        }

                        cout << "--------------------------------------------" << endl;
                        cout << "Do you want to try again? (Y/N)?" << endl;


                        do
                        {
                            cout << "--------------------------------------------" << endl;
                            cout << "Enter a your choice: ";
                            char choice;
                            cin >> ws >> choice;

                            if (choice == 'Y' || choice == 'y')
                            {
                                goto search;
                                break;
                            }
                            else if (choice == 'N' || choice == 'n')
                            {
                                user(id, type);
                                break;
                            }
                            else
                            {
                                cout << "--------------------------------------------" << endl;
                                cout << "You entered an invalid choice!" << endl;
                                continue;
                            }

                        } while (true);

                    }
                
                    break;
                
                } while (true);

            }
            
            break;
        }
        else if (menu == 2)
        {
            user(id, type);
            break;
        }
        else
        {
            cout << "--------------------------------------------" << endl;
            cout << "You entered an invalid menu No.!" << endl;
            continue;
        }

    } while (true);
}

// Profile Menu
void User::profile(string id, string type)
{

    display_header("Profile Screen");

    Customer customer;
    Staff staff;

    cout << "1. Edit your profile " << endl;
    cout << "2. Change your password" << endl;
    cout << "3. Go back" << endl;

    do {

        cout << "--------------------------------------------" << endl;
        cout << "Enter a menu No.: ";
        string input;
        getline(cin >> ws, input);

        try {
            menu = stoi(input);
        } catch(invalid_argument&) {
            cout << "--------------------------------------------" << endl;
            cout << "Invalid input! Please enter a valid menu No." << endl;
            continue;
        } catch(out_of_range&) {
            cout << "--------------------------------------------" << endl;
            cout << "Invalid input! Please enter a valid menu No." << endl;
            continue;
        }
        
        if (menu == 1)
        {
            edit_profile(id, type);
            break;
        }
        else if (menu == 2)
        {
            change_password(id, type);
            break;
        }
        else if (menu == 3)
        {
            if (type == "Customer") 
            {
                customer.customer(id, type);
            }
            else if (type == "Staff") 
            {
                staff.home(id, type);
            }
            break;
        }
        else
        {
            cout << "--------------------------------------------" << endl;
            cout << "You entered an invalid menu No.!" << endl;
            continue;
        }

    } while (true);

}

// need to cheek it again!
// Edit Profile
void User::edit_profile(string id, string type)
{
    display_header("Edit Profile Screen");

    Customer customer;
    Staff staff;

    if (type == "Customer") 
    {
        query = "SELECT * FROM user JOIN customer ON user.user_id = customer.user_id WHERE user.user_id = '" + id + "'";
        qstate = mysql_query(Connection::conn, query.c_str());

        if (!qstate)
        {

            cout << left << setw(20) << "First Name" << setw(20) << "Last Name" << setw(30) << "Email" << setw(20) << "License No." << setw(20) << "Address" << setw(20) << "Last Update" << endl;

            cout << setfill('-') << setw(150) << "" << endl;
            cout << setfill(' ');

            MYSQL_RES *res = mysql_store_result(Connection::conn);
            MYSQL_ROW row;

            while ((row = mysql_fetch_row(res)) != nullptr)
            {
                cout << left << setw(20) << row[1]
                    << setw(20) << row[2]
                    << setw(30) << row[3]
                    << setw(20) << row[10]
                    << setw(20) << row[5]
                    << setw(20) << row[8]
                    << endl;
            }
            mysql_free_result(res);

            cout << setfill('-') << setw(150) << "" << endl;
            cout << setfill(' ');

            cout << "1. Continue Editing" << endl;
            cout << "2. Go back" << endl;

            do
            {
                cout << "--------------------------------------------" << endl;
                cout << "Enter a menu No.: ";
                string input;
                getline(cin >> ws, input);

                try {
                    menu = stoi(input);
                } catch(invalid_argument&) {
                    cout << "--------------------------------------------" << endl;
                    cout << "Invalid input! Please enter a valid menu No." << endl;
                    continue;
                } catch(out_of_range&) {
                    cout << "--------------------------------------------" << endl;
                    cout << "Invalid input! Please enter a valid menu No." << endl;
                    continue;
                }
                
                if (menu == 1)
                {
                    display_header("Edit Profile Screen");

                    query = "SELECT * FROM user JOIN customer ON user.user_id = customer.user_id WHERE user.user_id = '" + id + "'";
                    qstate = mysql_query(Connection::conn, query.c_str());

                    if (!qstate)
                    {

                        cout << left << setw(20) << "First Name" << setw(20) << "Last Name" << setw(30) << "Email" << setw(20) << "License No." << setw(20) << "Address" << setw(20) << "Last Update" << endl;

                        cout << setfill('-') << setw(115) << "" << endl;
                        cout << setfill(' ');

                        MYSQL_RES *res = mysql_store_result(Connection::conn);
                        MYSQL_ROW row;

                        while ((row = mysql_fetch_row(res)) != nullptr)
                        {
                            cout << left << setw(20) << row[1]
                                << setw(20) << row[2]
                                << setw(30) << row[3]
                                << setw(20) << row[10]
                                << setw(20) << row[5]
                                << setw(20) << row[8]
                                << endl;
                        }
                        mysql_free_result(res);

                        cout << setfill('-') << setw(115) << "" << endl;
                        cout << setfill(' ');

                        cout << setfill('-') << setw(150) << "" << endl;
                        cout << "		>> Please let the value blank, if you wish to not update it <<" << endl;
                        cout << setfill('-') << setw(150) << "" << endl;

                        do
                        {
                            cout << "Enter your first name: ";
                            getline(cin >> ws, fname);

                            if (fname.empty())
                            {
                                fname = oldDate[1];
                                break;
                            }
                            else if (!is_string(fname))
                            {
                                cout << "--------------------------------------------" << endl;
                                cout << "Invalid first name! First name must contain only alphabetical characters." << endl;
                                continue;
                            }
                            else
                            {
                                break;
                            }

                        } while (true);

                        do
                        {
                            
                            cout << "--------------------------------------------" << endl;
                            cout << "Enter your last name: ";
                            getline(cin >> ws, lname);

                            if (lname.empty())
                            {
                                lname = oldDate[1];
                                break;
                            }
                            else if (!is_string(lname))
                            {
                                cout << "--------------------------------------------" << endl;
                                cout << "Invalid last name! Last name must contain only alphabetical characters." << endl;
                                continue;
                            }
                            else
                            {
                                break;
                            }

                        } while (true);

                        do
                        {
                            cout << "--------------------------------------------" << endl;
                            cout << "Enter your email: ";
                            getline(cin >> ws, email);

                            if (email.empty())
                            {
                                email = oldDate[1];
                                break;
                            }
                            else if (!is_valid_email(email))
                            {
                                cout << "--------------------------------------------" << endl;
                                cout << "Invalid email format! Please enter a valid email." << endl;
                                continue;
                            }
                            else
                            {
                                query = "SELECT email FROM user WHERE email = '" + email + "' AND user.user_id != '" + id + "'";
                                qstate = mysql_query(Connection::conn, query.c_str());

                                if (!qstate)
                                {

                                    MYSQL_RES *res = mysql_store_result(Connection::conn);
                                    MYSQL_ROW row;

                                    if (mysql_num_rows(res) > 0)
                                    {
                                        cout << "--------------------------------------------" << endl;
                                        cout << "This email is already exist!" << endl;
                                        continue;
                                    }
                                    else
                                    {
                                        break;   
                                    }
                                    mysql_free_result(res);
                                }
                                else
                                {
                                    cout << "--------------------------------------------" << endl;
                                    cout << "Failed to execute the query! " << mysql_errno(Connection::conn) << endl;
                                    cout << "Please wait a moment..." << endl;

                                    this_thread::sleep_for(chrono::seconds(2));
                                    if (type == "Customer") 
                                    {
                                        customer.customer(id, type);
                                    }
                                    else if (type == "Staff") 
                                    {
                                        staff.home(id, type);
                                    }
                                }
                            }

                        } while (true);

                        do
                        {
                            cout << "--------------------------------------------" << endl;
                            cout << "Enter your license No.: ";
                            getline(cin >> ws, customer.license_no);

                            if (customer.license_no.empty())
                            {
                                customer.license_no = oldDate[1];
                                break;
                            }
                            else if (!is_valid_license_no(customer.license_no))
                            {
                                cout << "--------------------------------------------" << endl;
                                cout << "Invalid license No.! License No. must be 8 characters long and contain only uppercase letters and digits." << endl;
                                continue;
                            }
                            else
                            {
                                break;
                            }

                        } while (true);

                        cout << "--------------------------------------------" << endl;
                        cout << "Enter your address: ";
                        getline(cin >> ws, address);

                        if (address.empty())
                        {
                            address = oldDate[3];
                        }

                        query = "UPDATE user JOIN customer ON user.user_id = customer.user_id SET user.fname='" + fname + "', user.lname = '" + lname + "', user.email = '" + email + "', user.address = '" + address + "', customer.license_no = '" + customer.license_no + "' WHERE user.user_id = '" + id + "'";
                        qstate = mysql_query(Connection::conn, query.c_str());

                        if (!qstate)
                        {
                            cout << "--------------------------------------------" << endl;
                            cout << "Your profile has been updated successfully!" << endl;
                            cout << "Please wait a moment..." << endl;

                            this_thread::sleep_for(chrono::seconds(2));

                            if (type == "Customer") 
                            {
                                customer.customer(id, type);
                            }
                            else if (type == "Staff") 
                            {
                                staff.home(id, type);
                            }
                        }
                        else
                        {
                            cout << "--------------------------------------------" << endl;
                            cout << "Failed to execute the query! " << mysql_errno(Connection::conn) << endl;
                            cout << "Please wait a moment..." << endl;

                            this_thread::sleep_for(chrono::seconds(2));

                            if (type == "Customer") 
                            {
                                customer.customer(id, type);;
                            }
                            else if (type == "Staff") 
                            {
                                staff.home(id, type);
                            }
                        }
                    }
                    else
                    {
                        cout << "--------------------------------------------" << endl;
                        cout << "Failed to execute the query! " << mysql_errno(Connection::conn) << endl;
                        cout << "Please wait a moment..." << endl;

                        this_thread::sleep_for(chrono::seconds(2));

                        if (type == "Customer") 
                        {
                            customer.customer(id, type);;
                        }
                        else if (type == "Staff") 
                        {
                            staff.home(id, type);
                        }
                    }
                    break;
                }
                else if (menu == 2)
                {
                    if (type == "Customer") 
                    {
                        customer.customer(id, type);;
                    }
                    else if (type == "Staff") 
                    {
                        staff.home(id, type);
                    }
                    break;
                }
                else
                {
                    cout << "--------------------------------------------" << endl;
                    cout << "You entered an invalid menu No.!" << endl;
                    continue;
                }

            } while (true);
        }
        else
        {
            cout << "--------------------------------------------" << endl;
            cout << "Failed to execute the query! " << mysql_errno(Connection::conn) << endl;
            cout << "Please wait a moment..." << endl;

            this_thread::sleep_for(chrono::seconds(2));

            if (type == "Customer") 
            {  
                customer.customer(id, type);;
            }
            else if (type == "Staff") 
            {
                staff.home(id, type);
            }
        }

    }
    else if (type == "Staff") 
    {
        query = "SELECT * FROM user WHERE user_type = '" + type + "' AND user.user_id = '" + id + "'";
        qstate = mysql_query(Connection::conn, query.c_str());

        if (!qstate)
        {

            cout << left << setw(20) << "First Name" << setw(20) << "Last Name" << setw(30) << "Email" << setw(20) << "Address" << setw(20) << "Last Update" << endl;

            cout << setfill('-') << setw(150) << "" << endl;
            cout << setfill(' ');

            MYSQL_RES *res = mysql_store_result(Connection::conn);
            MYSQL_ROW row;

            while ((row = mysql_fetch_row(res)) != nullptr)
            {
                cout << left << setw(20) << row[1]
                    << setw(20) << row[2]
                    << setw(30) << row[3]
                    << setw(20) << row[5]
                    << setw(20) << row[8]
                    << endl;
            }
            mysql_free_result(res);

            cout << setfill('-') << setw(150) << "" << endl;
            cout << setfill(' ');

            cout << "1. Continue Editing" << endl;
            cout << "2. Go back" << endl;

            do
            {
                cout << "--------------------------------------------" << endl;
                cout << "Enter a menu No.: ";
                string input;
                getline(cin >> ws, input);

                try {
                    menu = stoi(input);
                } catch(invalid_argument&) {
                    cout << "--------------------------------------------" << endl;
                    cout << "Invalid input! Please enter a valid menu No." << endl;
                    continue;
                } catch(out_of_range&) {
                    cout << "--------------------------------------------" << endl;
                    cout << "Invalid input! Please enter a valid menu No." << endl;
                    continue;
                }
                
                if (menu == 1)
                {
                    display_header("Edit Profile Screen");

                    query = "SELECT * FROM user WHERE user_type = '" + type + "' AND user.user_id = '" + id + "'";
                    qstate = mysql_query(Connection::conn, query.c_str());

                    if (!qstate)
                    {

                        cout << left << setw(20) << "First Name" << setw(20) << "Last Name" << setw(30) << "Email" << setw(20) << "Address" << setw(20) << "Last Update" << endl;

                        cout << setfill('-') << setw(115) << "" << endl;
                        cout << setfill(' ');

                        MYSQL_RES *res = mysql_store_result(Connection::conn);
                        MYSQL_ROW row;

                        while ((row = mysql_fetch_row(res)) != nullptr)
                        {
                            cout << left << setw(20) << row[1]
                                << setw(20) << row[2]
                                << setw(30) << row[3]
                                << setw(20) << row[5]
                                << setw(20) << row[8]
                                << endl;
                        }
                        mysql_free_result(res);

                        cout << setfill('-') << setw(115) << "" << endl;
                        cout << setfill(' ');

                        cout << setfill('-') << setw(150) << "" << endl;
                        cout << "		>> Please let the value blank, if you wish to not update it <<" << endl;
                        cout << setfill('-') << setw(150) << "" << endl;

                        do
                        {
                            cout << "Enter your first name: ";
                            getline(cin >> ws, fname);

                            if (fname.empty())
                            {
                                fname = oldDate[1];
                                break;
                            }
                            else if (!is_string(fname))
                            {
                                cout << "--------------------------------------------" << endl;
                                cout << "Invalid first name! First name must contain only alphabetical characters." << endl;
                                continue;
                            }
                            else
                            {
                                break;
                            }

                        } while (true);

                        do
                        {
                            
                            cout << "--------------------------------------------" << endl;
                            cout << "Enter your last name: ";
                            getline(cin >> ws, lname);

                            if (lname.empty())
                            {
                                lname = oldDate[1];
                                break;
                            }
                            else if (!is_string(lname))
                            {
                                cout << "--------------------------------------------" << endl;
                                cout << "Invalid last name! Last name must contain only alphabetical characters." << endl;
                                continue;
                            }
                            else
                            {
                                break;
                            }

                        } while (true);

                        do
                        {
                            cout << "--------------------------------------------" << endl;
                            cout << "Enter your email: ";
                            getline(cin >> ws, email);

                            if (email.empty())
                            {
                                email = oldDate[1];
                                break;
                            }
                            else if (!is_valid_email(email))
                            {
                                cout << "--------------------------------------------" << endl;
                                cout << "Invalid email format! Please enter a valid email." << endl;
                                continue;
                            }
                            else
                            {
                                query = "SELECT email FROM user WHERE email = '" + email + "' AND user.user_id != '" + id + "'";
                                qstate = mysql_query(Connection::conn, query.c_str());

                                if (!qstate)
                                {

                                    MYSQL_RES *res = mysql_store_result(Connection::conn);
                                    MYSQL_ROW row;

                                    if (mysql_num_rows(res) > 0)
                                    {
                                        
                                        //mysql_free_result(res);

                                        cout << "--------------------------------------------" << endl;
                                        cout << "This email is already exist!" << endl;
                                        continue;
                                    }
                                    else
                                    {
                                        break;
                                       
                                    }
                                    mysql_free_result(res);
                                }
                                else
                                {
                                    cout << "--------------------------------------------" << endl;
                                    cout << "Failed to execute the query! " << mysql_errno(Connection::conn) << endl;
                                    cout << "Please wait a moment..." << endl;

                                    this_thread::sleep_for(chrono::seconds(2));
                                    if (type == "Customer") 
                                    {
                                        customer.customer(id, type);
                                    }
                                    else if (type == "Staff") 
                                    {
                                        staff.home(id, type);
                                    }
                                }
                            }

                        } while (true);

                        cout << "--------------------------------------------" << endl;
                        cout << "Enter your address: ";
                        getline(cin >> ws, address);

                        if (address.empty())
                        {
                            address = oldDate[3];
                        }

                        query = "UPDATE user SET user.fname='" + fname + "', user.lname = '" + lname + "', user.email = '" + email + "', user.address = '" + address + "' WHERE user.user_id = '" + id + "'";
                        qstate = mysql_query(Connection::conn, query.c_str());

                        if (!qstate)
                        {
                            cout << "--------------------------------------------" << endl;
                            cout << "Your profile has been updated successfully!" << endl;
                            cout << "Please wait a moment..." << endl;

                            this_thread::sleep_for(chrono::seconds(2));

                            if (type == "Customer") 
                            {
                                customer.customer(id, type);
                            }
                            else if (type == "Staff") 
                            {
                                staff.home(id, type);
                            }
                        }
                        else
                        {
                            cout << "--------------------------------------------" << endl;
                            cout << "Failed to execute the query! " << mysql_errno(Connection::conn) << endl;
                            cout << "Please wait a moment..." << endl;

                            this_thread::sleep_for(chrono::seconds(2));

                            if (type == "Customer") 
                            {
                                customer.customer(id, type);;
                            }
                            else if (type == "Staff") 
                            {
                                staff.home(id, type);
                            }
                        }
                    }
                    else
                    {
                        cout << "--------------------------------------------" << endl;
                        cout << "Failed to execute the query! " << mysql_errno(Connection::conn) << endl;
                        cout << "Please wait a moment..." << endl;

                        this_thread::sleep_for(chrono::seconds(2));

                        if (type == "Customer") 
                        {
                            customer.customer(id, type);;
                        }
                        else if (type == "Staff") 
                        {
                            staff.home(id, type);
                        }
                    }
                    break;
                }
                else if (menu == 2)
                {
                    if (type == "Customer") 
                    {
                        customer.customer(id, type);;
                    }
                    else if (type == "Staff") 
                    {
                        staff.home(id, type);
                    }
                    break;
                }
                else
                {
                    cout << "--------------------------------------------" << endl;
                    cout << "You entered an invalid menu No.!" << endl;
                    continue;
                }

            } while (true);
        }
        else
        {
            cout << "--------------------------------------------" << endl;
            cout << "Failed to execute the query! " << mysql_errno(Connection::conn) << endl;
            cout << "Please wait a moment..." << endl;

            this_thread::sleep_for(chrono::seconds(2));

            if (type == "Customer") 
            {  
                customer.customer(id, type);;
            }
            else if (type == "Staff") 
            {
                staff.home(id, type);
            }
        }

    }
}

// Change Password 
void User::change_password(string id, string type)
{
    
    display_header("Change Password Screen");

    Customer customer;
    Staff staff;

    cout << "1. Continue Adding" << endl;
    cout << "2. Go back" << endl;

    do
    {
        cout << "--------------------------------------------" << endl;
        cout << "Enter a menu No.: ";
        string input;
        getline(cin >> ws, input);

        try {
            menu = stoi(input);
        } catch(invalid_argument&) {
            cout << "--------------------------------------------" << endl;
            cout << "Invalid input! Please enter a valid menu No." << endl;
            continue;
        } catch(out_of_range&) {
            cout << "--------------------------------------------" << endl;
            cout << "Invalid input! Please enter a valid menu No." << endl;
            continue;
        }
    
        if (menu == 1)
        {

            query = "SELECT password FROM user WHERE user_id = '" + id + "'";
            qstate = mysql_query(Connection::conn, query.c_str());

            if (!qstate)
            {

                change_password:

                {
                    cout << "--------------------------------------------" << endl;
                    cout << "Enter your current password: ";
                    string old_password;
                    getline(cin >> ws, old_password);

                    MYSQL_RES *res = mysql_store_result(Connection::conn);
                    MYSQL_ROW row;

                    if (mysql_num_rows(res) > 0)
                    {
                        while ((row = mysql_fetch_row(res)) != nullptr)
                        {
                            if (old_password != row[0])
                            {

                                mysql_free_result(res);

                                cout << "--------------------------------------------" << endl;
                                cout << "You entered a wrong password!" << endl;
                                cout << "Do you want to try again? (Y/N)?" << endl;

                                do
                                {
                                    cout << "--------------------------------------------" << endl;
                                    cout << "Enter a your choice: ";
                                    char choice;
                                    cin >> ws >> choice;

                                    if (choice == 'Y' || choice == 'y')
                                    {
                                        goto change_password;
                                        break;
                                    }
                                    else if (choice == 'N' || choice == 'n')
                                    {
                                        profile(id, type);
                                        break;
                                    }
                                    else
                                    {
                                        cout << "--------------------------------------------" << endl;
                                        cout << "You entered an invalid choice!" << endl;
                                        continue;
                                    }

                                } while (true);

                                break;
                            } 
                            else 
                            {
                                cout << "--------------------------------------------" << endl;
                                cout << "Enter your a new password: ";
                                string new_password;
                                getline(cin >> ws, new_password);


                                query = "UPDATE user SET password = '" + new_password + "' WHERE user_id = '" + id + "'";
                                qstate = mysql_query(Connection::conn, query.c_str());

                                if (!qstate)
                                {
                                    cout << "--------------------------------------------" << endl;
                                    cout << "Your password has been updated successfully!" << endl;
                                    cout << "Please wait a moment..." << endl;

                                    this_thread::sleep_for(chrono::seconds(2));
                                    profile(id, type);
                                }
                                else
                                {
                                    cout << "--------------------------------------------" << endl;
                                    cout << "Failed to execute the query! " << mysql_errno(Connection::conn) << endl;
                                    cout << "Please wait a moment..." << endl;

                                    this_thread::sleep_for(chrono::seconds(2));

                                    if (type == "Customer") 
                                    {
                                        customer.customer(id, type);;
                                    }
                                    else if (type == "Staff") 
                                    {
                                        staff.home(id, type);
                                    }
                                }
                                break;
                            }
                        }
                    }   
                
                }
            }
            else
            {
                cout << "--------------------------------------------" << endl;
                cout << "Failed to execute the query! " << mysql_errno(Connection::conn) << endl;
                cout << "Please wait a moment..." << endl;

                this_thread::sleep_for(chrono::seconds(2));
                
                if (type == "Customer") 
                {
                    customer.customer(id, type);;
                }
                else if (type == "Staff") 
                {
                    staff.home(id, type);
                }
            }
            break;
        }
        else if (menu == 2)
        {
            user(id, type);
            if (type == "Customer") 
            {
                customer.customer(id, type);;
            }
            else if (type == "Staff") 
            {
                staff.home(id, type);
            }
            break;
        }
        else
        {
            cout << "--------------------------------------------" << endl;
            cout << "You entered an invalid menu No.!" << endl;
            continue;
        }

    } while (true);
}
