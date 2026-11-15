#include "connection.h"
#include "user.h"
#include "staff.h"
#include "equipment_type.h"
#include "equipment.h"
#include "rent.h"
#include "report.h"

using namespace std;

// staff menu
void Staff::home(string id, string type)
{

    User::display_header("Staff Menu Screen");

    cout << "1. Manage User" << endl;
    cout << "2. Manage Heavy Equipment Types" << endl;
    cout << "3. Manage Heavy Equipment" << endl;
    cout << "4. Manage Heavy Equipment Rentals" << endl;
    cout << "5. Report" << endl;
    cout << "6. Profile" << endl;
    cout << "7. Logout" << endl;
    
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
            User user;
            user.user(id, type);
            break;
        }
        else if (menu == 2)
        {
            Equipment_Type equipment_type;
            equipment_type.equipment_type(id, type);
            break;
        }
        else if (menu == 3)
        {
            Equipment equipment;
            equipment.equipment(id, type);
            break;
        }
        else if (menu == 4)
        {
            Rent rent;
            rent.history(id, type);
            break;
        }
        else if (menu == 5)
        {
            Report report;
            report.report(id, type);
            break;
        }
        else if (menu == 6)
        {
            User user;
            user.profile(id, type);
            break;
        }
        else if (menu == 7)
        {
            User user;
            user.logout();
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
