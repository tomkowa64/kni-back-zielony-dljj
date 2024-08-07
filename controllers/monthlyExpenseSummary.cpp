#include "monthlyExpenseSummary.h"
#include "..//helpers/operationHelper.h" // for CurrentDate
#include <fstream>

#include <iostream>
#include <sstream>
#include <string>
#include <ctime>
#include <map>
#include <vector>

Date parseDate(const string & dateStr) {
    Date date;
    stringstream ss(dateStr);
    char dot;
    ss >> date.day >> dot >> date.month >> dot >> date.year;
    return date;
}

vector < MonthlyExpenseData > monthlyExpenseSummary() {
    vector < MonthlyExpenseData > summary;
    MonthlyExpenseData data = {0,0,0,0};
    Date operationDate = {0,0,0};

    string line;
    int month = 0;
    int boundaryMonth = 0;
    int boundaryYear = 0;

    fstream operationsFile;
    operationsFile.open("operation.txt", ios::in);
    if (!operationsFile.is_open()) {
        cout << "Unable to open file" << endl;
        return summary;
    }

    boundaryYear = CurrentDate().tm_year + 1900;
    boundaryMonth = CurrentDate().tm_mon - 5;
    if (boundaryMonth <= 0) {
        boundaryMonth += 12;
        boundaryYear -= 1;
    }
    month = boundaryMonth;

    // reads the file line by line
    // if the line starts with a date, it checks if the date is in the range
    // if it is, start adding data to MonthlyExpenseData data
    // if month changes, push the data to summary and reset the data
    while (getline(operationsFile, line)) {
        if (line[3] != '.' || line[6] != '.') continue;
        operationDate = parseDate(line.substr(1));

        if (operationDate.year < boundaryYear || (operationDate.year == boundaryYear && operationDate.month < boundaryMonth)) continue;

        if (operationDate.month != month && month != 0) {
            data.month = month;
            data.year = operationDate.year - (month == 12);
            summary.push_back(data);
            data.income = 0;
            data.expense = 0;
        }
        month = operationDate.month;

        getline(operationsFile, line);
        if (line == "-addFunds") {
            getline(operationsFile, line);
            data.income += stoi(line.substr(1));
        } else if (line == "-withdrawFunds") {
            getline(operationsFile, line);
            data.expense += stoi(line.substr(1));
        }
    }
    data.month = operationDate.month;
    data.year = operationDate.year;
    summary.push_back(data);

    operationsFile.close();

    if (summary[0].year == 0) return {};

    return summary;
}