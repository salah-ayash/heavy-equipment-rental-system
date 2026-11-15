#include "connection.h"
#include "customer.h"
#include "equipment.h"
#include "user.h"
#include "rent.h"

using namespace std;

// customer menu
void Customer::customer(string id, string type) 
{
    
    User::display_header("Customer Screen");

    cout << "1. List equipment" << endl;
    cout << "2. List rental history" << endl;
    cout << "3. Profile" << endl;
    cout << "4. Logout" << endl;

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
            Rent rent;
            rent.rent(id, type);
            break;
        }
        else if (menu == 2)
        {
            Rent rent;
            rent.history(id, type);
            break;
        }
        else if (menu == 3)
        {
            User user;
            user.profile(id, type);
            break;
        }
        else if (menu == 4)
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
