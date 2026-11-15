#include "equipment_type.h"
#include "connection.h"
#include "user.h"
#include "staff.h"

using namespace std;

// string validation using regular expression
bool Equipment_Type::is_string(const string& str) 
{
    regex pattern("^[a-zA-Z]+$");
    return regex_match(str, pattern);
}

// type validation using regular expression
bool Equipment_Type::is_valid_type(const string& type_name)
{
    // Some examples of heavy equipment types might include:

    // Excavators: These are large, track-mounted machines that are used for digging, grading, and other earthmoving tasks.

    // Bulldozers: These are large, powerful machines that are used for pushing, leveling, and grading large areas of land.

    // Cranes: These are tall, heavy machines that are used for lifting and moving large loads, such as building materials, containers, and other heavy objects.

    // Backhoes: These are smaller, more versatile machines that are commonly used for digging and grading in tight spaces or on smaller construction sites.

    // Loaders: These are large machines that are used for loading and unloading materials, such as dirt, sand, and gravel.

    // Compactors: These are heavy machines that are used for compacting soil, asphalt, and other materials to create a stable base for construction projects.

    regex pattern("^(Excavators|Bulldozers|Cranes|Backhoes|Loaders|Compactors|Pavers|Scrapers|Dozers)$", regex_constants::icase);
    return regex_match(type_name, pattern);
}

// Equipment Type Module
void Equipment_Type::equipment_type(string id, string type) 
{

    User::display_header("Manage Equipment Types Screen");

    cout << "1. List equipment types" << endl;
    cout << "2. Add a new Equipment types" << endl;
    cout << "3. Edit an equipment type" << endl;
    cout << "4. Delete an equipment type" << endl;
    cout << "5. Search for an equipment type" << endl;
    cout << "6. Go back" << endl;

    do
    {
        cout << "--------------------------------------------" << endl;
        cout << "Enter a menu No.: ";
        string input;
        getline(cin >> ws, input);

        try {
            menu = stoi(input);
        } catch(std::invalid_argument&) {
            cout << "--------------------------------------------" << endl;
            cout << "Invalid input! Please enter a valid menu No." << endl;
            continue;
        } catch(std::out_of_range&) {
            cout << "--------------------------------------------" << endl;
            cout << "Invalid input! Please enter a valid menu No." << endl;
            continue;
        }

        if (menu == 1)
        {
            list_equipment_type(id, type);
            break;
        }
        else if (menu == 2)
        {
            add_equipment_type(id, type);
            break;
        }
        else if (menu == 3)
        {
            edit_equipment_type(id, type);
            break;
        }
        else if (menu == 4)
        {
            delete_equipment_type(id, type);
            break;
        }
        else if (menu == 5)
        {
            find_equipment_type(id, type);
            break;
        }
        else if (menu == 6)
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

// List Equipment Types
void Equipment_Type::list_equipment_type(string id, string type)
{

    User::display_header("List Equipment Types Screen");

    query = "SELECT * FROM equipment_type ORDER BY equipment_type_id DESC";
    qstate = mysql_query(Connection::conn, query.c_str());

    if (!qstate)
    {
        int rowNum = 1;

        cout << left << setw(5) << "No." << setw(20) << "Name" << setw(20) << "Last Update" << endl;

        cout << setfill('-') << setw(100) << "" << endl;
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
                    << endl;
            }
            mysql_free_result(res);

            cout << setfill('-') << setw(100) << "" << endl;
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
                } catch(std::invalid_argument&) {
                    cout << "--------------------------------------------" << endl;
                    cout << "Invalid input! Please enter a valid menu No." << endl;
                    continue;
                } catch(std::out_of_range&) {
                    cout << "--------------------------------------------" << endl;
                    cout << "Invalid input! Please enter a valid menu No." << endl;
                    continue;
                }

                if (menu == 1)
                {
                    equipment_type(id, type);
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
            cout << "There is no equipment type to list!" << endl;
            cout << "Please wait a moment..." << endl;
            this_thread::sleep_for(chrono::seconds(2));
            equipment_type(id, type);
        }
    }
    else
    {
        cout << "--------------------------------------------" << endl;
        cout << "Failed to execute the query! " << mysql_errno(Connection::conn) << endl;
        cout << "Please wait a moment..." << endl;

        this_thread::sleep_for(chrono::seconds(2));
        equipment_type(id, type);
    }
}

