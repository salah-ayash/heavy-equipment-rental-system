#include "connection.h"
#include "report.h"
#include "user.h"
#include "staff.h"

bool Report::check_date(string date, string type) {
    if (type == "year") {
        regex pattern("\\d{4}");
        return regex_match(date, pattern);
    } else if (type == "year_month") {
        regex pattern("\\d{4}-\\d{2}");
        return regex_match(date, pattern);
    } else {
        return false;
    }
}

//bool Report::validate_date(string date, string type)
//{
//    struct tm tm;
//    if (type == "year")
//    {
//        if(strptime(date.c_str(), "%Y", &tm) == nullptr)
//        {
//            return false;
//        }
//    }
//    else if (type == "year_month")
//    {
//        if(strptime(date.c_str(), "%Y-%m", &tm) == nullptr)
//        {
//            return false;
//        }
//    }
//    else
//    {
//        return false;
//    }
//
//    time_t t = mktime(&tm);
//    if (t == -1)
//    {
//        return false;
//    }
//
//    return true;
//
//}

 // windows
bool Report::validate_date(string date, string type) {
    int year, month, day;
    if (type == "year") {
        if (sscanf_s(date.c_str(), "%d", &year) == 1) 
            return true;
    } else if (type == "year_month") {
        if (sscanf_s(date.c_str(), "%d-%d", &year, &month) == 2)
            return true;
    } 
    return false;
} 

// report menu
void Report::report(string id, string type)
{

    User::display_header("Report Menu Screen");

    cout << "1. General Analysis" << endl;
    cout << "2. Rents Analysis" << endl;
    cout << "3. Go back" << endl;
    
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
            general(id, type);
            break;
        }
        else if (menu == 2)
        {
            rents(id, type);
            break;
        }
        else if (menu == 3)
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

