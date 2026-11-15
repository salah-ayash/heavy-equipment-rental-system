#include "connection.h"
#include "equipment.h"
#include "equipment_type.h"
#include "user.h"
#include "customer.h"
#include "staff.h"
#include "rent.h"

using namespace std;

// model validation using regular expression
bool Equipment::is_valid_model(const string &model)
{
    regex pattern("^[a-zA-Z0-9\\s]+$");
    return regex_match(model, pattern);
}

// price validation using regular expression
bool Equipment::is_valid_price(const string &price)
{
    regex pattern("^\\d{0,8}(\\.\\d{1,4})?$");
    return regex_match(price, pattern);
}

// status validation using regular expression
bool Equipment::is_valid_status(const string &status)
{
    regex pattern("^(Available|Unavailable|Reserved|Ranted)$", regex_constants::icase);
    return regex_match(status, pattern);
}

// Equipment Module
void Equipment::equipment(string id, string type) 
{
    
    User::display_header("Manage Heavy Equipment Screen");

    cout << "1. List equipment" << endl;
    cout << "2. Add a new equipment" << endl;
    cout << "3. Edit an equipment" << endl;
    cout << "4. Delete an equipment" << endl;
    cout << "5. Search for an equipment" << endl;
    cout << "6. Go back" << endl;

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
            list_equipment(id, type);
            break;
        }
        else if (menu == 2)
        {
            add_equipment(id, type);
            break;
        }
        else if (menu == 3)
        {
            edit_equipment(id, type);
            break;
        }
        else if (menu == 4)
        {
            delete_equipment(id, type);
            break;
        }
        else if (menu == 5)
        {
            find_equipment(id, type);
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

// List Equipment
void Equipment::list_equipment(string id, string type)
{

    User::display_header("List Equipment Screen");

    query = "SELECT * FROM equipment JOIN equipment_type ON equipment.equipment_type_id = equipment_type.equipment_type_id ORDER BY equipment_id DESC";
    qstate = mysql_query(Connection::conn, query.c_str());

    if (!qstate)
    {
        int rowNum = 1;

        cout << left << setw(5) << "No." << setw(20) << "Model" << setw(20) << "Color" << setw(20) << "Type" << setw(14) << "Price" << setw(20) << "Status" << setw(20) << "Last Update" << endl;

        cout << setfill('-') << setw(115) << "" << endl;
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
                    << setw(20) << row[8]
                    << setw(4) << "MYR" << setw(10) << fixed << setprecision(2) << atof(row[3])
                    << setw(20) << row[4]
                    << setw(20) << row[5]
                    << endl;
            }
            mysql_free_result(res);

            cout << setfill('-') << setw(115) << "" << endl;
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
                    equipment(id, type);
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
            cout << "There is no equipment to list!" << endl;
            cout << "Please wait a moment..." << endl;
            
            this_thread::sleep_for(chrono::seconds(2));
            equipment(id, type);
        }

    }
    else
    {
        cout << "--------------------------------------------" << endl;
        cout << "Failed to execute the query! " << mysql_errno(Connection::conn) << endl;
        cout << "Please wait a moment..." << endl;

        this_thread::sleep_for(chrono::seconds(2));
        equipment(id, type);
    }
}

