
#include <iostream>
#include <iomanip>
#include <mysql.h>
#include <thread>
#include <string>
#include <regex>
#include <map>
#include <algorithm>

#include "connection.h"
#include "user.h"

// #include "report.h"

using namespace std;

// Main Function
int main()
{
    cout << "\t\t\t                 -------------------------------------------------" << endl;
    cout << "\t\t\t*****************| Heavy Equipment Rental Management System |*********************" << endl;
    cout << "\t\t\t                 -------------------------------------------------" << endl;

    // Step 1: Connect to MySQL database
    Connection db;
    db.connect();

    // Step 2: Check the database connection
    if (Connection::conn == nullptr)
    {
        cerr << "--------------------------------------------" << endl;
        cerr << "ERROR: Database connection failed!" << endl;
        cerr << "Please check the MySQL server and your database credentials." << endl;
        cerr << "--------------------------------------------" << endl;

        return 1;
    }

    cout << "Database connection verified successfully!" << endl;

    // Step 3: Start the user system
    User usr;
    usr.home();

    // Optional: Test report module
    /*
    Report report;
    report.report("1", "Staff");
    */

    return 0;
}
