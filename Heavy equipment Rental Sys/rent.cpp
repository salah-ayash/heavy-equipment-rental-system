#include "connection.h"
#include "rent.h"
#include "equipment.h"
#include "user.h"
#include "customer.h"
#include "staff.h"

#include <sstream>
// Check the date format
bool Rent::check_date(string date) 
{
    const regex pattern("^\\d{4}-\\d{2}-\\d{2}$");
    return regex_match(date, pattern);
}

//bool Rent::validate_date(string date) 
//{
//    struct tm tm;
//    strptime(date.c_str(), "%Y-%m-%d", &tm);
//
//    time_t t = mktime(&tm);
//    if (t == -1)
//    {
//        return false;
//    }
//
//    return true;
//}

// windows
bool Rent::validate_date(string date)
{
    struct tm tm;
    sscanf_s(date.c_str(), "%d-%d-%d", &tm.tm_year, &tm.tm_mon, &tm.tm_mday);
    tm.tm_year -= 1900;
    tm.tm_mon -= 1;
    tm.tm_hour = tm.tm_min = tm.tm_sec = 0;
    time_t t = mktime(&tm);
    if (t == -1)
    {
        return false;
    }
    return true;
}

//bool Rent::compare_dates(string rental_date, string return_date)
//{
//    // Convert the dates to time_t values
//    struct tm tm_rental_date, tm_end_date;
//    strptime(rental_date.c_str(), "%Y-%m-%d", &tm_rental_date);
//    strptime(return_date.c_str(), "%Y-%m-%d", &tm_end_date);
//    time_t rental_date_time = mktime(&tm_rental_date);
//    time_t end_date_time = mktime(&tm_end_date);
//
//    // Compare the two dates
//    return (rental_date_time > end_date_time);
//}


// windows
bool Rent::compare_dates(string rental_date, string return_date)
{
    struct tm tm_rental_date, tm_end_date;
    sscanf_s(rental_date.c_str(), "%d-%d-%d", &tm_rental_date.tm_year, &tm_rental_date.tm_mon, &tm_rental_date.tm_mday);
    sscanf_s(return_date.c_str(), "%d-%d-%d", &tm_end_date.tm_year, &tm_end_date.tm_mon, &tm_end_date.tm_mday);
    tm_rental_date.tm_year -= 1900;
    tm_rental_date.tm_mon -= 1;
    tm_rental_date.tm_hour = tm_rental_date.tm_min = tm_rental_date.tm_sec = 0;
    tm_end_date.tm_year -= 1900;
    tm_end_date.tm_mon -= 1;
    tm_end_date.tm_hour = tm_end_date.tm_min = tm_end_date.tm_sec = 0;
    time_t rental_date_time = mktime(&tm_rental_date);
    time_t end_date_time = mktime(&tm_end_date);

    return (rental_date_time < end_date_time);
}

//int Rent::calc_duration(string rental_date, string return_date)
//{
//    // Convert the date strings to struct tm objects
//    struct tm rental_date_tm, return_date_tm;
//    strptime(rental_date.c_str(), "%Y-%m-%d", &rental_date_tm);
//    strptime(return_date.c_str(), "%Y-%m-%d", &return_date_tm);
//
//    // calculate the difference in days
//    int days = (return_date_tm.tm_year - rental_date_tm.tm_year) * 365 +
//               (return_date_tm.tm_yday - rental_date_tm.tm_yday);
//
//    return days;
//}

// windows
int Rent::calc_duration(string rental_date, string return_date)
{
    struct tm rental_date_tm, return_date_tm;
    sscanf_s(rental_date.c_str(), "%d-%d-%d", &rental_date_tm.tm_year, &rental_date_tm.tm_mon, &rental_date_tm.tm_mday);
    sscanf_s(return_date.c_str(), "%d-%d-%d", &return_date_tm.tm_year, &return_date_tm.tm_mon, &return_date_tm.tm_mday);
    rental_date_tm.tm_year -= 1900;
    rental_date_tm.tm_mon -= 1;
    rental_date_tm.tm_hour = rental_date_tm.tm_min = rental_date_tm.tm_sec = 0;
    return_date_tm.tm_year -= 1900;
    return_date_tm.tm_mon -= 1;
    return_date_tm.tm_hour = return_date_tm.tm_min = return_date_tm.tm_sec = 0;
    time_t rental_date_time = mktime(&rental_date_tm);
    time_t return_date_time = mktime(&return_date_tm);

    double seconds = difftime(return_date_time, rental_date_time);
    int days = (int)(seconds / (24 * 60 * 60));
    return days;
}

string Rent::displayDuration(int duration) {
    return duration > 1 ? to_string(duration) + " Days" : to_string(duration) + " Day";
}

bool Rent::validate_transaction_id(string transaction_id) 
{
    const regex pattern("^[a-zA-Z0-9]{6,}$");
    return regex_match(transaction_id, pattern);
}