// Add New Equipment
void Equipment::add_equipment(string id, string type)
{

    User::display_header("Add a New Equipment Screen");

    Equipment_Type equipment_type;
    User user;

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
                cout << "Enter the equipment model: ";
                getline(cin >> ws, model);

                if (model.empty())
                {
                    cout << "--------------------------------------------" << endl;
                    cout << "Equipment model cannot be empty! Please try again." << endl;
                    continue;
                }
                else if (!is_valid_model(model))
                {
                    cout << "--------------------------------------------" << endl;
                    cout << "Invalid equipment model! Equipment model must contain only letters and numbers."  << endl;
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
                cout << "Enter the equipment color: ";
                getline(cin >> ws, color);

                if (color.empty())
                {
                    cout << "--------------------------------------------" << endl;
                    cout << "Equipment color cannot be empty! Please try again." << endl;
                    continue;
                }
                else if (!user.is_string(color))
                {
                    cout << "--------------------------------------------" << endl;
                    // cout << "Invalid equipment color! Equipment color should be in the #FFFFFF format." << endl;
                    cout << "Invalid equipment color! Equipment color must contain only alphabetical characters." << endl;
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
                cout << "Enter the equipment price: ";
                getline(cin >> ws, price);

                if (price.empty())
                {
                    cout << "--------------------------------------------" << endl;
                    cout << "Equipment price cannot be empty! Please try again." << endl;
                    continue;
                }
                else if (!is_valid_price(price))
                {
                    cout << "--------------------------------------------" << endl;
                    cout << "Invalid price format! Please enter a valid price" << endl;
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
                cout << "Enter the equipment type: ";
                getline(cin >> ws, equipment_type.type_name);

                if (equipment_type.type_name.empty())
                {
                    cout << "--------------------------------------------" << endl;
                    cout << "Equipment type cannot be empty! Please try again." << endl;
                    continue;
                }
                else if (!equipment_type.is_valid_type(equipment_type.type_name))
                {
                    cout << "--------------------------------------------" << endl;
                    cout << "Invalid equipment type! Equipment type should be one of these options Excavators|Bulldozers|Cranes|Backhoes|Loaders|Compactors|Pavers|Scrapers|Dozers." << endl;
                    continue;
                }
                else 
                {
                    break;
                }

            } while (true);

            query = "SELECT equipment_type_id FROM equipment_type WHERE type_name = '" + equipment_type.type_name + "'";
            qstate = mysql_query(Connection::conn, query.c_str());

            if (!qstate)
            {

                MYSQL_RES *res = mysql_store_result(Connection::conn);
                MYSQL_ROW row;

                while ((row = mysql_fetch_row(res)) != nullptr)
                {
                    string equipment_type_id = row[0];

                    mysql_free_result(res);

                    query = "INSERT INTO equipment (model, color, price, equipment_type_id) VALUES ('" + model + "','" + color + "','" + price + "','" + equipment_type_id + "')";
                    qstate = mysql_query(Connection::conn, query.c_str());

                    if (!qstate)
                    {
                        cout << "--------------------------------------------" << endl;
                        cout << "This equipment has been added successfully!" << endl;
                        cout << "Please wait a moment..." << endl;

                        this_thread::sleep_for(chrono::seconds(2));
                        equipment(id, type);
                    }
                    else
                    {
                        cout << "--------------------------------------------" << endl;
                        cout << "Failed to execute the query! " << mysql_errno(Connection::conn) << endl;
                        cout << "Please wait a moment..." << endl;

                        this_thread::sleep_for(chrono::seconds(2));
                        equipment(id, type);
                    }

                   
                }
            }
            else
            {
                cout << "--------------------------------------------" << endl;
                cout << "Failed to execute the query! " << mysql_errno(Connection::conn) << endl;
                cout << "Please wait a moment..." << endl;

                this_thread::sleep_for(chrono::seconds(2));
                equipment(id, type);
            }
            break;
        }
        else if (menu == 2)
        {
            equipment(id, type);
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

// Edit Equipment
void Equipment::edit_equipment(string id, string type)
{

    User::display_header("Edit Equipment Screen");

    Equipment_Type equipment_type;
    User user;

    query = "SELECT * FROM equipment JOIN equipment_type ON equipment.equipment_type_id = equipment_type.equipment_type_id ORDER BY equipment_id DESC";
    qstate = mysql_query(Connection::conn, query.c_str());

    if (!qstate)
    {

        int rowNum = 1;

        cout << left << setw(5) << "No." << setw(20) << "Model" << setw(20) << "Color" << setw(20) << "Type" << setw(14) << "Price" << setw(20) << "Status" << setw(20) << "Last Update" << endl;

        cout << setfill('-') << setw(115) << "" << endl;
        cout << setfill(' ');

        MYSQL_RES *res = mysql_store_result(Connection::conn);
        MYSQL_ROW row;

        while ((row = mysql_fetch_row(res)) != nullptr)
        {
            cout << left << setw(5) << rowNum
                << setw(20) << row[1]
                << setw(20) << row[2]
                << setw(20) << row[8]
                << setw(4) << "MYR" << setw(10) << fixed << setprecision(2) << atof(row[3])
                << setw(20) << row[4]
                << setw(20) << row[5]
                << endl;
        }
        mysql_free_result(res);

        cout << setfill('-') << setw(115) << "" << endl;
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

                User::display_header("Edit Equipment Screen");

                map<int, string> rowIdMap;
                int rowNum = 1;

                query = "SELECT * FROM equipment JOIN equipment_type ON equipment.equipment_type_id = equipment_type.equipment_type_id ORDER BY equipment_id DESC";
                qstate = mysql_query(Connection::conn, query.c_str());

                if (!qstate)
                {

                    cout << left << setw(5) << "No." << setw(20) << "Model" << setw(20) << "Color" << setw(20) << "Type" << setw(14) << "Price" << setw(20) << "Status" << setw(20) << "Last Update" << endl;

                    cout << setfill('-') << setw(115) << "" << endl;
                    cout << setfill(' ');

                    MYSQL_RES *res = mysql_store_result(Connection::conn);
                    MYSQL_ROW row;

                    while ((row = mysql_fetch_row(res)) != nullptr)
                    {
                        cout << left << setw(5) << rowNum
                            << setw(20) << row[1]
                            << setw(20) << row[2]
                            << setw(20) << row[8]
                            << setw(4) << "MYR" << setw(10) << fixed << setprecision(2) << atof(row[3])
                            << setw(20) << row[4]
                            << setw(20) << row[5]
                            << endl;

                        rowIdMap[rowNum] = row[0];
                        
                        rowNum++;
                    }
                    mysql_free_result(res);

                    cout << setfill('-') << setw(115) << "" << endl;
                    cout << setfill(' ');

                    do
                    {
                        cout << "Enter the row No. of the equipment: ";
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

                            User::display_header("Edit an Equipment Screen");

                            equipment_id = rowIdMap[rowId];

                            query = "SELECT * FROM equipment JOIN equipment_type ON equipment.equipment_type_id = equipment_type.equipment_type_id WHERE equipment_id = '" + equipment_id + "'";
                            qstate = mysql_query(Connection::conn, query.c_str());

                            if (!qstate)
                            {

                                cout << left << setw(20) << "Model" << setw(20) << "Color" << setw(20) << "Type" << setw(14) << "Price" << setw(20) << "Status" << setw(20) << "Last Update" << endl;
                        
                                cout << setfill('-') << setw(115) << "" << endl;
                                cout << setfill(' ');

                                MYSQL_RES *res = mysql_store_result(Connection::conn);
                                MYSQL_ROW row;

                                while ((row = mysql_fetch_row(res)) != nullptr)
                                {
                                    cout << left << setw(20) << row[1]
                                        << setw(20) << row[2]
                                        << setw(20) << row[8]
                                        << setw(4) << "MYR" << setw(10) << fixed << setprecision(2) << atof(row[3])
                                        << setw(20) << row[4]
                                        << setw(20) << row[5]
                                        << endl;   
                                }
                                mysql_free_result(res);

                                cout << setfill('-') << setw(115) << "" << endl;
                                cout << setfill(' ');

                                cout << "Are you sure do you want to edit this equipment? (Y/N)?" << endl;

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

                                            cout << "Enter the equipment model: ";
                                            getline(cin >> ws, model);

                                            if (model.empty())
                                            {
                                                model = oldDate[0];
                                                break;;
                                            }
                                            else if (!is_valid_model(model))
                                            {
                                                cout << "--------------------------------------------" << endl;
                                                cout << "Invalid equipment model! Equipment model must contain only letters and numbers." << endl;
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
                                            cout << "Enter the equipment color: ";
                                            getline(cin >> ws, color);

                                            if (color.empty())
                                            {
                                                color = oldDate[1];
                                                break;;
                                            }
                                            else if (!user.is_string(color))
                                            {
                                                cout << "--------------------------------------------" << endl;
                                                // cout << "Invalid equipment color! Equipment color should be in the #FFFFFF format." << endl;
                                                cout << "Invalid equipment color! Equipment color must contain only alphabetical characters." << endl;
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
                                            cout << "Enter the equipment price: ";
                                            getline(cin >> ws, price);

                                            if (price.empty())
                                            {
                                                price = oldDate[2];
                                                break;
                                            }
                                            else if (!is_valid_price(price))
                                            {
                                                cout << "--------------------------------------------" << endl;
                                                cout << "Invalid price format! Please enter a valid price" << endl;
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
                                            cout << "Enter the equipment status: ";
                                            getline(cin >> ws, status);

                                            if (status.empty())
                                            {
                                                status = oldDate[3];
                                                break;
                                            }
                                            else if (!is_valid_status(status))
                                            {
                                                cout << "--------------------------------------------" << endl;
                                                cout << "Invalid equipment status! Equipment status should be one of these options Available|Unavailable|Reserved|Ranted." << endl;
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
                                            cout << "Enter the equipment type: ";
                                            getline(cin >> ws, equipment_type.type_name);

                                            if (equipment_type.type_name.empty())
                                            {
                                                equipment_type.type_name = oldDate[4];
                                                break;
                                            }
                                            else if (!equipment_type.is_valid_type(equipment_type.type_name))
                                            {
                                                cout << "--------------------------------------------" << endl;
                                                cout << "Invalid equipment type! Equipment type should be one of these options Excavators|Bulldozers|Cranes|Backhoes|Loaders|Compactors|Pavers|Scrapers|Dozers." << endl;
                                                continue;
                                            }
                                            else 
                                            {
                                                break;
                                            }

                                        } while (true);

                                        query = "SELECT equipment_type_id FROM equipment_type WHERE type_name = '" + equipment_type.type_name + "'";
                                        qstate = mysql_query(Connection::conn, query.c_str());

                                        if (!qstate)
                                        {

                                            MYSQL_RES *res = mysql_store_result(Connection::conn);
                                            MYSQL_ROW row;

                                            while ((row = mysql_fetch_row(res)) != nullptr)
                                            {
                                                string equipment_type_id = row[0];

                                                mysql_free_result(res);

                                                query = "UPDATE equipment SET model = '" + model + "', color = '" + color + "', price = '" + price + "', status = '" + status + "', equipment_type_id = '" + equipment_type_id + "' WHERE equipment_id = '" + equipment_id + "'";
                                                qstate = mysql_query(Connection::conn, query.c_str());

                                                if (!qstate)
                                                {
                                                    cout << "--------------------------------------------" << endl;
                                                    cout << "This equipment has been updated successfully!" << endl;
                                                    cout << "Please wait a moment..." << endl;

                                                    this_thread::sleep_for(chrono::seconds(2));
                                                    equipment(id, type);
                                                }
                                                else
                                                {
                                                    cout << "--------------------------------------------" << endl;
                                                    cout << "Failed to execute the query! " << mysql_errno(Connection::conn) << endl;
                                                    cout << "Please wait a moment..." << endl;

                                                    this_thread::sleep_for(chrono::seconds(2));
                                                    equipment(id, type);
                                                }

                                            }

                                        }
                                        else
                                        {
                                            cout << "--------------------------------------------" << endl;
                                            cout << "Failed to execute the query! " << mysql_errno(Connection::conn) << endl;
                                            cout << "Please wait a moment..." << endl;

                                            this_thread::sleep_for(chrono::seconds(2));
                                            equipment(id, type);
                                        }
                                        break;

                                    }
                                    else if (choice == 'N' || choice == 'n')
                                    {
                                        equipment(id, type);
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
                                equipment(id, type);
                            }
                            break;
                        }
                        else
                        {
                            cout << "--------------------------------------------" << endl;
                            cout << "Please enter a valid row No.!" << endl;
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
                    equipment(id, type);
                }
            }
            else if (menu == 2)
            {
                equipment(id, type);
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
        equipment(id, type);
    }

}

// Delete Equipment
void Equipment::delete_equipment(string id, string type)
{

    User::display_header("Delete Equipment Screen");

    query = "SELECT * FROM equipment ORDER BY equipment_id DESC";
    qstate = mysql_query(Connection::conn, query.c_str());

    if (!qstate)
    {

        map<int, string> rowIdMap;
        int rowNum = 1;

        cout << left << setw(5) << "No." << setw(20) << "Model" << setw(20) << "Color" << setw(20) << "Type" << setw(14) << "Price" << setw(20) << "Status" << setw(20) << "Last Update" << endl;

        cout << setfill('-') << setw(115) << "" << endl;
        cout << setfill(' ');

        MYSQL_RES *res = mysql_store_result(Connection::conn);
        MYSQL_ROW row;

        while ((row = mysql_fetch_row(res)) != nullptr)
        {
            cout << left << setw(5) << rowNum
                << setw(20) << row[1]
                << setw(20) << row[2]
                << setw(20) << row[8]
                << setw(4) << "MYR" << setw(10) << fixed << setprecision(2) << atof(row[3])
                << setw(20) << row[4]
                << setw(20) << row[5]
                << endl;

            rowIdMap[rowNum] = row[0];
            
            rowNum++;
        }
        mysql_free_result(res);
        
        cout << setfill('-') << setw(115) << "" << endl;
        cout << setfill(' ');
        
        do
        {

            cout << "Enter the row No. of the equipment: ";
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
                
                User::display_header("Delete an Equipment Screen");
                
                equipment_id = rowIdMap[rowId];

                query = "SELECT * FROM equipment WHERE equipment_id = '" + equipment_id + "'";
                qstate = mysql_query(Connection::conn, query.c_str());

                if (!qstate)
                {

                    cout << left << setw(20) << "Model" << setw(20) << "Color" << setw(20) << "Type" << setw(14) << "Price" << setw(20) << "Status" << setw(20) << "Last Update" << endl;

                    cout << setfill('-') << setw(115) << "" << endl;
                    cout << setfill(' ');

                    MYSQL_RES *res = mysql_store_result(Connection::conn);
                    MYSQL_ROW row;

                    while ((row = mysql_fetch_row(res)) != nullptr)
                    {
                        cout << left << setw(20) << row[1]
                            << setw(20) << row[2]
                            << setw(20) << row[8]
                            << setw(4) << "MYR" << setw(10) << fixed << setprecision(2) << atof(row[3])
                            << setw(20) << row[4]
                            << setw(20) << row[5]
                            << endl;
                    }
                    mysql_free_result(res);

                    cout << setfill('-') << setw(115) << "" << endl;
                    cout << setfill(' ');

                    cout << "Are you sure do you want to delete this equipment? (Y/N)?" << endl;

                    do
                    {
                        cout << "--------------------------------------------" << endl;
                        cout << "Enter a your choice: ";
                        char choice;
                        cin >> ws >> choice;

                        if (choice == 'Y' || choice == 'y')
                        {
                            // Check if the equipment is being used in any rentals
                            //query = "SELECT COUNT(equipment_id) FROM rental_details WHERE equipment_id IN (SELECT equipment_id FROM equipment WHERE equipment_type_id = '" + equipment_type_id + "')";
                            //qstate = mysql_query(Connection::conn, query.c_str());

                            //res = mysql_store_result(Connection::conn);

                            //if(mysql_num_rows(res) > 0)
                            //{    
                            //    // mysql_free_result(res);

                            //    cout << "--------------------------------------------" << endl;
                            //    cout << "Cannot delete this equipment as it is being used in some rentals!" << endl;
                            //    
                            //    this_thread::sleep_for(chrono::seconds(2));
                            //    equipment(id, type);
                            //}
                            //else
                            //{
                            //    // mysql_free_result(res);

                            //    query = "DELETE FROM equipment WHERE equipment_id = '" + equipment_id + "'";
                            //    qstate = mysql_query(Connection::conn, query.c_str());

                            //    if (!qstate)
                            //    {
                            //        cout << "--------------------------------------------" << endl;
                            //        cout << "This equipment has been deleted successfully!" << endl;
                            //        cout << "Please wait a moment..." << endl;

                            //        this_thread::sleep_for(chrono::seconds(2));
                            //        equipment(id, type);
                            //    }
                            //    else
                            //    {
                            //        cout << "--------------------------------------------" << endl;
                            //        cout << "Failed to execute the query! " << mysql_errno(Connection::conn) << endl;
                            //        cout << "Please wait a moment..." << endl;

                            //        this_thread::sleep_for(chrono::seconds(2));
                            //        equipment(id, type);
                            //    }
                            //}
                            //mysql_free_result(res);
                            
                            break;
                        }
                        else if (choice == 'N' || choice == 'n')
                        {
                            equipment(id, type);
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
                    equipment(id, type);
                }
                break;
            }
            else
            {
                cout << "--------------------------------------------" << endl;
                cout << "Please enter a valid row No.!" << endl;
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
        equipment(id, type);
    }
}

// Search Equipment
void Equipment::find_equipment(string id, string type)
{

    User::display_header("Search for Equipment Screen");

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
                User::display_header("Search for an Equipment Screen");

                do
                {
                    cout << "--------------------------------------------" << endl;
                    cout << "Enter the equipment model: ";
                    getline(cin >> ws, model);

                    if (model.empty())
                    {
                        cout << "--------------------------------------------" << endl;
                        cout << "Equipment model cannot be empty! Please try again." << endl;
                        continue;
                    }
                    else 
                    {

                        query = "SELECT * FROM equipment JOIN equipment_type ON equipment.equipment_type_id = equipment_type.equipment_type_id WHERE model LIKE '%" + model + "%'";
                        qstate = mysql_query(Connection::conn, query.c_str());

                        if (!qstate)
                        {

                            MYSQL_RES *res = mysql_store_result(Connection::conn);
                            MYSQL_ROW row;

                            if (mysql_num_rows(res) > 0)
                            {
                                int rowNum = 1;

                                cout << left << setw(5) << "No." << setw(20) << "Model" << setw(20) << "Color" << setw(20) << "Type" << setw(14) << "Price" << setw(20) << "Status" << setw(20) << "Last Update" << endl;

                                cout << setfill('-') << setw(115) << "" << endl;
                                cout << setfill(' ');

                                while ((row = mysql_fetch_row(res)) != nullptr)
                                {
                                    cout << left << setw(5) << rowNum++ 
                                        << setw(20) << row[1]
                                        << setw(20) << row[2]
                                        << setw(20) << row[8]
                                        << setw(4) << "MYR" << setw(10) << fixed << setprecision(2) << atof(row[3])
                                        << setw(20) << row[4]
                                        << setw(20) << row[5]
                                        << endl;
                                }                                
                                mysql_free_result(res);

                                cout << setfill('-') << setw(115) << "" << endl;
                                cout << setfill(' ');
                            }
                            else
                            {
                                mysql_free_result(res);
                                cout << "--------------------------------------------" << endl;
                                cout << "This equipment model is not found!" << endl;
                            }

                        }
                        else
                        {
                            cout << "--------------------------------------------" << endl;
                            cout << "Failed to execute the query! " << mysql_errno(Connection::conn) << endl;
                            cout << "Please wait a moment..." << endl;

                            this_thread::sleep_for(chrono::seconds(2));
                            equipment(id, type);
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
                                equipment(id, type);
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
            equipment(id, type);
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
