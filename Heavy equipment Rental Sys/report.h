#pragma once
#ifndef REPORT_H
#define REPORT_H

#include <string>
#include <mysql.h>
#include <iostream>
#include <thread>
#include <map>

using namespace std;

// Report Class
class Report
{
private:
    int menu;
    string query;
    int qstate;
    int rowId;
public:

    bool validate_date(string date, string type);

    bool check_date(string date, string type);

    void report(string id, string type);

    void general(string id, string type);

    void rents(string id, string type);

    void total(string id, string type);

    void monthly_menu(string id, string type);

    void monthly(string id, string type);

    void custom_monthly(string id, string type);

    void yearly_menu(string id, string type);

    void yearly(string id, string type);

    void custom_yearly(string id, string type);

};

#endif