#ifndef RENT_H
#define RENT_H

#include <mysql.h>
#include <iostream>
#include <iomanip>
#include <thread>
#include <map>
#include <regex>

using namespace std;

class Rent
{
private:

    string bank;
    string transaction_id;
    string rental_id;

    // string model;
    // string color;
    // double price;
    // string rental_date;
    // string return_date;
    // int duration;
    // double total;

    struct rental_info {
        string equipment_id;
        string model;
        string color;
        double price;
        string rental_date;
        string return_date;
        int duration;
        double total;
    };

    int menu;
    string query;
    int qstate;
    int rowId;

public:

    // Check the date format
    bool check_date(string date);

    bool validate_date(string date);

    bool compare_dates(string rental_date, string end_date);

    int calc_duration(string rental_date, string return_date);

    string displayDuration(int duration);

    bool validate_transaction_id(string transaction_id);

    void rent(string id, string type);

    void history(string id, string type);

    void cancel(string id, string type);

    void approve(string id, string type);

};

#endif