// Add New Equipment Types
void Equipment_Type::add_equipment_type(string id, string type)
{

    User::display_header("Add a New Equipment Types Screen");

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
        } catch(std::invalid_argument&) {
            cout << "--------------------------------------------" << endl;
            cout << "Invalid input! Please enter a valid menu No." << endl;
            continue;
        } catch(std::out_of_range&) {
            cout << "--------------------------------------------" << endl;
            cout << "Invalid input! Please enter a valid menu No." << endl;
            continue;
        }

        if (menu == 1)
        {

            do 
            {
                cout << "--------------------------------------------" << endl;
                cout << "Enter the equipment type name: ";
                getline(cin >> ws, type_name);

                if (type_name.empty()) {
                    cout << "--------------------------------------------" << endl;
                    cout << "Equipment type name cannot be empty! Please try again." << endl;
                    continue;
                }
                else if (!is_valid_type(type_name))
                {
                    cout << "--------------------------------------------" << endl;
                    cout << "Invalid equipment type name! Equipment type name should be one of these options Excavators|Bulldozers|Cranes|Backhoes|Loaders|Compactors|Pavers|Scrapers|Dozers." << endl;
                    continue;
                }
                else 
                {
                    break;
                }

            } while (true);

            query = "INSERT INTO equipment_type (type_name) VALUES ('" + type_name + "')";
            qstate = mysql_query(Connection::conn, query.c_str());

            if (!qstate)
            {
                cout << "--------------------------------------------" << endl;
                cout << "This equipment type has been added successfully!" << endl;
                cout << "Please wait a moment..." << endl;

                this_thread::sleep_for(chrono::seconds(2));
                equipment_type(id, type);
            }
            else
            {
                cout << "--------------------------------------------" << endl;
                cout << "Failed to execute the query! " << mysql_errno(Connection::conn) << endl;
                cout << "Please wait a moment..." << endl;

                this_thread::sleep_for(chrono::seconds(2));
                equipment_type(id, type);
            }
            break;
        }
        else if (menu == 2)
        {
            equipment_type(id, type);
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

// Edit Equipment Type
void Equipment_Type::edit_equipment_type(string id, string type)
{

    User::display_header("Edit Equipment Types Screen");

    query = "SELECT * FROM equipment_type ORDER BY equipment_type_id DESC";
    qstate = mysql_query(Connection::conn, query.c_str());

    if (!qstate)
    {

        map<int, string> rowIdMap;
        int rowNum = 1;

        cout << left << setw(5) << "No." << setw(20) << "Name" << setw(20) << "Last Update" << endl;

        cout << setfill('-') << setw(100) << "" << endl;
        cout << setfill(' ');

        MYSQL_RES *res = mysql_store_result(Connection::conn);
        MYSQL_ROW row;

        while ((row = mysql_fetch_row(res)) != nullptr)
        {
            cout << left << setw(5) << rowNum
                << setw(20) << row[1]
                << setw(20) << row[2]
                << endl;

            rowIdMap[rowNum] = row[0];
            
            rowNum++;
        }
        mysql_free_result(res);

        cout << setfill('-') << setw(100) << "" << endl;
        cout << setfill(' ');
        
        do
        {
            cout << "Enter the row No. of the equipment type: ";
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

                User::display_header("Edit an Equipment Type Screen");

                equipment_type_id = rowIdMap[rowId];

                query = "SELECT * FROM equipment_type WHERE equipment_type_id = '" + equipment_type_id + "'";
                qstate = mysql_query(Connection::conn, query.c_str());

                if (!qstate)
                {
                    cout << left << setw(20) << "Name" << setw(20) << "Last Update" << endl;

                    cout << setfill('-') << setw(100) << "" << endl;
                    cout << setfill(' ');

                    MYSQL_RES *res = mysql_store_result(Connection::conn);
                    MYSQL_ROW row;

                    while ((row = mysql_fetch_row(res)) != nullptr)
                    {
                        cout << left << setw(20) << row[1]
                            << setw(20) << row[2]
                            << endl;   
                    }
                    mysql_free_result(res);

                    cout << setfill('-') << setw(100) << "" << endl;
                    cout << setfill(' ');

                    cout << "Are you sure do you want to edit this equipment type? (Y/N)?" << endl;


                    do
                    {
                        cout << "--------------------------------------------" << endl;
                        cout << "Enter a your choice: ";
                        char choice;
                        cin >> ws >> choice;

                        if (choice == 'Y' || choice == 'y')
                        {

                            cout << setfill('-') << setw(115) << "" << endl;
                            cout << setfill(' ');
                            cout << "		>> Please let the value blank, if you wish to not update it <<" << endl;
                            cout << setfill('-') << setw(115) << "" << endl;
                            cout << setfill(' ');

                            do 
                            {

                                cout << "Enter the equipment type name: ";
                                getline(cin >> ws, type_name);

                                if (type_name.empty()) {
                                    type_name = oldDate[0];
                                    break;
                                }
                                else if (!is_valid_type(type_name))
                                {
                                    cout << "--------------------------------------------" << endl;
                                    cout << "Invalid equipment type name! Equipment type name should be one of these options Excavators|Bulldozers|Cranes|Backhoes|Loaders|Compactors|Pavers|Scrapers|Dozers." << endl;
                                    continue;
                                }
                                else 
                                {
                                    break;
                                }

                            } while (true);

                            query = "UPDATE equipment_type SET type_name = '" + type_name + "' WHERE equipment_type_id = '" + equipment_type_id + "'";
                            qstate = mysql_query(Connection::conn, query.c_str());

                            if (!qstate)
                            {
                                cout << "--------------------------------------------" << endl;
                                cout << "This equipment type has been updated successfully!" << endl;
                                cout << "Please wait a moment..." << endl;

                                this_thread::sleep_for(chrono::seconds(2));
                                equipment_type(id, type);
                            }
                            else
                            {
                                cout << "--------------------------------------------" << endl;
                                cout << "Failed to execute the query! " << mysql_errno(Connection::conn) << endl;
                                cout << "Please wait a moment..." << endl;

                                this_thread::sleep_for(chrono::seconds(2));
                                equipment_type(id, type);
                            }
                            break;
                        }
                        else if (choice == 'N' || choice == 'n')
                        {
                            equipment_type(id, type);
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
                    equipment_type(id, type);
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
        equipment_type(id, type);
    }
}

// Delete Equipment Type
void Equipment_Type::delete_equipment_type(string id, string type)
{

    User::display_header("Delete Equipment Types Screen");

    query = "SELECT * FROM equipment_type ORDER BY equipment_type_id DESC";
    qstate = mysql_query(Connection::conn, query.c_str());

    if (!qstate)
    {

        map<int, string> rowIdMap;
        int rowNum = 1;

        cout << left << setw(5) << "No." << setw(20) << "Name" << setw(20) << "Last Update" << endl;

        cout << setfill('-') << setw(100) << "" << endl;
        cout << setfill(' ');

        MYSQL_RES *res = mysql_store_result(Connection::conn);
        MYSQL_ROW row;

        while ((row = mysql_fetch_row(res)) != nullptr)
        {
            cout << left << setw(5) << rowNum
                << setw(20) << row[1]
                << setw(20) << row[2]
                << endl;

            rowIdMap[rowNum] = row[0];
            
            rowNum++;
        }
        mysql_free_result(res);
        
        cout << setfill('-') << setw(100) << "" << endl;
        cout << setfill(' ');
        
        do
        {

            cout << "Enter the row No. of the equipment type: ";
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
                
                User::display_header("Delete an Equipment Type Screen");
                
                equipment_type_id = rowIdMap[rowId];

                query = "SELECT * FROM equipment_type WHERE equipment_type_id = '" + equipment_type_id + "'";
                qstate = mysql_query(Connection::conn, query.c_str());

                if (!qstate)
                {

                    cout << left << setw(20) << "Name" << setw(20) << "Last Update" << endl;

                    cout << setfill('-') << setw(100) << "" << endl;
                    cout << setfill(' ');

                    MYSQL_RES *res = mysql_store_result(Connection::conn);
                    MYSQL_ROW row;

                    while ((row = mysql_fetch_row(res)) != nullptr)
                    {
                        cout << left << setw(20) << row[1]
                            << setw(20) << row[2]
                            << endl;
                    }
                    mysql_free_result(res);

                    cout << setfill('-') << setw(100) << "" << endl;
                    cout << setfill(' ');

                    cout << "Are you sure do you want to delete this equipment type? (Y/N)?" << endl;

                    do
                    {
                        cout << "--------------------------------------------" << endl;
                        cout << "Enter a your choice: ";
                        char choice;
                        cin >> ws >> choice;

                        if (choice == 'Y' || choice == 'y')
                        {
                            // Check if the equipment type is being used in any equipment
                            query = "SELECT COUNT(equipment_id) FROM equipment WHERE equipment_type_id = '" + equipment_type_id + "'";
                            qstate = mysql_query(Connection::conn, query.c_str());

                            res = mysql_store_result(Connection::conn);

                            if(mysql_num_rows(res) > 0)
                            {    
                                // mysql_free_result(res);

                                cout << "--------------------------------------------" << endl;
                                cout << "Cannot delete this equipment type as it is being used in some equipment!" << endl;
                                
                                this_thread::sleep_for(chrono::seconds(2));
                                equipment_type(id, type);
                            }
                            else
                            {
                                // mysql_free_result(res);

                                query = "DELETE FROM equipment_type WHERE equipment_type_id = '" + equipment_type_id + "'";
                                qstate = mysql_query(Connection::conn, query.c_str());

                                if (!qstate)
                                {
                                    cout << "--------------------------------------------" << endl;
                                    cout << "This equipment type has been deleted successfully!" << endl;
                                    cout << "Please wait a moment..." << endl;

                                    this_thread::sleep_for(chrono::seconds(2));
                                    equipment_type(id, type);
                                }
                                else
                                {
                                    cout << "--------------------------------------------" << endl;
                                    cout << "Failed to execute the query! " << mysql_errno(Connection::conn) << endl;
                                    cout << "Please wait a moment..." << endl;

                                    this_thread::sleep_for(chrono::seconds(2));
                                    equipment_type(id, type);
                                }
                            }
                            mysql_free_result(res);

                            break;
                        }
                        else if (choice == 'N' || choice == 'n')
                        {
                            equipment_type(id, type);
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
                    equipment_type(id, type);
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
        equipment_type(id, type);
    }
}

// Search Equipment Type
void Equipment_Type::find_equipment_type(string id, string type)
{

    User::display_header("Search for Equipment Type Screen");

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
        } catch(std::invalid_argument&) {
            cout << "--------------------------------------------" << endl;
            cout << "Invalid input! Please enter a valid menu No." << endl;
            continue;
        } catch(std::out_of_range&) {
            cout << "--------------------------------------------" << endl;
            cout << "Invalid input! Please enter a valid menu No." << endl;
            continue;
        }
    
        if (menu == 1)
        {
            search:
            {
                User::display_header("Search for an Equipment Type Screen");

                do
                {
                    cout << "--------------------------------------------" << endl;
                    cout << "Enter the equipment type name: ";
                    getline(cin >> ws, type_name);

                    if (type_name.empty()) {
                        cout << "--------------------------------------------" << endl;
                        cout << "Equipment type name cannot be empty! Please try again." << endl;
                        continue;
                    }
                    else 
                    {
                        query = "SELECT * FROM equipment_type WHERE type_name LIKE '%" + type_name + "%'";
                        qstate = mysql_query(Connection::conn, query.c_str());

                        if (!qstate)
                        {

                            MYSQL_RES *res = mysql_store_result(Connection::conn);
                            MYSQL_ROW row;

                            if (mysql_num_rows(res) > 0)
                            {
                                int rowNum = 1;

                                cout << left << setw(5) << "No." << setw(20) << "Name" << setw(20) << "Last Update" << endl;

                                cout << setfill('-') << setw(100) << "" << endl;
                                cout << setfill(' ');

                                while ((row = mysql_fetch_row(res)) != nullptr)
                                {
                                    cout << left << setw(5) << rowNum++
                                        << setw(20) << row[1]
                                        << setw(20) << row[2]
                                        << endl;
                                }
                                mysql_free_result(res);

                                cout << setfill('-') << setw(100) << "" << endl;
                                cout << setfill(' ');
                            }
                            else
                            {
                                cout << "--------------------------------------------" << endl;
                                cout << "This equipment type name is not found!" << endl;
                            }

                        }
                        else
                        {
                            cout << "--------------------------------------------" << endl;
                            cout << "Failed to execute the query! " << mysql_errno(Connection::conn) << endl;
                            cout << "Please wait a moment..." << endl;

                            this_thread::sleep_for(chrono::seconds(2));
                            equipment_type(id, type);
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
                                equipment_type(id, type);
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
            equipment_type(id, type);
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