void Report::general(string id, string type)
{

    User::display_header("General Analysis Menu Screen");

    cout << "1. Customers Analysis" << endl;
    cout << "2. Equipment Types Analysis" << endl;
    cout << "3. Equipment Analysis" << endl;
    cout << "4. Go back" << endl;

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

            User::display_header("Customers Analysis Screen");

            query = "SELECT CONCAT(fname, ' ', lname), email, (SELECT COUNT(rental.rental_id) FROM rental JOIN customer ON rental.customer_id = customer.customer_id AND customer.user_id = user.user_id AND rental.status = 'Not Approved Yet') as 'Not Approved Yet', (SELECT COUNT(rental.rental_id) FROM rental JOIN customer ON rental.customer_id = customer.customer_id AND customer.user_id = user.user_id AND rental.status = 'Approved') as 'Approved', (SELECT COUNT(rental.rental_id) FROM rental JOIN customer ON rental.customer_id = customer.customer_id AND customer.user_id = user.user_id AND rental.status = 'Canceled') as 'Canceled', (SELECT COUNT(rental.rental_id) FROM rental JOIN customer ON rental.customer_id = customer.customer_id AND customer.user_id = user.user_id) as Total FROM user WHERE user_type = 'Customer' GROUP BY user.user_id";
            // query = "SELECT CONCAT(fname, ' ', lname), email, SUM(CASE WHEN rental.status = 'Not Approved Yet' THEN 1 ELSE 0 END) as 'Not Approved', SUM(CASE WHEN rental.status = 'Approved' THEN 1 ELSE 0 END) as Approved, SUM(CASE WHEN rental.status = 'Canceled' THEN 1 ELSE 0 END) as Canceled, SUM(CASE WHEN rental.rental_id IS NOT NULL THEN 1 ELSE 0 END) as Total FROM user LEFT JOIN customer ON user.user_id = customer.user_id LEFT JOIN rental ON customer.customer_id = rental.customer_id WHERE user_type = 'Customer' GROUP BY user.user_id";
            qstate = mysql_query(Connection::conn, query.c_str());

            if (!qstate)
            {
                int rowNum = 1;

                cout << left << setw(5) << "No." << setw(20) << "Customer" << setw(25) << "Email" << setw(20) << "Not Approved Yet" << setw(15) << "Approved" << setw(15) << "Canceled" << setw(15) << "Total Rentals" << endl;

                cout << setfill('-') << setw(150) << "" << endl;
                cout << setfill(' ');

                MYSQL_RES *res = mysql_store_result(Connection::conn);
                MYSQL_ROW row;

                while ((row = mysql_fetch_row(res)) != nullptr)
                {
                    cout << left << setw(5) << rowNum++
                        << setw(20) << row[0]
                        << setw(25) << row[1]
                        << setw(20) << row[2]
                        << setw(15) << row[3]
                        << setw(15) << row[4]
                        << setw(15) << row[5]
                        << setw(15) << row[6]
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
                        general(id, type);
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
                cout << "There is no equipment to list!" << endl;
                cout << "Please wait a moment..." << endl;
                
                this_thread::sleep_for(chrono::seconds(2));
                report(id, type);
            }
            
            break;
        }
        else if (menu == 2)
        {
            User::display_header("Equipment Types Analysis Screen");

            query = "SELECT type_name, (SELECT COUNT(equipment.equipment_id) FROM equipment WHERE equipment.equipment_type_id = equipment_type.equipment_type_id AND equipment.status = 'Available') as Available, (SELECT COUNT(equipment.equipment_id) FROM equipment WHERE equipment.equipment_type_id = equipment_type.equipment_type_id AND equipment.status = 'Reserved') as Reserved, (SELECT COUNT(equipment.equipment_id) FROM equipment WHERE equipment.equipment_type_id = equipment_type.equipment_type_id AND equipment.status = 'Rented') as Rented, (SELECT COUNT(equipment.equipment_id) FROM equipment WHERE equipment.equipment_type_id = equipment_type.equipment_type_id) as Total FROM equipment_type GROUP BY equipment_type.equipment_type_id";
            // query = "SELECT type_name, SUM(CASE WHEN equipment.status = 'Available' THEN 1 ELSE 0 END) as available, SUM(CASE WHEN equipment.status = 'Reserved' THEN 1 ELSE 0 END) as reserved, SUM(CASE WHEN equipment.status = 'Rented' THEN 1 ELSE 0 END) as rented, COUNT(equipment.equipment_id) as total FROM equipment_type LEFT JOIN equipment ON equipment_type.equipment_type_id = equipment.equipment_type_id GROUP BY equipment_type.equipment_type_id";
            qstate = mysql_query(Connection::conn, query.c_str());

            if (!qstate)
            {
                int rowNum = 1;

                cout << left << setw(5) << "No." << setw(15) << "Type" << setw(15) << "Available" << setw(15) << "Reserved" << setw(15) << "Rented" << setw(15) << "Total Equipment" << endl;

                cout << setfill('-') << setw(150) << "" << endl;
                cout << setfill(' ');

                MYSQL_RES *res = mysql_store_result(Connection::conn);
                MYSQL_ROW row;

                while ((row = mysql_fetch_row(res)) != nullptr)
                {
                    cout << left << setw(5) << rowNum++
                        << setw(15) << row[0]
                        << setw(15) << row[1]
                        << setw(15) << row[2]
                        << setw(15) << row[3]
                        << setw(15) << row[4]
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
                        general(id, type);
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
                cout << "There is no equipment to list!" << endl;
                cout << "Please wait a moment..." << endl;
                
                this_thread::sleep_for(chrono::seconds(2));
                report(id, type);
            }

            break;
        }
        else if (menu == 3)
        {
            User::display_header("Equipment Analysis Screen");

            query = "SELECT equipment.model, equipment.status, (SELECT COUNT(rental_details.rental_id) FROM rental_details JOIN rental ON rental_details.rental_id = rental.rental_id WHERE rental_details.equipment_id = equipment.equipment_id AND rental.status = 'Not Approved Yet') as 'Not Approved', (SELECT COUNT(rental_details.rental_id) FROM rental_details JOIN rental ON rental_details.rental_id = rental.rental_id WHERE rental_details.equipment_id = equipment.equipment_id AND rental.status = 'Approved') as Approved, (SELECT COUNT(rental_details.rental_id) FROM rental_details JOIN rental ON rental_details.rental_id = rental.rental_id WHERE rental_details.equipment_id = equipment.equipment_id AND rental.status = 'Canceled') as Canceled, (SELECT COUNT(rental_details.rental_id) FROM rental_details WHERE rental_details.equipment_id = equipment.equipment_id) as Total FROM equipment LEFT JOIN rental_details ON equipment.equipment_id = rental_details.equipment_id GROUP BY equipment.equipment_id";
            // query = "SELECT equipment.model, equipment.status, SUM(CASE WHEN rental.status = 'Not Approved Yet' THEN 1 ELSE 0 END) as 'Not Approved', SUM(CASE WHEN rental.status = 'Approved' THEN 1 ELSE 0 END) as Approved, SUM(CASE WHEN rental.status = 'Canceled' THEN 1 ELSE 0 END) as Canceled, SUM(CASE WHEN rental.rental_id IS NOT NULL THEN 1 ELSE 0 END) as Total FROM equipment LEFT JOIN rental_details ON equipment.equipment_id = rental_details.equipment_id LEFT JOIN rental ON rental_details.rental_id = rental.rental_id GROUP BY equipment.model, equipment.status, equipment.equipment_id";
            qstate = mysql_query(Connection::conn, query.c_str());

            if (!qstate)
            {
                int rowNum = 1;

                cout << left << setw(5) << "No." << setw(15) << "Model" << setw(15) << "Status" << setw(20) << "Not Approved Yet" << setw(15) << "Approved" << setw(15) << "Canceled" << setw(15) << "Total Rentals" << endl;

                cout << setfill('-') << setw(150) << "" << endl;
                cout << setfill(' ');

                MYSQL_RES *res = mysql_store_result(Connection::conn);
                MYSQL_ROW row;

                while ((row = mysql_fetch_row(res)) != nullptr)
                {
                    cout << left << setw(5) << rowNum++
                        << setw(15) << row[0]
                        << setw(15) << row[1]
                        << setw(20) << row[2]
                        << setw(15) << row[3]
                        << setw(15) << row[4]
                        << setw(15) << row[5]
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
                        general(id, type);
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
                cout << "There is no equipment to list!" << endl;
                cout << "Please wait a moment..." << endl;
                
                this_thread::sleep_for(chrono::seconds(2));
                report(id, type);
            }

            break;
        }
        else if (menu == 4)
        {
            report(id, type);
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

void Report::rents(string id, string type)
{
    User::display_header("Rents Analysis Menu Screen");

    cout << "1. Total rentals revenue" << endl;
    cout << "2. Monthly Rents" << endl;
    cout << "3. Yearly Rents" << endl;
    cout << "4. Go back" << endl;

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
            total(id, type);
            break;
        }
        if (menu == 2)
        {
            monthly_menu( id, type);
            break;
        }
        else if (menu == 3)
        {
            yearly_menu( id, type);
            break;
        }
        else if (menu == 4)
        {
            report(id, type);
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

void Report::total(string id, string type)
{
    User::display_header("Total Rentals Revenue Screen");

    query = "SELECT equipment.model, SUM(rental.total) as revenue, MIN(rental.rental_date) as 'From Date', MAX(rental.return_date) as 'To Date' FROM equipment JOIN rental_details ON equipment.equipment_id = rental_details.equipment_id JOIN rental ON rental_details.rental_id = rental.rental_id GROUP BY equipment.model ORDER BY revenue DESC";
    qstate = mysql_query(Connection::conn, query.c_str());

    if (!qstate)
    {
        int rowNum = 1;

        cout << left << setw(5) << "No." << setw(15) << "Model" << setw(20) << "Revenue" << setw(20) << "From Date" << setw(20) << "To Date" << endl;

        cout << setfill('-') << setw(150) << "" << endl;
        cout << setfill(' ');

        MYSQL_RES *res = mysql_store_result(Connection::conn);
        MYSQL_ROW row;

        double revenue = 0;

        while ((row = mysql_fetch_row(res)) != nullptr)
        {
            cout << left << setw(5) << rowNum++
                << setw(15) << row[0]
                << setw(4) <<  "MYR" << setw(16) << fixed << setprecision(2) << atof(row[1])
                << setw(20) << row[2]
                << setw(20) << row[3]
                << endl;

                revenue += atof(row[1]);
        }
        mysql_free_result(res);
            
        cout << setfill('-') << setw(150) << "" << endl;
        cout << setfill(' ');
        cout << "Total Rentals Revenue: " <<  "MYR " << setw(16) << fixed << setprecision(2) << revenue << endl;
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
                rents(id, type);
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
        report(id, type);
    }
}
    
void Report::monthly_menu(string id, string type)
{

    User::display_header("Monthly Rents Menu Screen");

    cout << "1. All Monthly Rents" << endl;
    cout << "2. Custom Monthly Rents" << endl;
    cout << "3. Go back" << endl;

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
            monthly(id, type);
            break;
        }
        else if (menu == 2)
        {
            custom_monthly(id, type);
            break;
        }
        else if (menu == 3)
        {
            rents(id, type);
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

void Report::monthly(string id, string type)
{

    User::display_header("All Monthly Analysis Screen");

    query = "SELECT equipment.model, SUM(total) as revenue, DATE_FORMAT(application_date, '%Y-%m') as month FROM rental JOIN rental_details ON rental.rental_id = rental_details.rental_id JOIN equipment ON rental_details.equipment_id = equipment.equipment_id WHERE rental.status = 'Approved' GROUP BY month, equipment.model ORDER BY month DESC";
    qstate = mysql_query(Connection::conn, query.c_str());

    if (!qstate)
    {
        int rowNum = 1;

        cout << left << setw(5) << "No." << setw(15) << "Model" << setw(20) << "Revenue" << setw(15) << "Month" << endl;

        cout << setfill('-') << setw(150) << "" << endl;
        cout << setfill(' ');

        MYSQL_RES *res = mysql_store_result(Connection::conn);
        MYSQL_ROW row;

        while ((row = mysql_fetch_row(res)) != nullptr)
        {
            cout << left << setw(5) << rowNum++
                << setw(15) << row[0]
                << setw(4) << "MYR" << setw(16) << fixed << setprecision(2) << atof(row[1])
                << setw(15) << row[2]
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
                monthly_menu(id, type);
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
        monthly_menu(id, type);
    }

}

void Report::custom_monthly(string id, string type)
{
    User::display_header("Custom Monthly Analysis Screen");

    cout << "1. Continue" << endl;
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
            bool flag = true;

            do {
            
                string year_month;
            
                do
            {
                cout << "--------------------------------------------" << endl;
                cout << "Enter the rents year and month: ";
                cin >> year_month;

                if (year_month.empty()) {
                    cout << "--------------------------------------------" << endl;
                    cout << "Rents year and month cannot be empty. Please try again." << endl;
                    continue;
                }
                else if (!check_date(year_month, "year_month"))
                {
                    cout << "--------------------------------------------" << endl;
                    cout << "Invalid year and month format. The year month should be in the YYYY-MM format." << endl;
                    continue;
                }
                else if (!validate_date(year_month, "year_month"))
                {
                    cout << "--------------------------------------------" << endl;
                    cout << "Invalid year and month! Please enter a valid date" << endl;
                    continue;
                }
                else
                {
                    break;
                }

            } while (true);

                User::display_header("Custom Monthly Analysis Screen");

                query = "SELECT equipment.model, SUM(total) as revenue, DATE_FORMAT(application_date, '%Y-%m') as month FROM rental JOIN rental_details ON rental.rental_id = rental_details.rental_id JOIN equipment ON rental_details.equipment_id = equipment.equipment_id WHERE rental.status = 'Approved' AND DATE_FORMAT(application_date, '%Y-%m') LIKE '" + year_month + "' GROUP BY month, equipment.model ORDER BY month DESC";
                qstate = mysql_query(Connection::conn, query.c_str());

                if (!qstate)
                {

                    MYSQL_RES* res = mysql_store_result(Connection::conn);
                    MYSQL_ROW row;

                    if (mysql_num_rows(res) > 0)
                    {
                        int rowNum = 1;

                        cout << left << setw(5) << "No." << setw(15) << "Model" << setw(20) << "Revenue" << setw(15) << "Month" << endl;

                        cout << setfill('-') << setw(150) << "" << endl;
                        cout << setfill(' ');

                        while ((row = mysql_fetch_row(res)) != nullptr)
                        {
                            cout << left << setw(5) << rowNum++
                                << setw(15) << row[0]
                                << setw(4) << "MYR" << setw(16) << fixed << setprecision(2) << atof(row[1])
                                << setw(15) << row[2]
                                << endl;
                        }
                        mysql_free_result(res);

                        cout << setfill('-') << setw(150) << "" << endl;
                        cout << setfill(' ');

                    }
                    else
                    {
                        cout << "--------------------------------------------" << endl;
                        cout << "There is no data to display!" << endl;
                    }

                }
                else
                {
                    cout << "--------------------------------------------" << endl;
                    cout << "Failed to execute the query! " << mysql_errno(Connection::conn) << endl;
                    cout << "Please wait a moment..." << endl;

                    this_thread::sleep_for(chrono::seconds(2));
                    monthly_menu(id, type);
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
                        break;
                    }
                    else if (choice == 'N' || choice == 'n')
                    {
                        flag = false;
                        monthly_menu(id, type);
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

        }
        else if (menu == 2)
        {
            monthly_menu(id, type);
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

void Report::yearly_menu(string id, string type)
{
    User::display_header("Yearly Rents Menu Screen");

    cout << "1. All Yearly Rents" << endl;
    cout << "2. Custom Yearly Rents" << endl;
    cout << "3. Go back" << endl;

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
            yearly(id, type);
            break;
        }
        else if (menu == 2)
        {
            custom_yearly(id, type);
            break;
        }
        else if (menu == 3)
        {
            rents(id, type);
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

void Report::yearly(string id, string type)
{
    User::display_header("All Yearly Analysis Screen");
                    
    query = "SELECT equipment.model, SUM(total) as revenue, DATE_FORMAT(application_date, '%Y') as year FROM rental JOIN rental_details ON rental.rental_id = rental_details.rental_id JOIN equipment ON rental_details.equipment_id = equipment.equipment_id WHERE rental.status = 'Approved' GROUP BY year, equipment.model ORDER BY year DESC";
    qstate = mysql_query(Connection::conn, query.c_str());

    if (!qstate)
    {
        int rowNum = 1;

        cout << left << setw(5) << "No." << setw(15) << "Model" << setw(20) << "Revenue" << setw(15) << "Year" << endl;

        cout << setfill('-') << setw(150) << "" << endl;
        cout << setfill(' ');

        MYSQL_RES *res = mysql_store_result(Connection::conn);
        MYSQL_ROW row;

        while ((row = mysql_fetch_row(res)) != nullptr)
        {
            cout << left << setw(5) << rowNum++
                << setw(15) << row[0]
                << setw(4) << "MYR" << setw(16) << fixed << setprecision(2) << atof(row[1])
                << setw(15) << row[2]
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
                yearly_menu(id, type);
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
        yearly_menu(id, type);
    }

}

void Report::custom_yearly(string id, string type)
{
    User::display_header("Custom Yearly Analysis Screen");

    cout << "1. Continue" << endl;
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
            
            bool flag = true;
            
            do
            {
                string year;
                
                do
            {
                cout << "--------------------------------------------" << endl;
                cout << "Enter the rents year: ";
                cin >> year;

                if (year.empty()) {
                    cout << "--------------------------------------------" << endl;
                    cout << "Rents year cannot be empty. Please try again." << endl;
                    continue;
                }
                else if (!check_date(year, "year"))
                {
                    cout << "--------------------------------------------" << endl;
                    cout << "Invalid year format. The year should be in the YYYY format." << endl;
                    continue;
                }
                else if (!validate_date(year, "year"))
                {
                    cout << "--------------------------------------------" << endl;
                    cout << "Invalid year! Please enter a valid date" << endl;
                    continue;
                }
                else
                {
                    break;
                }

            } while (true);

                User::display_header("Custom Yearly Analysis Screen");

                query = "SELECT equipment.model, SUM(total) as revenue, DATE_FORMAT(application_date, '%Y') as year FROM rental JOIN rental_details ON rental.rental_id = rental_details.rental_id JOIN equipment ON rental_details.equipment_id = equipment.equipment_id WHERE rental.status = 'Approved' AND  DATE_FORMAT(application_date, '%Y') = '" + year + "' GROUP BY year, equipment.model ORDER BY year DESC";
                qstate = mysql_query(Connection::conn, query.c_str());

                if (!qstate)
                {

                    MYSQL_RES* res = mysql_store_result(Connection::conn);
                    MYSQL_ROW row;

                    if (mysql_num_rows(res) > 0)
                    {
                        int rowNum = 1;

                        cout << left << setw(5) << "No." << setw(15) << "Model" << setw(20) << "Revenue" << setw(15) << "Year" << endl;

                        cout << setfill('-') << setw(150) << "" << endl;
                        cout << setfill(' ');

                        while ((row = mysql_fetch_row(res)) != nullptr)
                        {
                            cout << left << setw(5) << rowNum++
                                << setw(15) << row[0]
                                << setw(4) << "MYR" << setw(16) << fixed << setprecision(2) << atof(row[1])
                                << setw(15) << row[2]
                                << endl;
                        }
                        mysql_free_result(res);

                        cout << setfill('-') << setw(150) << "" << endl;
                        cout << setfill(' ');

                    }
                    else
                    {
                        cout << "--------------------------------------------" << endl;
                        cout << "There is no data to display!" << endl;
                    }

                }
                else
                {
                    cout << "--------------------------------------------" << endl;
                    cout << "Failed to execute the query! " << mysql_errno(Connection::conn) << endl;
                    cout << "Please wait a moment..." << endl;

                    this_thread::sleep_for(chrono::seconds(2));
                    yearly_menu(id, type);
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
                        break;
                    }
                    else if (choice == 'N' || choice == 'n')
                    {
                        flag = false;
                        yearly_menu(id, type);
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

        }
        else if (menu == 2)
        {
            yearly_menu(id, type);
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
