#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>

using namespace std;

class Customer
{
private:
    int menu;
    int rowId;
    string query;
    int qstate;
public:
    string customer_id;
    string license_no;
    string status;
    string user_id;

    // customer menu
    void customer(string id, string type);
};
#endif