void Rent::rent(string id, string type)
{

    User::display_header("Rental Screen");

    Customer customer;
    Staff staff;

    query = "SELECT * FROM equipment JOIN equipment_type ON equipment.equipment_type_id = equipment_type.equipment_type_id WHERE status = 'Available' ORDER BY equipment.equipment_id DESC";
    qstate = mysql_query(Connection::conn, query.c_str());

    if (!qstate)
    {

        int rowNum = 1;

        cout << left << setw(5) << "No." << setw(10) << "Model" << setw(10) << "Color" << setw(20) << "Type" << setw(14) << "Price" << setw(20) << "Status" << setw(20) << "Last Update" << endl;

        cout << setfill('-') << setw(115) << "" << endl;
        cout << setfill(' ');

        MYSQL_RES *res = mysql_store_result(Connection::conn);
        MYSQL_ROW row;

        while ((row = mysql_fetch_row(res)) != nullptr)
        {

            cout << left << setw(5) << rowNum++
                << setw(10) << row[1]
                << setw(10) << row[2]
                << setw(20) << row[8]
                << setw(4) <<  "MYR" << setw(10) << fixed << setprecision(2) << atof(row[3])
                << setw(20) << row[4]
                << setw(20) << row[5]
                << endl;
        }
        mysql_free_result(res);
        
        cout << setfill('-') << setw(115) << "" << endl;
        cout << setfill(' ');

        cout << "1. Continue renting" << endl;
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
                map<int, rental_info> rentalMap;
                int rentNum = 1;

                bool flag = true;
                
                do
                {
                
                    User::display_header("Rental Screen");

                    map<int, string> rowIdMap;
                    int rowNum = 1;

                    query = "SELECT * FROM equipment JOIN equipment_type ON equipment.equipment_type_id = equipment_type.equipment_type_id WHERE status = 'Available' ORDER BY equipment.equipment_id DESC";
                    qstate = mysql_query(Connection::conn, query.c_str());

                    if (!qstate)
                    {

                        cout << left << setw(5) << "No." << setw(10) << "Model" << setw(10) << "Color" << setw(20) << "Type" << setw(14) << "Price" << setw(20) << "Status" << setw(20) << "Last Update" << endl;

                        cout << setfill('-') << setw(115) << "" << endl;
                        cout << setfill(' ');

                        MYSQL_RES *res = mysql_store_result(Connection::conn);
                        MYSQL_ROW row;

                        while ((row = mysql_fetch_row(res)) != nullptr) 
                        {

                            cout << left << setw(5) << rowNum
                                << setw(10) << row[1]
                                << setw(10) << row[2]
                                << setw(20) << row[8]
                                << setw(4) <<  "MYR" << setw(10) << fixed << setprecision(2) << atof(row[3])
                                << setw(20) << row[4]
                                << setw(20) << row[5]
                                << endl;

                            rowIdMap[rowNum] = string(row[0]);

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

                                User::display_header("Rental Screen");

                                rental_info new_rental;
                                new_rental.equipment_id = rowIdMap[rowId];
                                
                                query = "SELECT * FROM equipment JOIN equipment_type ON equipment.equipment_type_id = equipment_type.equipment_type_id WHERE status = 'Available' AND equipment_id = '" + new_rental.equipment_id + "'";
                                qstate = mysql_query(Connection::conn, query.c_str());

                                if (!qstate)
                                {

                                    cout << left << setw(10) << "Model" << setw(10) << "Color" << setw(20) << "Type" << setw(14) << "Price" << setw(20) << "Status" << setw(20) << "Last Update" << endl;

                                    cout << setfill('-') << setw(115) << "" << endl;
                                    cout << setfill(' ');

                                    MYSQL_RES *res = mysql_store_result(Connection::conn);
                                    MYSQL_ROW row;

                                    while ((row = mysql_fetch_row(res)) != nullptr)
                                    {

                                        cout << left << setw(20) << row[1]
                                            << setw(10) << row[2]
                                            << setw(10) << row[8]
                                            << setw(4) <<  "MYR" << setw(10) << fixed << setprecision(2) << atof(row[3])
                                            << setw(20) << row[4]
                                            << setw(20) << row[5]
                                            << endl;

                                        new_rental.model = string(row[1]);
                                        new_rental.color = string(row[2]);
                                        new_rental.price = atof(row[3]);
                                    }
                                    mysql_free_result(res);
                                        
                                    cout << setfill('-') << setw(115) << "" << endl;
                                    cout << setfill(' ');

                                    do {

                                        cout << "Enter the rental date: ";
                                        getline(cin >> ws, new_rental.rental_date);

                                        if (new_rental.rental_date.empty()) {
                                            cout << "--------------------------------------------" << endl;
                                            cout << "Rental date cannot be empty. Please try again." << endl;
                                            continue;
                                        }
                                        else if (!check_date(new_rental.rental_date))
                                        {
                                            cout << "--------------------------------------------" << endl;
                                            cout << "Invalid date format. The date should be in the YYYY-MM-DD format." << endl;
                                            continue;
                                        }
                                        else if (!validate_date(new_rental.rental_date))
                                        {
                                            cout << "--------------------------------------------" << endl;
                                            cout << "Invalid date! Please enter a valid date" << endl;
                                            continue;
                                        }
                                        else 
                                        {
                                            break;
                                        }

                                    } while (true);

                                    do {
                                        cout << "--------------------------------------------" << endl;
                                        cout << "Enter the return date: ";
                                        getline(cin >> ws, new_rental.return_date);

                                        if (new_rental.return_date.empty()) {
                                            cout << "--------------------------------------------" << endl;
                                            cout << "Return end date cannot be empty. Please try again." << endl;
                                            continue;
                                        }
                                        else if (!check_date(new_rental.return_date))
                                        {
                                            cout << "--------------------------------------------" << endl;
                                            cout << "Invalid date format. The date should be in the YYYY-MM-DD format." << endl;
                                            continue;
                                        }
                                        else if (!validate_date(new_rental.return_date))
                                        {
                                            cout << "--------------------------------------------" << endl;
                                            cout << "Invalid date! Please enter a valid date" << endl;
                                            continue;
                                        }
                                        else if (!compare_dates(new_rental.rental_date, new_rental.return_date))
                                        {
                                            cout << "--------------------------------------------" << endl;
                                            cout << "Return date must be later than rental date" << endl;
                                            continue;
                                        }
                                        else 
                                        {
                                            break;
                                        }

                                    } while (true);

                                    new_rental.duration = calc_duration(new_rental.rental_date, new_rental.return_date);
                                    new_rental.total = new_rental.price * new_rental.duration;
                                    
                                    rentalMap[rentNum] = new_rental;
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

                    cout << "Do you want to rent more? (Y/N)?" << endl;
                        
                    do
                    {
                        cout << "--------------------------------------------" << endl;
                        cout << "Enter a your choice: ";
                        char choice;
                        cin >> ws >> choice;

                        if (choice == 'Y' || choice == 'y')
                        {
                            rentNum++;
                            break;
                        }
                        else if (choice == 'N' || choice == 'n')
                        {
                            flag = false;
                            break;
                        }
                        else
                        {
                            cout << "--------------------------------------------" << endl;
                            cout << "You entered an invalid choice!" << endl;
                            continue;
                        }

                    } while (true);

                    if (!flag) {
                        break;
                    }

                } while (flag);

                cout << setfill('-') << setw(115) << "" << endl;
                cout << setfill(' ');
                cout << "Here is your receipt" << endl;
                cout << setfill('-') << setw(115) << "" << endl;
                cout << setfill(' ');

                cout << left << setw(5) << "No." << setw(10) << "Model" << setw(10) << "Color" << setw(14) << "Price" << setw(20) << "Rental Date" << setw(20) << "Return Date" << setw(15) << "Duration" << setw(14) << "Total" << endl;

                double amount = 0;

                for (map<int, rental_info>::iterator it = rentalMap.begin(); it != rentalMap.end(); it++) {

                    cout << left << setw(5) << it->first
                        << setw(10) << it->second.model
                        << setw(10) << it->second.color
                        << setw(4) <<  "MYR" << setw(10) << fixed << setprecision(2) << it->second.price
                        << setw(20) << it->second.rental_date
                        << setw(20) << it->second.return_date
                        << setw(15) << displayDuration(it->second.duration)
                        << setw(4) << "MYR" << setw(15) << fixed << setprecision(2) << it->second.total
                        << endl;

                    amount += it->second.total;
                }

                cout << setfill('-') << setw(115) << "" << endl;
                cout << setfill(' ');
                cout << "Total Amount: MYR " << fixed << setprecision(2) << amount << endl;
                cout << setfill('-') << setw(115) << "" << endl;
                cout << setfill(' ');


                cout << "--------------------------------------------" << endl;
                cout << "1. Continue to payment" << endl;
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
                        cout << "--------------------------------------------" << endl;
                        cout << "Please do a payment to the following bank account." << endl;
                        cout << "--------------------------------------------" << endl;
                        cout << "Bank Name: RHB" << endl;
                        cout << "Bank Account No.: 15401300148598" << endl;
                        cout << "Account Holder Name: SALAH" << endl;

                        cout << "--------------------------------------------" << endl;
                        cout << "Did you make the payment? (Y/N)?" << endl;

                        do
                        {
                            cout << "--------------------------------------------" << endl;
                            cout << "Enter a your choice: ";
                            char choice;
                            cin >> ws >> choice;

                            if (choice == 'Y' || choice == 'y')
                            {

                                while (true) {

                                    cout << "--------------------------------------------" << endl;
                                    cout << "Please enter your bank name: ";
                                    getline(cin >> ws, bank);

                                    if (bank.empty()) {
                                        cout << "--------------------------------------------" << endl;
                                        cout << "Bank name cannot be empty. Please try again." << endl;
                                        continue;
                                    }
                                    else 
                                    {
                                        break;
                                    }
                                }

                                while (true) {

                                    cout << "--------------------------------------------" << endl;
                                    cout << "Please enter the payment reference ID: ";
                                    getline(cin >> ws, transaction_id);

                                    if (transaction_id.empty()) {
                                        cout << "--------------------------------------------" << endl;
                                        cout << "Reference ID cannot be empty. Please try again." << endl;
                                        continue;
                                    }
                                    // else if (!validate_transaction_id(transaction_id)) 
                                    // {
                                    //     cout << "Invalid reference ID!" << endl;
                                    //     continue;
                                    // }
                                    else 
                                    {
                                        break;
                                    }
                                }

                                query = "SELECT customer_id FROM customer JOIN user ON customer.user_id = user.user_id WHERE user.user_id = '" + id + "'";
                                qstate = mysql_query(Connection::conn, query.c_str());

                                if (!qstate)
                                {
                                    MYSQL_RES *res = mysql_store_result(Connection::conn);
                                    MYSQL_ROW row;

                                    while ((row = mysql_fetch_row(res)) != nullptr)
                                    {

                                        string customer_id = row[0];

                                        mysql_free_result(res);

                                        for (map<int, rental_info>::iterator it = rentalMap.begin(); it != rentalMap.end(); it++) {

                                            query = "INSERT INTO rental (customer_id, rental_date, return_date, duration, total, bank, transaction_id) VALUES ('" + customer_id + "','" + it->second.rental_date + "','" + it->second.return_date + "','" + to_string(it->second.duration) +  "','" + to_string(it->second.total) +  "','" + bank + "','" + transaction_id + "')";
                                            qstate = mysql_query(Connection::conn, query.c_str());

                                            // Retrieve the last inserted ID
                                            unsigned long rental_id = mysql_insert_id(Connection::conn);

                                            query = "INSERT INTO rental_details (equipment_id, rental_id) VALUES ('" + it->second.equipment_id + "','" + to_string(rental_id) + "')";
                                            qstate = mysql_query(Connection::conn, query.c_str());

                                            query = "UPDATE equipment SET status = 'Reserved' WHERE equipment_id = '" + it->second.equipment_id + "'";
                                            qstate = mysql_query(Connection::conn, query.c_str());
                    
                                        }


                                        if (!qstate)
                                        {
                                            cout << "--------------------------------------------" << endl;
                                            cout << "This equipment has been rented successfully!" << endl;
                                            cout << "We will check your payment, and update your rental status soon!" << endl;
                                            cout << "Thank you..." << endl;

                                            this_thread::sleep_for(chrono::seconds(2));
                                            
                                            if (type == "Customer") 
                                            {
                                                Customer customer;
                                                customer.customer(id, type);
                                            }
                                            else if (type == "Staff") 
                                            {
                                                Staff staff;
                                                staff.home(id, type);
                                            }
                                        }
                                        else
                                        {
                                            cout << "--------------------------------------------" << endl;
                                            cout << "Something went wrong! " << mysql_errno(Connection::conn) << endl;
                                            cout << "Please wait a moment..." << endl;

                                            this_thread::sleep_for(chrono::seconds(2));
                                            if (type == "Customer") 
                                            {
                                                Customer customer;
                                                customer.customer(id, type);
                                            }
                                            else if (type == "Staff") 
                                            {
                                                Staff staff;
                                                staff.home(id, type);
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
                                        customer.customer(id, type);
                                    }
                                    else if (type == "Staff") 
                                    {
                                        staff.home(id, type);
                                    }
                                }

                                break;

                            }
                            else if (choice == 'N' || choice == 'n')
                            {
                                customer.customer(id, type);
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
                    else if (menu == 2)
                    {
                        customer.customer(id, type);
                        break;
                    }
                    else
                    {
                        cout << "--------------------------------------------" << endl;
                        cout << "You entered an invalid menu No.!" << endl;
                        continue;
                    }

                } while(true); 

            }
            else if (menu == 2)
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

void Rent::history(string id, string type)
{
    User::display_header("Rental History Screen");

    Customer customer;
    Staff staff;

    if (type == "Customer") 
    {
        query = "SELECT rental.rental_id, model, color, type_name, price, rental_date, return_date, duration, total, rental.status, application_date FROM rental JOIN rental_details ON rental.rental_id = rental_details.rental_id JOIN customer ON rental.customer_id = customer.customer_id JOIN user ON customer.user_id = user.user_id JOIN equipment ON rental_details.equipment_id = equipment.equipment_id JOIN equipment_type ON equipment.equipment_type_id = equipment_type.equipment_type_id WHERE user.user_id = '" + id + "' ORDER BY rental.rental_id DESC";
    }
    else if (type == "Staff")
    {
        query = "SELECT rental.rental_id, CONCAT(fname, ' ', lname), email, model, color, type_name, price, rental_date, return_date, duration, total, rental.status, application_date FROM rental JOIN rental_details ON rental.rental_id = rental_details.rental_id JOIN customer ON rental.customer_id = customer.customer_id JOIN user ON customer.user_id = user.user_id JOIN equipment ON rental_details.equipment_id = equipment.equipment_id JOIN equipment_type ON equipment.equipment_type_id = equipment_type.equipment_type_id ORDER BY rental.rental_id DESC";
    }
    qstate = mysql_query(Connection::conn, query.c_str());

    if (!qstate)
    {
        int rowNum = 1;
        map<int, string> rowIdMap;

        if (type == "Customer") 
        {    
            cout << left << setw(5) << "No." << setw(10) << "Model" << setw(10) << "Color" << setw(15) << "Type" << setw(14) << "Price" << setw(15) << "Rental Date" << setw(15) << "Return Date" << setw(15) << "Duration" << setw(14) << "Total" << setw(20) << "Status" << setw(15) << "Application Date" << endl;
        }
        else if (type == "Staff")
        {
            cout << left << setw(5) << "No." << setw(15) << "Customer" << setw(25) << "Email" << setw(10) << "Model" << setw(10) << "Color" << setw(15) << "Type" << setw(14) << "Price" << setw(15) << "Rental Date" << setw(15) << "Return Date" << setw(15) << "Duration" << setw(14) << "Total" << setw(20) << "Status" << setw(15) << "Application Date" << endl;
        }

        cout << setfill('-') << setw(150) << "" << endl;
        cout << setfill(' ');

        MYSQL_RES *res = mysql_store_result(Connection::conn);
        MYSQL_ROW row;

        if (type == "Customer")
        {   
            while ((row = mysql_fetch_row(res)) != nullptr)
            {
                cout << left << setw(5) << rowNum++
                    << setw(10) << row[1]
                    << setw(10) << row[2]
                    << setw(15) << row[3]
                    << setw(4) <<  "MYR" << setw(10) << fixed << setprecision(2) << atof(row[4])
                    << setw(15) << row[5]
                    << setw(15) << row[6]
                    << setw(15) << displayDuration(atoi(row[7]))
                    << setw(4) <<  "MYR" << setw(10) << fixed << setprecision(2) << atof(row[8])
                    << setw(20) << row[9]
                    << setw(15) << row[10]
                    << endl;
            }
        }
        else if (type == "Staff")
        {
            while ((row = mysql_fetch_row(res)) != nullptr)
            {

                cout << left << setw(5) << rowNum++
                    << setw(15) << row[1]
                    << setw(25) << row[2]
                    << setw(10) << row[3]
                    << setw(10) << row[4]
                    << setw(15) << row[5]
                    << setw(4) <<  "MYR" << setw(10) << fixed << setprecision(2) << atof(row[6])
                    << setw(15) << row[7]
                    << setw(15) << row[8]
                    << setw(15) << displayDuration(atoi(row[9]))
                    << setw(4) <<  "MYR" << setw(10) << fixed << setprecision(2) << atof(row[10])
                    << setw(20) << row[11]
                    << setw(15) << row[12]
                    << endl;
            }
        }
        mysql_free_result(res);
            
        cout << setfill('-') << setw(150) << "" << endl;
        cout << setfill(' ');

        if (type == "Customer")
        {
            cout << "1. Cancel a rental" << endl;
            cout << "2. Go back" << endl;
        }
        else if (type == "Staff")
        {
            cout << "1. Approve a rental" << endl;
            cout << "2. Cancel a rental" << endl;
            cout << "3. Go back" << endl;
        }        

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
                if (type == "Customer")
                {
                    cancel(id, type);
                }
                else if (type == "Staff")
                {
                    approve(id, type);
                } 
                break;
            }
            else if (menu == 2)
            {
                if (type == "Customer")
                {
                    customer.customer(id, type);
                }
                else if (type == "Staff")
                {
                    cancel(id, type);
                } 
                break;
            }
            else if (menu == 3 && type == "Staff")
            {
                staff.home(id, type);
                break;
            }
            else
            {
                cout << "--------------------------------------------" << endl;
                cout << "You entered an invalid menu No.!" << endl;
                continue;
            }

        } while(true);
    
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

void Rent::cancel(string id, string type)
{
    
    User::display_header("Cancel Rentals Screen");

    Customer customer;
    Staff staff;

    cout << "1. Continue canceling" << endl;
    cout << "2. Go back" << endl << endl;

    do
    {
    
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
            User::display_header("Cancel Rentals Screen");
            
            if (type == "Customer") 
            {
                query = "SELECT rental.rental_id, model, color, type_name, price, rental_date, return_date, duration, total, rental.status, application_date FROM rental JOIN rental_details ON rental.rental_id = rental_details.rental_id JOIN customer ON rental.customer_id = customer.customer_id JOIN user ON customer.user_id = user.user_id JOIN equipment ON rental_details.equipment_id = equipment.equipment_id JOIN equipment_type ON equipment.equipment_type_id = equipment_type.equipment_type_id WHERE user.user_id = '" + id + "' AND rental.status != 'Canceled' ORDER BY rental.rental_id DESC";
            }
            else if (type == "Staff")
            {
                query = "SELECT rental.rental_id, CONCAT(fname, ' ', lname), email, model, color, type_name, price, rental_date, return_date, duration, total, rental.status, application_date FROM rental JOIN rental_details ON rental.rental_id = rental_details.rental_id JOIN customer ON rental.customer_id = customer.customer_id JOIN user ON customer.user_id = user.user_id JOIN equipment ON rental_details.equipment_id = equipment.equipment_id JOIN equipment_type ON equipment.equipment_type_id = equipment_type.equipment_type_id WHERE rental.status != 'Canceled' ORDER BY rental.rental_id DESC";
            }
            qstate = mysql_query(Connection::conn, query.c_str());

            if (!qstate)
            {
                map<int, string> rowIdMap;
                int rowNum = 1;

                if (type == "Customer") 
                {    
                    cout << left << setw(5) << "No." << setw(10) << "Model" << setw(10) << "Color" << setw(15) << "Type" << setw(14) << "Price" << setw(15) << "Rental Date" << setw(15) << "Return Date" << setw(15) << "Duration" << setw(14) << "Total" << setw(20) << "Status" << setw(15) << "Application Date" << endl;
                }
                else if (type == "Staff")
                {
                    cout << left << setw(5) << "No." << setw(15) << "Customer" << setw(25) << "Email" << setw(10) << "Model" << setw(10) << "Color" << setw(15) << "Type" << setw(14) << "Price" << setw(15) << "Rental Date" << setw(15) << "Return Date" << setw(15) << "Duration" << setw(14) << "Total" << setw(20) << "Status" << setw(15) << "Application Date" << endl;
                }

                cout << setfill('-') << setw(150) << "" << endl;
                cout << setfill(' ');

                MYSQL_RES *res = mysql_store_result(Connection::conn);
                MYSQL_ROW row;

                if (type == "Customer")
                {   
                    while ((row = mysql_fetch_row(res)) != nullptr)
                    {
                        cout << left << setw(5) << rowNum
                            << setw(10) << row[1]
                            << setw(10) << row[2]
                            << setw(15) << row[3]
                            << setw(4) <<  "MYR" << setw(10) << fixed << setprecision(2) << atof(row[4])
                            << setw(15) << row[5]
                            << setw(15) << row[6]
                            << setw(15) << displayDuration(atoi(row[7]))
                            << setw(4) <<  "MYR" << setw(10) << fixed << setprecision(2) << atof(row[8])
                            << setw(20) << row[9]
                            << setw(15) << row[10]
                            << endl;

                        rowIdMap[rowNum] = row[0];

                        rowNum++;
                    }
                }
                else if (type == "Staff")
                {
                    while ((row = mysql_fetch_row(res)) != nullptr)
                    {

                        cout << left << setw(5) << rowNum
                            << setw(15) << row[1]
                            << setw(25) << row[2]
                            << setw(10) << row[3]
                            << setw(10) << row[4]
                            << setw(15) << row[5]
                            << setw(4) <<  "MYR" << setw(10) << fixed << setprecision(2) << atof(row[6])
                            << setw(15) << row[7]
                            << setw(15) << row[8]
                            << setw(15) << displayDuration(atoi(row[9]))
                            << setw(4) <<  "MYR" << setw(10) << fixed << setprecision(2) << atof(row[10])
                            << setw(20) << row[11]
                            << setw(15) << row[12]
                            << endl;

                        rowIdMap[rowNum] = row[0];

                        rowNum++;
                    }
                }
                mysql_free_result(res);
                    
                cout << setfill('-') << setw(150) << "" << endl;
                cout << setfill(' ');

                do
                {
                    cout << "Enter the row No. of the rental: ";
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

                        User::display_header("Cancel a Rental Screen");

                        rental_id = rowIdMap[rowId];

                        if (type == "Customer") 
                        {
                            query = "SELECT equipment.equipment_id, model, color, type_name, price, rental_date, return_date, duration, total, rental.status, application_date FROM rental JOIN rental_details ON rental.rental_id = rental_details.rental_id JOIN customer ON rental.customer_id = customer.customer_id JOIN user ON customer.user_id = user.user_id JOIN equipment ON rental_details.equipment_id = equipment.equipment_id JOIN equipment_type ON equipment.equipment_type_id = equipment_type.equipment_type_id WHERE user.user_id = '" + id + "' AND rental.rental_id = '" + rental_id + "' AND rental.status != 'Canceled'";
                        }
                        else if (type == "Staff")
                        {
                            query = "SELECT equipment.equipment_id, CONCAT(fname, ' ', lname), email, model, color, type_name, price, rental_date, return_date, duration, total, rental.status, application_date FROM rental JOIN rental_details ON rental.rental_id = rental_details.rental_id JOIN customer ON rental.customer_id = customer.customer_id JOIN user ON customer.user_id = user.user_id JOIN equipment ON rental_details.equipment_id = equipment.equipment_id JOIN equipment_type ON equipment.equipment_type_id = equipment_type.equipment_type_id  WHERE rental.rental_id = '" + rental_id + "' AND rental.status != 'Canceled'";
                        }
                        qstate = mysql_query(Connection::conn, query.c_str());

                        if (!qstate)
                        {
                            map<int, string> rowIdMap;
                            int rowNum = 1;

                            Equipment equipment;

                            if (type == "Customer") 
                            {    
                                cout << left << setw(5) << "No." << setw(10) << "Model" << setw(10) << "Color" << setw(15) << "Type" << setw(14) << "Price" << setw(15) << "Rental Date" << setw(15) << "Return Date" << setw(15) << "Duration" << setw(14) << "Total" << setw(20) << "Status" << setw(15) << "Application Date" << endl;
                            }
                            else if (type == "Staff")
                            {
                                cout << left << setw(5) << "No." << setw(15) << "Customer" << setw(25) << "Email" << setw(10) << "Model" << setw(10) << "Color" << setw(15) << "Type" << setw(14) << "Price" << setw(15) << "Rental Date" << setw(15) << "Return Date" << setw(15) << "Duration" << setw(14) << "Total" << setw(20) << "Status" << setw(15) << "Application Date" << endl;
                            }

                            cout << setfill('-') << setw(150) << "" << endl;
                            cout << setfill(' ');

                            MYSQL_RES *res = mysql_store_result(Connection::conn);
                            MYSQL_ROW row;

                            if (type == "Customer")
                            {    
                                while ((row = mysql_fetch_row(res)) != nullptr)
                                {
                                    cout << left << setw(5) << rowNum++
                                        << setw(10) << row[1]
                                        << setw(10) << row[2]
                                        << setw(15) << row[3]
                                        << setw(4) <<  "MYR" << setw(10) << fixed << setprecision(2) << atof(row[4])
                                        << setw(15) << row[5]
                                        << setw(15) << row[6]
                                        << setw(15) << displayDuration(atoi(row[7]))
                                        << setw(4) <<  "MYR" << setw(10) << fixed << setprecision(2) << atof(row[8])
                                        << setw(20) << row[9]
                                        << setw(15) << row[10]
                                        << endl;
                                        
                                    equipment.equipment_id = row[0];
                                }
                            }
                            else if (type == "Staff")
                            {
                                while ((row = mysql_fetch_row(res)) != nullptr)
                                {

                                    cout << left << setw(5) << rowNum++
                                        << setw(15) << row[1]
                                        << setw(25) << row[2]
                                        << setw(10) << row[3]
                                        << setw(10) << row[4]
                                        << setw(15) << row[5]
                                        << setw(4) <<  "MYR" << setw(10) << fixed << setprecision(2) << atof(row[6])
                                        << setw(15) << row[7]
                                        << setw(15) << row[8]
                                        << setw(15) << displayDuration(atoi(row[9]))
                                        << setw(4) <<  "MYR" << setw(10) << fixed << setprecision(2) << atof(row[10])
                                        << setw(20) << row[11]
                                        << setw(15) << row[12]
                                        << endl;
                                    
                                    equipment.equipment_id = row[0];
                                }
                            }
                            mysql_free_result(res);
                                
                            cout << setfill('-') << setw(150) << "" << endl;
                            cout << setfill(' ');                            

                            cout << "Are you sure do you want to cancel this rental? (Y/N)?" << endl;

                            do
                            {
                                cout << "--------------------------------------------" << endl;
                                cout << "Enter a your choice: ";
                                char choice;
                                cin >> ws >> choice;

                                if (choice == 'Y' || choice == 'y')
                                {

                                    query = "UPDATE rental SET status = 'Canceled'  WHERE rental_id = '" + rental_id + "'";
                                    qstate = mysql_query(Connection::conn, query.c_str());
                                    
                                    query = "UPDATE equipment SET status = 'Available' WHERE equipment_id = '" + equipment.equipment_id + "'";
                                    qstate = mysql_query(Connection::conn, query.c_str());

                                    if (!qstate)
                                    {
                                        cout << "--------------------------------------------" << endl;
                                        cout << "This Rental has been canceled successfully!" << endl;
                                        cout << "Please wait a moment..." << endl;

                                        this_thread::sleep_for(chrono::seconds(2));
                                        history(id, type);
                                    }
                                    else
                                    {
                                        cout << "--------------------------------------------" << endl;
                                        cout << "Failed to execute the query! " << mysql_errno(Connection::conn) << endl;
                                        cout << "Please wait a moment..." << endl;

                                        this_thread::sleep_for(chrono::seconds(2));
                                        history(id, type);
                                    }

                                    break;
                                }
                                else if (choice == 'N' || choice == 'n')
                                {
                                    history(id, type);
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
                            history(id, type);
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
                history(id, type);
            }
            
        }
        else if (menu == 2)
        {

            history(id, type);
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

void Rent::approve(string id, string type)
{
    
    User::display_header("Approve Rentals Screen");

    Customer customer;
    Staff staff;

    cout << "1. Continue approving" << endl;
    cout << "2. Go back" << endl << endl;

    do
    {
    
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
            User::display_header("Approve Rentals Screen");
            
      
            query = "SELECT rental.rental_id, CONCAT(fname, ' ', lname), email, model, color, type_name, price, rental_date, return_date, duration, total, rental.status, application_date FROM rental JOIN rental_details ON rental.rental_id = rental_details.rental_id JOIN customer ON rental.customer_id = customer.customer_id JOIN user ON customer.user_id = user.user_id JOIN equipment ON rental_details.equipment_id = equipment.equipment_id JOIN equipment_type ON equipment.equipment_type_id = equipment_type.equipment_type_id WHERE rental.status = 'Not Approved Yet' ORDER BY rental.rental_id DESC";
            qstate = mysql_query(Connection::conn, query.c_str());

            if (!qstate)
            {
                map<int, string> rowIdMap;
                int rowNum = 1;

                cout << left << setw(5) << "No." << setw(15) << "Customer" << setw(25) << "Email" << setw(10) << "Model" << setw(10) << "Color" << setw(15) << "Type" << setw(14) << "Price" << setw(15) << "Rental Date" << setw(15) << "Return Date" << setw(15) << "Duration" << setw(14) << "Total" << setw(20) << "Status" << setw(15) << "Application Date" << endl;

                cout << setfill('-') << setw(150) << "" << endl;
                cout << setfill(' ');

                MYSQL_RES *res = mysql_store_result(Connection::conn);
                MYSQL_ROW row;

                
                while ((row = mysql_fetch_row(res)) != nullptr)
                {

                    cout << left << setw(5) << rowNum
                        << setw(15) << row[1]
                        << setw(25) << row[2]
                        << setw(10) << row[3]
                        << setw(10) << row[4]
                        << setw(15) << row[5]
                        << setw(4) <<  "MYR" << setw(10) << fixed << setprecision(2) << atof(row[6])
                        << setw(15) << row[7]
                        << setw(15) << row[8]
                        << setw(15) << displayDuration(atoi(row[9]))
                        << setw(4) <<  "MYR" << setw(10) << fixed << setprecision(2) << atof(row[10])
                        << setw(20) << row[11]
                        << setw(15) << row[12]
                        << endl;

                    rowIdMap[rowNum] = row[0];

                    rowNum++;
                }
                mysql_free_result(res);
                    
                cout << setfill('-') << setw(150) << "" << endl;
                cout << setfill(' ');

                do
                {
                    cout << "Enter the row No. of the rental: ";
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

                        User::display_header("Approve a Rental Screen");

                        rental_id = rowIdMap[rowId];

                        query = "SELECT equipment.equipment_id, CONCAT(fname, ' ', lname), email, model, color, type_name, price, rental_date, return_date, duration, total, rental.status, application_date FROM rental JOIN rental_details ON rental.rental_id = rental_details.rental_id JOIN customer ON rental.customer_id = customer.customer_id JOIN user ON customer.user_id = user.user_id JOIN equipment ON rental_details.equipment_id = equipment.equipment_id JOIN equipment_type ON equipment.equipment_type_id = equipment_type.equipment_type_id WHERE rental.rental_id = '" + rental_id + "' AND rental.status = 'Not Approved Yet'";
                        qstate = mysql_query(Connection::conn, query.c_str());

                        if (!qstate)
                        {
                            map<int, string> rowIdMap;
                            int rowNum = 1;

                            Equipment equipment;

                            cout << left << setw(5) << "No." << setw(15) << "Customer" << setw(25) << "Email" << setw(10) << "Model" << setw(10) << "Color" << setw(15) << "Type" << setw(14) << "Price" << setw(15) << "Rental Date" << setw(15) << "Return Date" << setw(15) << "Duration" << setw(14) << "Total" << setw(20) << "Status" << setw(15) << "Application Date" << endl;

                            cout << setfill('-') << setw(150) << "" << endl;
                            cout << setfill(' ');

                            MYSQL_RES *res = mysql_store_result(Connection::conn);
                            MYSQL_ROW row;

                            while ((row = mysql_fetch_row(res)) != nullptr)
                            {

                                cout << left << setw(5) << rowNum++
                                    << setw(15) << row[1]
                                    << setw(25) << row[2]
                                    << setw(10) << row[3]
                                    << setw(10) << row[4]
                                    << setw(15) << row[5]
                                    << setw(4) <<  "MYR" << setw(10) << fixed << setprecision(2) << atof(row[6])
                                    << setw(15) << row[7]
                                    << setw(15) << row[8]
                                    << setw(15) << displayDuration(atoi(row[9]))
                                    << setw(4) <<  "MYR" << setw(10) << fixed << setprecision(2) << atof(row[10])
                                    << setw(20) << row[11]
                                    << setw(15) << row[12]
                                    << endl;
                                
                                equipment.equipment_id = row[0];
                            }
                            mysql_free_result(res);
                                
                            cout << setfill('-') << setw(150) << "" << endl;
                            cout << setfill(' ');                            

                            cout << "Are you sure do you want to approve this rental? (Y/N)?" << endl;

                            do
                            {
                                cout << "--------------------------------------------" << endl;
                                cout << "Enter a your choice: ";
                                char choice;
                                cin >> ws >> choice;

                                if (choice == 'Y' || choice == 'y')
                                {

                                    query = "UPDATE rental SET status = 'Approved'  WHERE rental_id = '" + rental_id + "'";
                                    qstate = mysql_query(Connection::conn, query.c_str());
                                    
                                    query = "UPDATE equipment SET status = 'Rented' WHERE equipment_id = '" + equipment.equipment_id + "'";
                                    qstate = mysql_query(Connection::conn, query.c_str());

                                    if (!qstate)
                                    {
                                        cout << "--------------------------------------------" << endl;
                                        cout << "This Rental has been approved successfully!" << endl;
                                        cout << "Please wait a moment..." << endl;

                                        this_thread::sleep_for(chrono::seconds(2));
                                        history(id, type);
                                    }
                                    else
                                    {
                                        cout << "--------------------------------------------" << endl;
                                        cout << "Failed to execute the query! " << mysql_errno(Connection::conn) << endl;
                                        cout << "Please wait a moment..." << endl;

                                        this_thread::sleep_for(chrono::seconds(2));
                                        history(id, type);
                                    }

                                    break;
                                }
                                else if (choice == 'N' || choice == 'n')
                                {
                                    history(id, type);
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
                            history(id, type);
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
                history(id, type);
            }
            
        }
        else if (menu == 2)
        {
            history(id, type);
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
